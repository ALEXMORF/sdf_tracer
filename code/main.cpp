#include <stdio.h>

#include "game.cpp"

#include <windows.h>
#include <stdio.h>
#include "win32_kernel.h"

global_variable b32 gGameIsRunning = true;
global_variable int gWindowWidth;
global_variable int gWindowHeight;

LRESULT CALLBACK
Win32MessageCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;
    
    switch (Message)
    {
        case WM_CLOSE:
        {
            gGameIsRunning = false;
        } break;
        
        case WM_SIZE:
        {
            gWindowWidth = LOWORD(LParam);
            gWindowHeight = HIWORD(LParam);
        } break;
        
        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }
    
    return Result;
}

int main()
{
    gWindowWidth = 800; 
    gWindowHeight = 600;
    f32 MSPerFrame = 1000.0f / 60.0f;
    
    HWND Window = Win32CreateWindow(0, gWindowWidth, gWindowHeight, 
                                    "SDF Tracer", "SDFTracerWindowClass",
                                    Win32MessageCallback);
    HDC WindowDC = GetDC(Window);
    Win32InitializeOpengl(WindowDC, 4, 0);
    LoadGLFunctions(Win32GetOpenglFunction);
    wgl_swap_interval_ext *SwapInterval = (wgl_swap_interval_ext *)Win32GetOpenglFunction("wglSwapIntervalEXT");
    ASSERT(SwapInterval);
    SwapInterval(true);
    
    //load platform functions back to global functions
    ReadEntireFile = Win32ReadFileToMemory;
    FreeFile = Win32FreeFileMemory;
    
    u32 MemorySize = MB(64);
    void *Memory = Win32AllocateMemory(MemorySize);
    input Input = {};
    
    f32 LastFrameTime = 0.0f;
    while (gGameIsRunning)
    {
        u64 BeginTime = Win32GetPerformanceCounter();
        
        MSG Message = {};
        while (PeekMessage(&Message, Window, 0, 0, PM_REMOVE))
        {
            switch (Message.message)
            {
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                case WM_KEYUP:
                case WM_SYSKEYUP:
                {
                    b32 KeyDown = (Message.lParam & (1 << 31)) == 0;
                    b32 KeyWasDown = (Message.lParam & (1 << 30)) != 0;
                    b32 AltKeyDown = (Message.lParam & (1 << 29)) != 0;
                    
                    if (KeyDown && !KeyWasDown)
                    {
                        if (AltKeyDown && Message.wParam == VK_RETURN)
                        {
                            Win32ToggleFullscreen(Window);
                        }
                        if (Message.wParam == VK_ESCAPE)
                        {
                            gGameIsRunning = false;
                        }
                    }
                    
                    if (KeyDown != KeyWasDown)
                    {
                        if (Message.wParam == 'A') Input.Left = KeyDown;
                        if (Message.wParam == 'D') Input.Right = KeyDown;
                        if (Message.wParam == 'W') Input.Up = KeyDown;
                        if (Message.wParam == 'S') Input.Down = KeyDown;
                    }
                } break;
                
                default:
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                } break;
            }
        }
        
        f32 dT = LastFrameTime / 1000.0f;
        UpdateAndRender(Memory, MemorySize, gWindowWidth, gWindowHeight, &Input, dT);
        f32 FrameProcTime = Win32GetTimeElapsedInMS(BeginTime, Win32GetPerformanceCounter());
        
        SwapBuffers(WindowDC);
        
        f32 FrameElapsedTime = Win32GetTimeElapsedInMS(BeginTime, Win32GetPerformanceCounter());
        if (FrameElapsedTime < MSPerFrame)
        {
            Sleep((LONG)(MSPerFrame - FrameElapsedTime));
            do
            {
                FrameElapsedTime = Win32GetTimeElapsedInMS(BeginTime, Win32GetPerformanceCounter());
            } 
            while (FrameElapsedTime < MSPerFrame);
        }
        else
        {
            Sleep(1); //sleep anyway
        }
        
        char WindowTitle[100];
        snprintf(WindowTitle, sizeof(WindowTitle), "Proc Time: %.2fms, Frame Time: %.2fms", 
                 FrameProcTime, FrameElapsedTime);
        SetWindowText(Window, WindowTitle);
        
        LastFrameTime = FrameElapsedTime;
    }
    
    return 0;
}


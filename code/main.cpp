#include "renderer.cpp"

#include <windows.h>
#include <stdio.h>
#include "win32_kernel.h"

global_variable b32 gGameIsRunning = true;

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
        
        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }
    
    return Result;
}

int main()
{
    int Width = 800; 
    int Height = 600;
    f32 MSPerFrame = 1000.0f / 60.0f;
    
    HWND Window = Win32CreateWindow(0, Width, Height, 
                                    "SDF Tracer", "SDFTracerWindowClass",
                                    Win32MessageCallback);
    HDC WindowDC = GetDC(Window);
    Win32InitializeOpengl(WindowDC, 4, 0);
    LoadGLFunctions(Win32GetOpenglFunction);
    
    //load platform functions back to global functions
    ReadEntireFile = Win32ReadFileToMemory;
    FreeFile = Win32FreeFileMemory;
    
    u32 MemorySize = MB(64);
    void *Memory = Win32AllocateMemory(MemorySize);
    
    while (gGameIsRunning)
    {
        u64 BeginTime = Win32GetPerformanceCounter();
        
        MSG Message = {};
        while (PeekMessage(&Message, Window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        
        UpdateAndRender(Memory, MemorySize, Width, Height);
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
    }
    
    return 0;
}


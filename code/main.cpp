#include "renderer.cpp"

#include <windows.h>
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
    int Width = 800, Height = 600;
    
    HWND Window = Win32CreateWindow(0, Width, Height, 
                                    "SDF Tracer", "SDFTracerWindowClass",
                                    Win32MessageCallback);
    HDC WindowDC = GetDC(Window);
    Win32InitializeOpengl(WindowDC, 4, 0);
    LoadGLFunctions(Win32GetOpenglFunction);
    
    int BytesPerpixel = 4;
    u32 *BackBuffer = (u32 *)Win32AllocateMemory(Width * Height * BytesPerpixel);
    
    MSG Message = {};
    while (gGameIsRunning)
    {
        while (PeekMessage(&Message, Window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        
        glViewport(0, 0, Width, Height);
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SwapBuffers(WindowDC);
        
        //Render(BackBuffer, Width, Height);
        //Win32BlitBufferToScreen(WindowDC, BackBuffer, Width, Height);
        
        Sleep(2);
    }
    
    return 0;
}


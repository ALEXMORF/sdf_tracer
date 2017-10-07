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
    
    u32 MemorySize = MB(64);
    void *Memory = Win32AllocateMemory(MemorySize);
    
    MSG Message = {};
    while (gGameIsRunning)
    {
        while (PeekMessage(&Message, Window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        
        //load platform functions back to global functions
        ReadEntireFile = Win32ReadFileToMemory;
        FreeFile = Win32FreeFileMemory;
        
        Render(Memory, MemorySize, Width, Height);
        SwapBuffers(WindowDC);
        
        Sleep(2);
    }
    
    return 0;
}


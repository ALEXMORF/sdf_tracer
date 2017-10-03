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
    
    HWND Window = Win32CreateWindow(0, Width, Height, Win32MessageCallback);
    HDC WindowDC = GetDC(Window);
    
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
        
        Render(BackBuffer, Width, Height);
        
        //blit it to the screen
        BITMAPINFO BitmapInfo = {};
        BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
        BitmapInfo.bmiHeader.biWidth = Width;
        BitmapInfo.bmiHeader.biHeight = Height;
        BitmapInfo.bmiHeader.biPlanes = 1;
        BitmapInfo.bmiHeader.biBitCount = 32;
        BitmapInfo.bmiHeader.biCompression = BI_RGB;
        StretchDIBits(WindowDC, 0, 0, Width, Height, 0, 0, Width, Height, 
                      BackBuffer, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
        
        Sleep(10);
    }
    
    return 0;
}


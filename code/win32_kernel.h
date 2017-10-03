u64
Win32GetPerformanceFrequency()
{
    LARGE_INTEGER Result = {};
    QueryPerformanceFrequency(&Result);
    return Result.QuadPart;
}

u64
Win32GetPerformanceCounter()
{
    LARGE_INTEGER Result = {};
    QueryPerformanceCounter(&Result);
    return Result.QuadPart;
}

inline f32
Win32GetTimeElapsedInMS(u64 BeginCounter, u64 EndCounter)
{
    f32 Result = (f32)(EndCounter - BeginCounter) / (f32)Win32GetPerformanceFrequency();
    Result *= 1000.0f;
    return Result;
}

struct win32_window_dimension
{
    int Width, Height;
};

inline win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result = {};
    
    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;
    
    return Result;
}

//NOTE(chen): RECT Is in screen-coordinate
inline RECT
Win32GetClientRect(HWND Window)
{
    RECT Result = {};
    GetWindowRect(Window, &Result);
    
    POINT ClientUpperLeft = {Result.left, Result.top};
    POINT ClientLowerRight = {Result.right, Result.bottom};
    ScreenToClient(Window, &ClientUpperLeft);
    ScreenToClient(Window, &ClientLowerRight);
    
    i32 SideBorderSize = -ClientUpperLeft.x;
    i32 TopBorderSize = -ClientUpperLeft.y;
    
    Result.left += SideBorderSize;
    Result.top += TopBorderSize;
    Result.right += SideBorderSize;
    Result.bottom += SideBorderSize;
    
    return Result;
}

void *
Win32AllocateMemory(size_t MemorySize)
{
    return VirtualAlloc(0, MemorySize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
}

global_variable WINDOWPLACEMENT GlobalWindowPosition;
internal void
Win32ToggleFullscreen(HWND Window)
{
    DWORD Style = GetWindowLong(Window, GWL_STYLE);
    if (Style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO mi = { sizeof(mi) };
        if (GetWindowPlacement(Window, &GlobalWindowPosition) &&
            GetMonitorInfo(MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY), &mi))
        {
            SetWindowLong(Window, GWL_STYLE,
                          Style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(Window, HWND_TOP,
                         mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLong(Window, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(Window, &GlobalWindowPosition);
        SetWindowPos(Window, NULL, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

internal HWND
Win32CreateWindow(HINSTANCE InstanceHandle, i32 WindowWidth, i32 WindowHeight,
                  char *WindowName, char *WindowClassName, WNDPROC WindowProcedure)
{
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
    WindowClass.lpfnWndProc = WindowProcedure;
    WindowClass.hInstance = InstanceHandle;
    WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
    WindowClass.lpszClassName = WindowClassName;
    RegisterClass(&WindowClass);
    
    DWORD WindowStyle = WS_VISIBLE|WS_OVERLAPPEDWINDOW;
    
    //find right window size
    RECT WindowSize = {};
    {
        HDC ScreenDeviceContext = GetWindowDC(0);
        
        int ScreenWidth = GetDeviceCaps(ScreenDeviceContext, HORZRES);
        int ScreenHeight = GetDeviceCaps(ScreenDeviceContext, VERTRES);
        
        int WindowLeft = (ScreenWidth - WindowWidth) / 2;
        int WindowTop = (ScreenHeight - WindowHeight) / 2;
        
        WindowSize = {WindowLeft, WindowTop, WindowLeft + WindowWidth, WindowTop + WindowHeight};
        AdjustWindowRect(&WindowSize, WindowStyle, FALSE);
        
        ReleaseDC(0, ScreenDeviceContext);
    }
    
    HWND Window = CreateWindow(
        WindowClassName,
        WindowName,
        WindowStyle, 
        WindowSize.left,
        WindowSize.top,
        WindowSize.right - WindowSize.left,
        WindowSize.bottom - WindowSize.top,
        0,
        0,
        InstanceHandle,
        0);
    
    return Window;
}

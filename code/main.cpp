#include "kernel.h"
#include <windows.h>
#include "win32_kernel.h"

int main()
{
    HWND Window = Win32CreateWindow(0, 800, 600, DefWindowProc);
    return 0;
}


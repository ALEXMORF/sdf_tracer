/*
TODO:

. Time the loop properly and lock the framerate at 60FPS

*/

internal void 
Render(u32 *ScreenBuffer, int Width, int Height)
{
    for (int I = 0; I < Width * Height; ++I)
    {
        ScreenBuffer[I] = 0x0000ff;
    }
}

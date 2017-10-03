/*
TODO:

. Handle screen aspect ratio
. Time the loop properly and lock the framerate at 60FPS

*/

#include "kernel.h"

const f32 EPSILON = 0.001f;
const i32 MAX_MARCH_STEP = 100;
const f32 MAX_DEPTH = 50.0f;

typedef f32 signed_distance_function(v3 P);

inline f32
SignedDistanceToSphere(v3 P)
{
    return Len(P) - 1.0f;
}

inline f32
SignedDistanceToScene(v3 P)
{
    return SignedDistanceToSphere(P);
}

inline v3
Gradient(signed_distance_function *Func, v3 P)
{
    return Normalize(V3(Func({P.X + EPSILON, P.Y, P.Z}) - Func({P.X - EPSILON, P.Y, P.Z}),
                        Func({P.X, P.Y + EPSILON, P.Z}) - Func({P.X, P.Y - EPSILON, P.Z}),
                        Func({P.X, P.Y, P.Z + EPSILON}) - Func({P.X, P.Y, P.Z - EPSILON})
                        ));
}

internal void 
Render(u32 *ScreenBuffer, int Width, int Height)
{
    v3 Camera = {0.0f, 0.0f, -5.0f};
    
    u32 *Pixel = ScreenBuffer;
    
    f32 ViewPlaneDepth = 0.5f / tanf(DegreeToRadian(22.5f));
    for (int Y = 0; Y < Height; ++Y)
    {
        for (int X = 0; X < Width; ++X)
        {
            v3 ViewRay = {};
            ViewRay.X = ((f32)X / (f32)Width) - 0.5f;
            ViewRay.Y = ((f32)Y / (f32)Height) - 0.5f;
            ViewRay.Z = ViewPlaneDepth;
            ViewRay = Normalize(ViewRay);
            
            float Depth = 0.0f;
            for (int I = 0; I < MAX_MARCH_STEP && Depth < MAX_DEPTH; ++I)
            {
                v3 LastP = Camera + Depth * ViewRay;
                float Dist = SignedDistanceToScene(LastP);
                if (Dist < EPSILON) break;
                Depth += Dist;
            }
            
            v3 ClosestP = Camera + Depth * ViewRay;
            if (SignedDistanceToScene(ClosestP) < EPSILON)
            {
                v3 Normal = Gradient(SignedDistanceToScene, ClosestP);
                f32 Intensity = Dot(Normal, -ZAxis());
                u8 Red = (u8)(255.0f * Intensity);
                *Pixel++ = Red << 16;
            }
            else
            {
                *Pixel++ = 0;
            }
        }
    }
}

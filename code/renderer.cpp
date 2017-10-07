/*
TODO:

(READ)
. Read more about SDF rendering

(CODE)
. Write a shader building routine
. Time the loop properly and lock the framerate at 60FPS
. Make it so that our simulation is frame-independent (with dT per frame)
. Use the DLL hotloader
. Use transform matrices to transform meshes and reposition camera
. Offload work onto GPU

*/

#include "kernel.h"

internal void
Render(int Width, int Height)
{
    glViewport(0, 0, Width, Height);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* NOTE(Chen): commented out because shader is required to make this work
    glBindVertexArray(ScreenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
*/
}

/*
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
    float AspectRatio = (f32)Width / (f32)Height;
    
    v3 Camera = {0.0f, 0.0f, -5.0f};
    local_persist quaternion LightDirRotation = Quaternion();
    LightDirRotation = LightDirRotation * Quaternion(YAxis(), DegreeToRadian(3.0f));
    v3 LightDir = Rotate(ZAxis(), LightDirRotation);
    
    u32 *Pixel = ScreenBuffer;
    
    f32 ViewPlaneDepth = 0.5f / tanf(DegreeToRadian(22.5f));
    for (int Y = 0; Y < Height; ++Y)
    {
        for (int X = 0; X < Width; ++X)
        {
            v3 ViewRay = {};
            ViewRay.X = AspectRatio * (((f32)X / (f32)Width) - 0.5f);
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
                float Intensity = Clamp(Dot(Normal, -LightDir), 0.2f, 1.0f);
                v3 Color = Intensity * V3(0.8f, 0.8f, 0.8f);
                
                u8 R = (u8)(255.0f * Color.X);
                u8 G = (u8)(255.0f * Color.Y);
                u8 B = (u8)(255.0f * Color.Z);
                
                *Pixel++ = (R << 16) | (G << 8) | B;
            }
            else
            {
                *Pixel++ = 0;
            }
        }
    }
}
*/
/*
TODO:

(READ)
. Read more about SDF rendering

(WORKFLOW)
. Handle shader error
 . Hotload shader
 
(CODE)
. Ambient Occlusion by sampling SDF along normal
. Soft Shadow by SDF from light caster
. Lift view-ray computation up to vertex shader???
. Have shapes other than sphere
. Have it so objects have their own color
. Global illumination with monte carl integration ???

*/

#include "kernel.h"
#include "game.h"
#include "renderer.cpp"

internal void
UpdateAndRender(void *Memory, u32 MemorySize, int Width, int Height, 
                input *Input, f32 dT)
{
    ASSERT(sizeof(game_state) < MemorySize);
    game_state *GameState = (game_state *)Memory;
    if (!GameState->IsInitialized)
    {
        GameState->Renderer.ScreenVAO = BuildScreenVAO();
        GameState->Renderer.ShaderProgram = BuildShaderProgram(
            "../code/vert.glsl", 
            "../code/frag.glsl");
        GameState->LightDirection = Normalize(V3(0.0f, -0.5f, 0.7f));
        GameState->CameraP = {-2.0f, 3.0f, -5.0f};
        
        GameState->IsInitialized = true;
    }
    
    f32 MoveSpeed = 5.0f * dT;
    if (Input->Left) GameState->CameraP.X -= MoveSpeed;
    if (Input->Right) GameState->CameraP.X += MoveSpeed;
    if (Input->Up) GameState->CameraP.Y += MoveSpeed;
    if (Input->Down) GameState->CameraP.Y -= MoveSpeed;
    
    BeginRender(&GameState->Renderer, Width, Height, 
                GameState->CameraP, GameState->LightDirection);
#if 0
    f32 SphereSize = 1.0f + 0.1f*sinf(5.0f*Time);
#else
    f32 SphereSize = 1.0f;
#endif
    DrawSphere(&GameState->Renderer, V3(0.0f, 0.0f, 0.0f), SphereSize);
    DrawSphere(&GameState->Renderer, V3(1.0f, 0.0f, 0.0f), SphereSize);
    DrawSphere(&GameState->Renderer, V3(-1.0f, 0.0f, 0.0f), SphereSize);
    DrawSphere(&GameState->Renderer, V3(0.0f, 1.0f, 0.0f), SphereSize);
    DrawSphere(&GameState->Renderer, V3(0.0f, -1.0f, 0.0f), SphereSize);
    DrawPlane(&GameState->Renderer, V3(0.0f, 0.0f, 0.0f), V3(0.0f, 1.0f, 0.0f));
    EndRender(&GameState->Renderer);
    
    glFinish(); //force opengl sychronize to measure performance
}

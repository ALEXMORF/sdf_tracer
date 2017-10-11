/*
TODO:

(READ)
. Read more about SDF rendering

(WORKFLOW)
. Handle shader error
 . Hotload shader
 
(CODE)
. Soft Shadow by SDF from light caster
. move shaders back into code so that main.exe can be executed standalone?
. Can we do GI color-bleeding using the fake AO scheme?
. Profile by using iteration-based coloring
. Integrate IMGUI into this project
. Lift view-ray computation up to vertex shader???
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
        
        GameState->LightDirection = Normalize(V3(-0.3f, -0.5f, 0.7f));
        GameState->CameraP = {-2.0f, 3.0f, -5.0f};
        
        GameState->IsInitialized = true;
    }
    
    f32 MoveSpeed = 5.0f * dT;
    if (Input->Left) GameState->CameraP.X -= MoveSpeed;
    if (Input->Right) GameState->CameraP.X += MoveSpeed;
    if (Input->Up) GameState->CameraP.Y += MoveSpeed;
    if (Input->Down) GameState->CameraP.Y -= MoveSpeed;
    
    local_persist f32 Time = 0.0f;
    Time += dT;
    
    renderer *Renderer = &GameState->Renderer;
    BeginRender(Renderer, Width, Height, GameState->CameraP, GameState->LightDirection);
    DrawSphere(Renderer, V3(1.0f * sinf(Time), 1.0f, 1.0f * cosf(Time)), 1.0f,
               V3(0.5f, 0.8f, 0.8f));
    DrawBox(Renderer, V3(3.0f, 0.5f, 0.0f), V3(1.0f, 1.0f, 1.0f),
            V3(0.8f, 0.5f, 0.8f));
    DrawPlane(Renderer, V3(0.0f, 0.0f, 0.0f), V3(0.0f, 1.0f, 0.0f),
              V3(1.0f, 1.0f, 1.0f));
    EndRender(Renderer);
    
    glFinish(); //force opengl sychronize to measure performance
}

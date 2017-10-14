/*
TODO:

(READ)
. Read more about SDF rendering

(CODE)

. Hotload shader
. Handle shader error
  . Bring back specular highlight
. Our current renderer API is too restricting, abandon it and adapt new method
. Shape blending
. Profile by using iteration-based coloring
. Integrate IMGUI into this project
. Global illumination with monte carl integration ???

(FINAL)
. Move shaders back into code so that main.exe can be executed standalone?

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
        
        GameState->LightDirection = Normalize(V3(-0.3f, -0.7f, 0.2f));
        GameState->CameraP = {-2.0f, 3.0f, -2.0f};
        
        GameState->IsInitialized = true;
    }
    
    f32 MoveSpeed = 3.0f * dT;
    if (Input->Left) GameState->CameraP.X -= MoveSpeed;
    if (Input->Right) GameState->CameraP.X += MoveSpeed;
    if (Input->Up) GameState->CameraP.Y += MoveSpeed;
    if (Input->Down) GameState->CameraP.Y -= MoveSpeed;
    
    local_persist f32 Time = 0.0f;
    Time += dT;
    
    local_persist quaternion TetrahedronRotation = Quaternion();
    TetrahedronRotation *= Quaternion(YAxis(), DegreeToRadian(-30.0f)*dT);
    mat4 TetraRotationMat = QuaternionToMat4(TetrahedronRotation);
    
    renderer *Renderer = &GameState->Renderer;
    BeginRender(Renderer, Width, Height, GameState->CameraP, GameState->LightDirection);
    glUploadMatrix4(Renderer->ShaderProgram, "ShapeRotation", 
                    &TetraRotationMat);
    
    DrawSphere(Renderer, V3(0.0f, 1.0f, 0.0), 0.8f,
               V3(0.7f, 0.7f, 0.7f));
#if 1
    DrawBox(Renderer, V3(3.0f, 0.5f, 0.0f), V3(1.0f, 1.0f, 1.0f),
            V3(0.6f, 0.6f, 0.6f));
    DrawBox(Renderer, V3(2.0f, 0.5f, 3.0f), V3(1.0f, 1.0f, 1.0f),
            V3(0.6f, 0.6f, 0.6f));
#endif
    DrawPlane(Renderer, V3(0.0f, 0.0f, 0.0f), V3(0.0f, 1.0f, 0.0f),
              V3(0.8f, 0.8f, 0.8f));
    EndRender(Renderer);
    
    glFinish(); //force opengl sychronize to measure performance
}

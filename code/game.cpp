/*
TODO:

(READ)
. Read more about SDF rendering

(WORKFLOW)
. Handle shader error
 . Hotload shader
 
(CODE)
. Have shapes other than sphere
. Add in plane as object
. Ambient Occlusion by sampling SDF along normal
. Lift view-ray computation up to vertex shader???
. Global illumination with monte carl integration ???

*/

#include "kernel.h"
#include "game.h"
#include "renderer.cpp"

inline loaded_file
LoadEntireFile(char *Path)
{
    loaded_file Result = {};
    Result.Data = ReadEntireFile(Path, &Result.Size);
    return Result;
}

internal GLuint
ReadAndCompileShader(char *Path, GLenum Type)
{
    GLuint Shader = glCreateShader(Type);
    
    loaded_file Source = LoadEntireFile(Path);
    if (!Source.Data)
    {
        ASSERT(!"Can't find file");
    }
    glShaderSource(Shader, 1, &Source.Data, 0);
    glCompileShader(Shader);
    GLint CompileStatus = 0;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &CompileStatus);
    if (CompileStatus != GL_TRUE)
    {
        GLsizei ErrorMsgLength = 0;
        GLchar ErrorMsg[1024];
        glGetShaderInfoLog(Shader, sizeof(ErrorMsg), 
                           &ErrorMsgLength, ErrorMsg);
        ASSERT(!"fail to compile vertex shader");
    }
    
    return Shader;
}

internal GLuint
BuildShaderProgram(char *VertShaderPath, char *FragShaderPath)
{
    GLuint VertShader = ReadAndCompileShader(VertShaderPath, GL_VERTEX_SHADER);
    GLuint FragShader = ReadAndCompileShader(FragShaderPath, GL_FRAGMENT_SHADER);
    //TODO(Chen): link up to a program then delete & detach the shaders
    
    GLuint Program = glCreateProgram();
    glAttachShader(Program, VertShader);
    glAttachShader(Program, FragShader);
    glLinkProgram(Program);
    GLint ProgramLinked = 0;
    glGetProgramiv(Program, GL_LINK_STATUS, &ProgramLinked);
    if (ProgramLinked != GL_TRUE)
    {
        GLsizei ErrorMsgLength = 0;
        GLchar ErrorMsg[1024];
        glGetProgramInfoLog(Program, sizeof(ErrorMsg),
                            &ErrorMsgLength, ErrorMsg);
        ASSERT(!"Failed to link shaders");
    }
    
    glDetachShader(Program, VertShader);
    glDeleteShader(VertShader);
    glDetachShader(Program, FragShader);
    glDeleteShader(FragShader);
    
    return Program;
}

internal void
UpdateAndRender(void *Memory, u32 MemorySize, int Width, int Height, 
                input *Input, f32 dT)
{
    ASSERT(sizeof(game_state) < MemorySize);
    game_state *GameState = (game_state *)Memory;
    if (!GameState->IsInitialized)
    {
        GameState->Renderer.ScreenVAO = BuildScreenVAO();
        GameState->Renderer.ShaderProgram = BuildShaderProgram("../code/vert.glsl", 
                                                               "../code/frag.glsl");
        GameState->LightDirection = {0.0f, 0.0f, 1.0f};
        GameState->CameraP = {0.0f, 0.0f, -3.0f};
        
        GameState->IsInitialized = true;
    }
    
    f32 MoveSpeed = 1.0f * dT;
    if (Input->Left) GameState->CameraP.X -= MoveSpeed;
    if (Input->Right) GameState->CameraP.X += MoveSpeed;
    if (Input->Up) GameState->CameraP.Y += MoveSpeed;
    if (Input->Down) GameState->CameraP.Y -= MoveSpeed;
    
    BeginRender(&GameState->Renderer, Width, Height, 
                GameState->CameraP, GameState->LightDirection);
    DrawSphere(&GameState->Renderer, V3(0.0f, 0.0f, 0.0f), 1.0f);
    DrawSphere(&GameState->Renderer, V3(2.5f, 0.0f, 0.0f), 1.0f);
    EndRender(&GameState->Renderer);
    
    glFinish(); //force opengl sychronize to measure performance
}

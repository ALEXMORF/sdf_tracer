/*
TODO:

(READ)
. Read more about SDF rendering

(CODE)
. Handle shader error
 . Hotload shader
 . Make it so that our simulation is frame-independent (with dT per frame)
. Use transform matrices to transform and reposition camera

*/

#include "kernel.h"
#include "renderer.h"

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
UpdateAndRender(void *Memory, u32 MemorySize, int Width, int Height, f32 dT)
{
    ASSERT(sizeof(render_state) < MemorySize);
    render_state *RS = (render_state *)Memory;
    if (!RS->IsInitialized)
    {
        RS->ScreenVAO = BuildScreenVAO();
        RS->ShaderProgram = BuildShaderProgram("../code/vert.glsl", 
                                               "../code/frag.glsl");
        
        RS->LightDirection = {0.0f, 0.0f, 1.0f};
        RS->IsInitialized = true;
    }
    
    glViewport(0, 0, Width, Height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    RS->LightDirection = Rotate(RS->LightDirection, 
                                Quaternion(YAxis(), DegreeToRadian(30.0f * dT)));
    
    glUseProgram(RS->ShaderProgram);
    glUploadVec2(RS->ShaderProgram, "ScreenSize", V2(Width, Height));
    glUploadVec3(RS->ShaderProgram, "LightDirection", RS->LightDirection);
    glBindVertexArray(RS->ScreenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    glFinish(); //force opengl sychronize to measure performance
}

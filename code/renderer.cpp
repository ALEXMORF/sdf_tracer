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
BeginRender(renderer *RS, int Width, int  Height, v3 CameraP, v3 LightDirection)
{
    glViewport(0, 0, Width, Height);
    
    mat4 ViewRotation = Mat4LookAt(CameraP, {});
    ViewRotation.Data[3][0] = 0.0f;
    ViewRotation.Data[3][1] = 0.0f;
    ViewRotation.Data[3][2] = 0.0f;
    
    glUseProgram(RS->ShaderProgram);
    glUploadVec2(RS->ShaderProgram, "ScreenSize", V2(Width, Height));
    glUploadVec3(RS->ShaderProgram, "LightDirection", LightDirection);
    glUploadVec3(RS->ShaderProgram, "CameraP", CameraP);
    glUploadMatrix4(RS->ShaderProgram, "ViewRotation", &ViewRotation);
    
    RS->ShapeCount = 0;
}

inline shape *
PushShape(renderer *RS, v3 Color)
{
    ASSERT(RS->ShapeCount < ARRAY_COUNT(RS->Shapes));
    shape *NewShape = RS->Shapes + RS->ShapeCount++;
    NewShape->Color = Color;
    return NewShape;
}

inline void
DrawSphere(renderer *RS, v3 P, f32 Radius, v3 Color)
{
    shape *NewShape = PushShape(RS, Color);
    NewShape->Type = SHAPE_TYPE_SPHERE;
    NewShape->AsSphere.P = P;
    NewShape->AsSphere.Radius = Radius;
}

inline void
DrawPlane(renderer *RS, v3 P, v3 Normal, v3 Color)
{
    shape *NewShape = PushShape(RS, Color);
    NewShape->Type = SHAPE_TYPE_PLANE;
    NewShape->AsPlane.P = P;
    NewShape->AsPlane.Normal = Normal;
}

inline void
DrawBox(renderer *RS, v3 P, v3 Dim, v3 Color)
{
    shape *NewShape = PushShape(RS, Color);
    NewShape->Type = SHAPE_TYPE_BOX;
    NewShape->AsBox.P = P;
    NewShape->AsBox.Dim = Dim;
}

internal void
EndRender(renderer *RS)
{
    glUploadInt32(RS->ShaderProgram, "ShapeCount", RS->ShapeCount);
    for (int ShapeIndex = 0; ShapeIndex < RS->ShapeCount; ++ShapeIndex)
    {
        shape *Shape = RS->Shapes + ShapeIndex;
        char UniformID[50];
        
        snprintf(UniformID, sizeof(UniformID), "Shapes[%d].Type", ShapeIndex);
        glUploadInt32(RS->ShaderProgram, UniformID, Shape->Type);
        
        snprintf(UniformID, sizeof(UniformID), "Shapes[%d].Color", ShapeIndex);
        glUploadVec3(RS->ShaderProgram, UniformID, Shape->Color);
        
        switch (Shape->Type)
        {
            case SHAPE_TYPE_SPHERE:
            {
                snprintf(UniformID, sizeof(UniformID), "Shapes[%d].P", ShapeIndex);
                glUploadVec3(RS->ShaderProgram, UniformID, Shape->AsSphere.P);
                
                snprintf(UniformID, sizeof(UniformID), "Shapes[%d].Radius", ShapeIndex);
                glUploadFloat(RS->ShaderProgram, UniformID, Shape->AsSphere.Radius);
            } break;
            
            case SHAPE_TYPE_BOX:
            {
                snprintf(UniformID, sizeof(UniformID), "Shapes[%d].P", ShapeIndex);
                glUploadVec3(RS->ShaderProgram, UniformID, Shape->AsBox.P);
                
                snprintf(UniformID, sizeof(UniformID), "Shapes[%d].Dim", ShapeIndex);
                glUploadVec3(RS->ShaderProgram, UniformID, Shape->AsBox.Dim);
            } break;
            
            case SHAPE_TYPE_PLANE:
            {
                snprintf(UniformID, sizeof(UniformID), "Shapes[%d].P", ShapeIndex);
                glUploadVec3(RS->ShaderProgram, UniformID, Shape->AsPlane.P);
                
                snprintf(UniformID, sizeof(UniformID), "Shapes[%d].Normal", ShapeIndex);
                glUploadVec3(RS->ShaderProgram, UniformID, Shape->AsPlane.Normal);
            } break;
            
            default:
            {
                ASSERT(!"Unknown shape type");
            } break;
        }
    }
    
    glBindVertexArray(RS->ScreenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


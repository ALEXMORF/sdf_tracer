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

inline void
DrawSphere(renderer *RS, v3 P, f32 Radius)
{
    ASSERT(RS->ShapeCount < ARRAY_COUNT(RS->Shapes));
    shape *NewShape = RS->Shapes + RS->ShapeCount++;
    NewShape->Type = SHAPE_TYPE_SPHERE;
    NewShape->AsSphere.P = P;
    NewShape->AsSphere.Radius = Radius;
}

internal void
EndRender(renderer *RS)
{
    glUploadInt32(RS->ShaderProgram, "ShapeCount", RS->ShapeCount);
    for (int ShapeIndex = 0; ShapeIndex < RS->ShapeCount; ++ShapeIndex)
    {
        shape *Shape = RS->Shapes + ShapeIndex;
        ASSERT(Shape->Type == SHAPE_TYPE_SPHERE); //only handle sphere for now
        
        char UniformID[50];
        
        snprintf(UniformID, sizeof(UniformID), "Shapes[%d].P", ShapeIndex);
        glUploadVec3(RS->ShaderProgram, UniformID, Shape->AsSphere.P);
        
        snprintf(UniformID, sizeof(UniformID), "Shapes[%d].Radius", ShapeIndex);
        glUploadFloat(RS->ShaderProgram, UniformID, Shape->AsSphere.Radius);
    }
    
    glBindVertexArray(RS->ScreenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


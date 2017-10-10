enum shape_type
{
    SHAPE_TYPE_SPHERE,
    SHAPE_TYPE_COUNT,
};

struct shape
{
    shape_type Type;
    
    union 
    {
        struct 
        {
            v3 P;
            f32 Radius;
        } AsSphere;
    };
};

struct renderer
{
    GLuint ScreenVAO;
    GLuint ShaderProgram;
    
    shape Shapes[100];
    int ShapeCount;
};



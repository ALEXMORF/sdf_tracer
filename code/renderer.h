enum shape_type
{
    SHAPE_TYPE_SPHERE = 0,
    SHAPE_TYPE_PLANE,
    SHAPE_TYPE_BOX,
    SHAPE_TYPE_COUNT,
};

struct shape
{
    shape_type Type;
    v3 Color;
    
    union 
    {
        struct 
        {
            v3 P;
            f32 Radius;
        } AsSphere;
        
        struct
        {
            v3 P;
            v3 Dim;
        } AsBox;
        
        struct
        {
            v3 P;
            v3 Normal;
        } AsPlane;
    };
};

struct renderer
{
    GLuint ScreenVAO;
    GLuint ShaderProgram;
    
    shape Shapes[100];
    int ShapeCount;
};



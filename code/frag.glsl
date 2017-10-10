#version 400 core

struct shape
{
    vec3 P;
    float Radius;
};

uniform vec2 ScreenSize;
uniform vec3 LightDirection;
uniform vec3 CameraP;
uniform mat4 ViewRotation;

uniform shape[50] Shapes;
uniform int ShapeCount;

in vec2 FragTexCoord;
out vec4 OutColor;

const float FOV = 45.0;
const float HFOV = FOV * 0.5;

const float EPSILON = 0.001;
const float MAX_MARCH_STEP = 200;
const float MAX_DEPTH = 100;

float SignedDistanceToScene(vec3 P)
{
    float MinDistance = MAX_DEPTH;
    
    for (int ShapeIndex = 0; ShapeIndex < ShapeCount; ++ShapeIndex)
    {
        float DistanceToShape = (distance(P, Shapes[ShapeIndex].P) - 
                                 Shapes[ShapeIndex].Radius);
        if (DistanceToShape < MinDistance)
        {
            MinDistance = DistanceToShape;
        }
    }
    
    return MinDistance;
}

float SDTS(vec3 P)
{
    return SignedDistanceToScene(P);
}

vec3 Gradient(vec3 P)
{
    return normalize(vec3(SDTS(vec3(P.x + EPSILON, P.y, P.z)) - SDTS(vec3(P.x - EPSILON, P.y, P.z)),
                          SDTS(vec3(P.x, P.y + EPSILON, P.z)) - SDTS(vec3(P.x, P.y - EPSILON, P.z)),
                          SDTS(vec3(P.x, P.y, P.z + EPSILON)) - SDTS(vec3(P.x, P.y, P.z - EPSILON))));
}

void main()
{
    vec3 LightDir = vec3(0.0, 0.0, 1.0);
    
    vec3 ViewRay;
    ViewRay.x = (ScreenSize.x / ScreenSize.y) * (FragTexCoord.x - 0.5);
    ViewRay.y = FragTexCoord.y - 0.5;
    ViewRay.z = 0.5 / tan(radians(HFOV));
    ViewRay = normalize(ViewRay);
    ViewRay = ViewRay * inverse(mat3(ViewRotation));
    
    bool RayHit = false;
    float Depth = 0.0;
    for (int I = 0; I < MAX_MARCH_STEP && Depth < MAX_DEPTH; ++I)
    {
        float Dist = SignedDistanceToScene(CameraP + Depth * ViewRay);
        if (Dist < EPSILON)
        {
            RayHit = true;
            break;
        }
        Depth += Dist;
    }
    
    if (RayHit)
    {
        vec3 Normal = Gradient(CameraP + Depth * ViewRay);
        float Intensity = 0.2 + 0.8*max(dot(Normal, -LightDirection), 0.0);
        vec3 Color = vec3(1.0, 0.0, 0.0) * Intensity;
        OutColor = vec4(Color, 1.0);
    }
    else
    {
        OutColor = vec4(0.0, 1.0, 1.0, 1.0);
    }
}


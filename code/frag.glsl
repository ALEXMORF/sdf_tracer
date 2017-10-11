#version 400 core

#define SHAPE_TYPE_SPHERE 0
#define SHAPE_TYPE_PLANE  1

struct shape
{
    int Type;
    
    vec3 P;
    vec3 Normal;
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
        float DistanceToShape = 0.0f;
        
        if (Shapes[ShapeIndex].Type == SHAPE_TYPE_SPHERE)
        {
            DistanceToShape = (distance(P, Shapes[ShapeIndex].P) - 
                               Shapes[ShapeIndex].Radius);
        }
        else if (Shapes[ShapeIndex].Type == SHAPE_TYPE_PLANE)
        {
            DistanceToShape = dot((P - Shapes[ShapeIndex].P), Shapes[ShapeIndex].Normal);
        }
        
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

float
GetOcclusionFactor(vec3 P, vec3 Normal)
{
    float AORadiusDelta = 0.2f;
    
    float AOFactor = 1.0;
    for (int I = 1; I <= 5; ++I)
    {
        float SampleDist = AORadiusDelta * float(I);
        float Diff = SampleDist - SignedDistanceToScene(P + Normal * SampleDist);
        AOFactor -= (1.0 / pow(2, float(I))) * Diff;
    }
    
    return AOFactor;
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
        vec3 HitP = CameraP + Depth * ViewRay;
        vec3 Normal = Gradient(HitP);
        float AOFactor = GetOcclusionFactor(HitP, Normal);
        float Intensity = AOFactor * (0.3 + 0.7*max(dot(Normal, -LightDirection), 0.0));
        vec3 Color = vec3(0.8, 0.8, 0.8) * Intensity;
        OutColor = vec4(Color, 1.0);
    }
    else
    {
        OutColor = vec4(0.0, 1.0, 1.0, 1.0);
    }
}


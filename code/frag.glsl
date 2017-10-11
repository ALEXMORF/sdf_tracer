#version 400 core

#define SHAPE_TYPE_SPHERE 0
#define SHAPE_TYPE_PLANE  1
#define SHAPE_TYPE_BOX  2

struct shape
{
    int Type;
    vec3 Color;
    
    vec3 P;
    vec3 Normal;
    vec3 Dim;
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

struct distance_info
{
    float Dist;
    vec3 Color;
};

distance_info SignedDistanceToScene(vec3 P)
{
    float MinDistance = MAX_DEPTH;
    vec3  MinDistanceColor;
    
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
        else if (Shapes[ShapeIndex].Type == SHAPE_TYPE_BOX)
        {
            DistanceToShape = length(max(abs(P - Shapes[ShapeIndex].P)-Shapes[ShapeIndex].Dim, 0.0));
        }
        
        if (DistanceToShape < MinDistance)
        {
            MinDistance = DistanceToShape;
            MinDistanceColor = Shapes[ShapeIndex].Color;
        }
    }
    
    distance_info Result;
    Result.Dist = MinDistance;
    Result.Color = MinDistanceColor;
    return Result;
}

float SDTS(vec3 P)
{
    return SignedDistanceToScene(P).Dist;
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
        float Diff = SampleDist - SignedDistanceToScene(P + Normal * SampleDist).Dist;
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
    vec3 RayColor;
    for (int I = 0; I < MAX_MARCH_STEP && Depth < MAX_DEPTH; ++I)
    {
        distance_info DistInfo = SignedDistanceToScene(CameraP + Depth * ViewRay);
        if (DistInfo.Dist < EPSILON)
        {
            RayColor = DistInfo.Color;
            RayHit = true;
            break;
        }
        Depth += DistInfo.Dist;
    }
    
    if (RayHit)
    {
        vec3 HitP = CameraP + Depth * ViewRay;
        vec3 Normal = Gradient(HitP);
        float AOFactor = GetOcclusionFactor(HitP, Normal);
        float Intensity = AOFactor * (0.3 + 0.7*max(dot(Normal, -LightDirection), 0.0));
        vec3 Color = RayColor * Intensity;
        OutColor = vec4(Color, 1.0);
    }
    else
    {
        OutColor = vec4(0.0, 1.0, 1.0, 1.0);
    }
}
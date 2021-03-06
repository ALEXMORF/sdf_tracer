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
uniform mat4 ShapeRotation;

uniform shape[50] Shapes;
uniform int ShapeCount;

in vec2 FragTexCoord;
in vec3 FragViewRay;
out vec3 OutColor;

const float EPSILON = 0.01;
const float MAX_MARCH_STEP = 200;
const float MAX_DEPTH = 50;

struct distance_info
{
    float Dist;
    vec3 Color;
};

float Square(float T)
{
    return T*T;
}

float SignedDistanceToTetrahedron(vec3 P)
{
    P.y -= 3.0;
    const int Iterations = 8;
    const float Scale = 2.0;
    
    const vec3 V1 = vec3(1, 1, 1);
    const vec3 V2 = vec3(-1, -1, 1);
    const vec3 V3 = vec3(1, -1, -1);
    const vec3 V4 = vec3(-1, 1, -1);
    vec3 C;
    int N = 0;
    float Dist, D;
    while (N < Iterations)
    {
        C = V1;
#if 1
        if (P.x + P.y < 0) P.xy = -P.yx; //fold 1
        if (P.x + P.z < 0) P.xz = -P.zx; //fold 2
        if (P.y + P.z < 0) P.yz = -P.zy; //fold 3
#else
        C = V1; Dist = distance(P, V1);
        D = distance(P, V2); if (D < Dist) { C = V2; Dist = D; }
        D = distance(P, V3); if (D < Dist) { C = V3; Dist = D; }
        D = distance(P, V4); if (D < Dist) { C = V4; Dist = D; }
#endif
        P = Scale*P - C*(Scale - 1.0);
        ++N;
    }
    
    return length(P) * pow(Scale, float(-N));
}

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
            DistanceToShape = length(max(abs(P - Shapes[ShapeIndex].P)-Shapes[ShapeIndex].Dim, 0.0))-0.1;
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
    
    float DistToTetrahedron = SignedDistanceToTetrahedron(P*inverse(mat3(ShapeRotation)));
#if 1
    if (Result.Dist > DistToTetrahedron)
    {
        Result.Dist = DistToTetrahedron;
        Result.Color = vec3(0.8, 0.8, 0.8);
    }
#endif
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
    const float AORadiusDelta = 0.2f;
    
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
    const vec3 LightDir = vec3(0.0, 0.0, 1.0);
    const vec3 SkyColor = vec3(1.0, 1.0, 1.0);
    vec3 ViewRay = normalize(FragViewRay);
    
    bool RayHit = false;
    float Depth = 0.0;
    int MarchStepCount = 0;
    vec3 RayColor;
    for (int I = 0; I < MAX_MARCH_STEP && Depth < MAX_DEPTH; ++I)
    {
        distance_info DistInfo = SignedDistanceToScene(CameraP + Depth * ViewRay);
        if (DistInfo.Dist < EPSILON)
        {
            RayColor = DistInfo.Color;
            MarchStepCount = I;
            RayHit = true;
            break;
        }
        Depth += DistInfo.Dist;
    }
    
    if (RayHit)
    {
        vec3 HitP = CameraP + Depth * ViewRay;
        
#if 0
        float Visibility = 1.0;
#else
        //compute shadow (visibility)
        float Visibility = 1.0;
        const float LightDist = 5.0;
        const float SharpShadowFactor = 4.0;
        const float DepthBias = 20*EPSILON;
        vec3 LightP = HitP - LightDirection * LightDist;
        for (float LightDepth = 0.0; LightDepth < LightDist-DepthBias;)
        {
            distance_info DistInfo = SignedDistanceToScene(LightP + LightDepth * LightDirection);
            if (DistInfo.Dist < EPSILON)
            {
                Visibility = 0.0;
                break;
            }
            Visibility = min(Visibility, SharpShadowFactor * DistInfo.Dist / (LightDist-LightDepth));
            LightDepth += DistInfo.Dist;
        }
#endif
        
        vec3 Normal = Gradient(HitP);
        float AOFactor = GetOcclusionFactor(HitP, Normal);
        float Ambient = AOFactor * 0.4;
        float Diffuse = 0.6 * Visibility * max(dot(Normal, -LightDirection), 0.0);
        vec3 HalfVector = normalize(-ViewRay + -LightDirection);
        float Specular = Visibility * pow(dot(HalfVector, Normal), 16);
        float Intensity = Ambient + Diffuse + Specular;
        vec3 Color = RayColor * Intensity;
        
        //blend with sky color to emulate fog
        float DepthPercent = (MAX_DEPTH - Depth) / MAX_DEPTH;
        OutColor = mix(Color, SkyColor, pow(1.0-DepthPercent, 2));
    }
    else
    {
        OutColor = SkyColor;
    }
    OutColor = sqrt(OutColor);
            }
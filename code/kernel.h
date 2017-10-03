//
//
// @Types
#include <stdint.h>
#define internal static
#define local_persist static
#define global_variable static
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;
typedef bool b8;
typedef i32 b32;
typedef u8 byte;
#define KB(Value) (1024LL * Value)
#define MB(Value) (1024LL * KB(Value))
#define GB(Value) (1024LL * MB(Value))
#define TB(Value) (1024LL * GB(Value))
#define ARRAY_COUNT(Array) sizeof(Array)/sizeof(Array[0])
#define ASSERT(Value) do { if (!(Value)) *(i32 *)0 = 0; } while (0)

//
//
// @Maths
#include <math.h>

#define Pi32 3.1415926f

struct v2
{
    f32 X, Y;
};

struct v2i
{
    i32 X, Y;
};

union v3
{
    struct
    {
        f32 X;
        f32 Y;
        f32 Z;
    };
    
    f32 Data[3];
};

union v4
{
    struct
    {
        f32 X, Y, Z, W;
    };
    
    f32 Data[4];
};

struct mat4
{
    f32 Data[4][4];
};

struct mat3
{
    f32 Data[3][3];
};

struct quaternion
{
    f32 X;
    f32 Y;
    f32 Z;
    f32 W;
};

inline f32
Square(f32 A)
{
    return A * A;
}

inline f32
SquareRoot(f32 Float)
{
    return (f32)sqrtf(Float);
}

inline i32
RoundF32ToI32(f32 Value)
{
    i32 Result = (i32)(roundf(Value));
    return Result;
}

inline f32
DegreeToRadian(f32 Degree)
{
    f32 Result = Degree / 180.0f * Pi32;
    return Result;
}

template <typename T>
inline T
Clamp(T Value, T Min, T Max)
{
    if (Value < Min)
    {
        return Min;
    }
    else if (Value > Max)
    {
        return Max;
    }
    else
    {
        return Value;
    }
}

//
//
// Vector

inline v2
V2(int A, int B)
{
    return {(f32)A, (f32)B};
}

inline v3
V3(f32 Float)
{
    v3 Result;
    
    Result.X = Float;
    Result.Y = Float;
    Result.Z = Float;
    
    return Result;
}

inline v3
V3(f32 A, f32 B, f32 C)
{
    return {A, B, C};
}

inline v3
V3(v4 V)
{
    v3 Result;
    
    Result.X = V.X;
    Result.Y = V.Y;
    Result.Z = V.Z;
    
    return Result;
}

inline v2
CastToV2(v2i V)
{
    v2 Result;
    Result = {(f32)V.X, (f32)V.Y};
    return Result;
}

inline void
SwapV2(v2 *A, v2 *B)
{
    v2 Temp = *A;
    *A = *B;
    *B = Temp;
}

inline void
SwapF32(f32 *A, f32 *B)
{
    f32 Temp = *A;
    *A = *B;
    *B = Temp;
}

inline v2i
operator+(v2i A, v2i B)
{
    v2i Result = {A.X + B.X, A.Y + B.Y};
    return Result;
}

inline v2i
operator-(v2i A, v2i B)
{
    v2i Result = {A.X - B.X, A.Y - B.Y};
    return Result;
}

inline v2i
operator*(int S, v2i A)
{
    v2i Result = {A.X * S, A.Y * S};
    return Result;
}

inline void
operator+=(v2i &A, v2i B)
{
    A = A + B;
}

inline void
operator-=(v2i &A, v2i B)
{
    A = A - B;
}

inline void
operator*=(v2i &A, int B)
{
    A = B * A;
}

inline v2
operator+(v2 A, v2 B)
{
    v2 Result = {A.X + B.X, A.Y + B.Y};
    return Result;
}

inline v2
operator-(v2 A, v2 B)
{
    v2 Result = {A.X - B.X, A.Y - B.Y};
    return Result;
}

inline v2
operator*(f32 S, v2 A)
{
    v2 Result = {A.X * S, A.Y * S};
    return Result;
}

inline void
operator+=(v2 &A, v2 B)
{
    A = A + B;
}

inline void
operator-=(v2 &A, v2 B)
{
    A = A - B;
}

inline void
operator*=(v2 &A, f32 B)
{
    A = B * A;
}

inline v3
ZeroV3()
{
    return {};
}

inline v3
operator+(v3 A, v3 B)
{
    v3 Result = {A.X + B.X, A.Y + B.Y, A.Z + B.Z};
    return Result;
}

inline v3
operator-(v3 A, v3 B)
{
    v3 Result = {A.X - B.X, A.Y - B.Y, A.Z - B.Z};
    return Result;
}

inline v3 
operator*(v3 A, v3 B)
{
    v3 Result = {A.X * B.X, A.Y * B.Y, A.Z * B.Z};
    return Result;
}

inline v3
operator*(f32 S, v3 A)
{
    v3 Result = {A.X * S, A.Y * S, A.Z * S};
    return Result;
}

inline v3
operator*(v3 A, f32 S)
{
    v3 Result = {A.X * S, A.Y * S, A.Z * S};
    return Result;
}

inline void
operator*=(v3 &A, v3 B)
{
    A = A * B;
}

inline void
operator+=(v3 &A, v3 B)
{
    A = A + B;
}

inline void
operator-=(v3 &A, v3 B)
{
    A = A - B;
}

inline void
operator*=(v3 &A, f32 B)
{
    A = B * A;
}

inline v4
V4(f32 X, f32 Y, f32 Z)
{
    v4 Result;
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = 1.0f;
    
    return Result;
}

inline v4
V4(v3 A)
{
    v4 Result = V4(A.X, A.Y, A.Z);
    return Result;
}

inline v4
ZeroV4()
{
    v4 Result = V4(0.0f, 0.0f, 0.0f);
    return Result;
}

inline v4
operator+(v4 A, v4 B)
{
    v4 Result = V4(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
    return Result;
}

inline v4
operator-(v4 A, v4 B)
{
    v4 Result = V4(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
    return Result;
}

inline v4
operator*(v4 A, f32 S)
{
    v4 Result = V4(A.X * S, A.Y * S, A.Z * S);
    return Result;
}

inline v4
operator/(v4 A, f32 S)
{
    v4 Result = V4(A.X / S, A.Y / S, A.Z / S);
    return Result;
}

inline void
operator+=(v4 &A, v4 B)
{
    A = A + B;
}

inline void
operator-=(v4 &A, v4 B)
{
    A = A - B;
}

inline void
operator*=(v4 &A, f32 B)
{
    A = A * B;
}

inline void
operator/=(v4 &A, f32 B)
{
    A = A / B;
}

inline v3
operator-(v3 A)
{
    return A * -1.0f;
}

inline v3
YAxis()
{
    return {0.0f, 1.0f, 0.0f};
}

inline v3
XAxis()
{
    return {1.0f, 0.0f, 0.0f};
}

inline v3
ZAxis()
{
    return {0.0f, 0.0f, 1.0f};
}

inline f32
Dot(v3 A, v3 B)
{
    f32 Result = A.X * B.X + A.Y * B.Y + A.Z * B.Z;
    return Result;
}

inline f32
Dot(v4 A, v4 B)
{
    f32 Result = A.X * B.X + A.Y * B.Y + A.Z * B.Z;
    return Result;
}

inline f32
Len(v3 A)
{
    f32 Result = sqrtf(Square(A.X) + Square(A.Y) + Square(A.Z));
    return Result;
}

inline f32
Len(v4 A)
{
    f32 Result = sqrtf(Square(A.X) + Square(A.Y) + Square(A.Z));
    return Result;
}

inline v3
Normalize(v3 V)
{
    f32 Epsilon = 0.000001f;
    
    v3 Result = V;
    
    f32 Length = Len(V);
    if (Length > Epsilon)
    {
        Result.X = V.X / Length;
        Result.Y = V.Y / Length;
        Result.Z = V.Z / Length;
    }
    
    return Result;
}

inline v4
Normalize(v4 V)
{
    f32 Epsilon = 0.000001f;
    
    v4 Result = V;
    
    f32 Length = Len(V);
    if (Length > Epsilon)
    {
        Result.X = V.X / Length;
        Result.Y = V.Y / Length;
        Result.Z = V.Z / Length;
    }
    
    return Result;
}

inline v3
Cross(v3 A, v3 B)
{
    v3 Result = {};
    
    Result.X = A.Y * B.Z - A.Z * B.Y;
    Result.Y = A.Z * B.X - A.X * B.Z;
    Result.Z = A.X * B.Y - A.Y * B.X;
    
    return Result;
}

inline v4
Cross(v4 A, v4 B)
{
    f32 X = A.Y * B.Z - A.Z * B.Y;
    f32 Y = A.Z * B.X - A.X * B.Z;
    f32 Z = A.X * B.Y - A.Y * B.X;
    
    v4 Result = V4(X, Y, Z);
    return Result;
}

//
//
// Matrix

inline mat4
operator*(mat4 A, mat4 B)
{
    mat4 Result = {};
    
    for (i32 Row = 0; Row < 4; ++Row)
    {
        for (i32 Col = 0; Col < 4; ++Col)
        {
            for (i32 I = 0; I < 4; ++I)
            {
                Result.Data[Row][Col] += A.Data[Row][I] * B.Data[I][Col];
            }
        }
    }
    
    return Result;
}

inline v4
operator*(v4 B, mat4 A)
{
    v4 Result = B;
    
    for (i32 Col = 0; Col < 4; ++Col)
    {
        f32 Sum = 0.0f;
        for (i32 I = 0; I < 4; ++I)
        {
            Sum += A.Data[I][Col] * B.Data[I];
        }
        Result.Data[Col] = Sum;
    }
    
    return Result;
}
inline v3
operator*(v3 B, mat3 A)
{
    v3 Result = B;
    
    for (i32 Col = 0; Col < 3; ++Col)
    {
        f32 Sum = 0.0f;
        for (i32 I = 0; I < 3; ++I)
        {
            Sum += A.Data[I][Col] * B.Data[I];
        }
        Result.Data[Col] = Sum;
    }
    
    return Result;
}

inline void
operator*=(v4 &A, mat4 B)
{
    A = A * B;
}

inline void
operator*=(v3 &A, mat3 B)
{
    A = A * B;
}

inline void
operator*=(mat4 &A, mat4 B)
{
    A = A * B;
}

inline mat4
Mat4Identity()
{
    mat4 Result = {};
    
    Result.Data[0][0] = 1.0f;
    Result.Data[1][1] = 1.0f;
    Result.Data[2][2] = 1.0f;
    Result.Data[3][3] = 1.0f;
    
    return Result;
}

inline mat4
Transpose(mat4 A)
{
    mat4 Result = {};
    
    for (int Y = 0; Y < 4; ++Y)
    {
        for (int X = 0; X < 4; ++X)
        {
            Result.Data[Y][X] = A.Data[X][Y];
        }
    }
    
    return Result;
}

inline mat4
Mat4Translate(f32 dX, f32 dY, f32 dZ)
{
    mat4 Result = Mat4Identity();
    
    Result.Data[3][0] = dX;
    Result.Data[3][1] = dY;
    Result.Data[3][2] = dZ;
    
    return Result;
}

inline mat4
Mat4Translate(v3 dP)
{
    return Mat4Translate(dP.X, dP.Y, dP.Z);
}

inline mat4
Mat4Scale(f32 sX, f32 sY, f32 sZ)
{
    mat4 Result = {};
    
    Result.Data[0][0] = sX;
    Result.Data[1][1] = sY;
    Result.Data[2][2] = sZ;
    Result.Data[3][3] = 1.0f;
    
    return Result;
}

inline mat4
Mat4Scale(v3 Scale)
{
    mat4 Result = {};
    
    Result.Data[0][0] = Scale.X;
    Result.Data[1][1] = Scale.Y;
    Result.Data[2][2] = Scale.Z;
    Result.Data[3][3] = 1.0f;
    
    return Result;
}

inline mat4
Mat4Scale(f32 S)
{
    return Mat4Scale(S, S, S);
}

inline mat4
Mat4RotateAroundX(f32 Radians)
{
    mat4 Result = Mat4Identity();
    
    Result.Data[1][1] = cosf(Radians);
    Result.Data[1][2] = sinf(Radians);
    Result.Data[2][1] = -sinf(Radians);
    Result.Data[2][2] = cosf(Radians);
    
    return Result;
}

inline mat4
Mat4RotateAroundY(f32 Radians)
{
    mat4 Result = Mat4Identity();
    
    Result.Data[0][0] = cosf(Radians);
    Result.Data[0][2] = -sinf(Radians);
    Result.Data[2][0] = sinf(Radians);
    Result.Data[2][2] = cosf(Radians);
    
    return Result;
}

inline mat4
Mat4RotateAroundZ(f32 Radians)
{
    mat4 Result = Mat4Identity();
    
    Result.Data[0][0] = cosf(Radians);
    Result.Data[0][1] = sinf(Radians);
    Result.Data[1][0] = -sinf(Radians);
    Result.Data[1][1] = cosf(Radians);
    
    return Result;
}

inline mat4
Mat4Rotate(v3 Euler)
{
    mat4 Result = {};
    
    Result  = Mat4RotateAroundX(Euler.X);
    Result *= Mat4RotateAroundY(Euler.Y);
    Result *= Mat4RotateAroundZ(Euler.Z);
    
    return Result;
}

inline mat4
Mat4Ortho(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
    mat4 Result = {};
    
    Result.Data[0][0] = 2.0f / (Right - Left);
    Result.Data[1][1] = 2.0f / (Top - Bottom);
    Result.Data[2][2] = 2.0f / (Far - Near);
    Result.Data[3][0] = (Left + Right) / (Left - Right);
    Result.Data[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.Data[3][2] = (Far + Near) / (Near - Far);
    Result.Data[3][3] = 1.0f;
    
    return Result;
}
inline mat4
Mat4Projection(f32 FOV, f32 AspectRatio, f32 ZNear, f32 ZFar)
{
    mat4 Result = {};
    
    f32 HFOV = FOV / 2.0f;
    
    Result.Data[0][0] = 1.0f / (tanf(DegreeToRadian(HFOV)) * AspectRatio);
    Result.Data[1][1] = 1.0f / tanf(DegreeToRadian(HFOV));
    Result.Data[2][3] = 1.0f;
    Result.Data[2][2] = (ZFar + ZNear) / (ZFar - ZNear);
    Result.Data[3][2] = -(2.0f *ZFar*ZNear) / (ZFar - ZNear);
    
    return Result;
}

inline mat4
Mat4LookAt(v3 P, v3 Target)
{
    v4 Up = V4(0.0f, 1.0f, 0.0f);
    
    v4 CamDirection = Normalize(V4(Target - P));
    v4 CamRight = Normalize(Cross(Up, CamDirection));
    v4 CamUp = Normalize(Cross(CamDirection, CamRight));
    
    mat4 Result = Mat4Identity();
    Result.Data[0][0] = CamRight.X;
    Result.Data[1][0] = CamRight.Y;
    Result.Data[2][0] = CamRight.Z;
    Result.Data[0][1] = CamUp.X;
    Result.Data[1][1] = CamUp.Y;
    Result.Data[2][1] = CamUp.Z;
    Result.Data[0][2] = CamDirection.X;
    Result.Data[1][2] = CamDirection.Y;
    Result.Data[2][2] = CamDirection.Z;
    
    Result = Mat4Translate(-P.X, -P.Y, -P.Z) * Result;
    return Result;
}

inline v3
ExtractTranslation(mat4 Matrix)
{
    v3 Translation = {};
    
    Translation.X = Matrix.Data[3][0];
    Translation.Y = Matrix.Data[3][1];
    Translation.Z = Matrix.Data[3][2];
    
    return Translation;
}

inline quaternion
Quaternion()
{
    quaternion Result = {};
    Result.W = 1.0f;
    return Result;
}

inline quaternion
Quaternion(v3 Axis, f32 Angle)
{
    quaternion Result;
    
    Axis = Normalize(Axis);
    Result.X = Axis.X * sinf(0.5f*Angle);
    Result.Y = Axis.Y * sinf(0.5f*Angle);
    Result.Z = Axis.Z * sinf(0.5f*Angle);
    Result.W = cosf(0.5f*Angle);
    
    return Result;
}

inline quaternion
Conjugate(quaternion A)
{
    quaternion Result = A;
    
    Result.X = -Result.X;
    Result.Y = -Result.Y;
    Result.Z = -Result.Z;
    
    return Result;
}

inline quaternion
operator*(quaternion A, quaternion B)
{
    quaternion Result;
    
    v4 AV = V4(A.X, A.Y, A.Z);
    v4 BV = V4(B.X, B.Y, B.Z);
    
    v4 VectorComponent = BV*A.W + AV*B.W + Cross(AV, BV);
    f32 ScalarComponent = A.W*B.W - Dot(AV, BV);
    
    Result.X = VectorComponent.X;
    Result.Y = VectorComponent.Y;
    Result.Z = VectorComponent.Z;
    Result.W = ScalarComponent;
    
    return Result;
}

inline void
operator*=(quaternion &A, quaternion B)
{
    A = A * B;
}

inline quaternion
Quaternion(v3 V)
{
    quaternion Result;
    Result.X = V.X;
    Result.Y = V.Y;
    Result.Z = V.Z;
    Result.W = 0.0f;
    
    return Result;
}

inline v3
Rotate(v3 V, quaternion Q)
{
    v3 Result;
    
    quaternion VectorAsQuaternion = Quaternion(V);
    quaternion ResultInQuaternion = Q * VectorAsQuaternion;
    ResultInQuaternion *= Conjugate(Q);
    
    Result.X = ResultInQuaternion.X;
    Result.Y = ResultInQuaternion.Y;
    Result.Z = ResultInQuaternion.Z;
    
    return Result;
}

inline v4 
Rotate(v4 Vector, quaternion Quaternion)
{
    v4 Result;
    
    quaternion VectorAsQuaternion;
    VectorAsQuaternion.X = Vector.X;
    VectorAsQuaternion.Y = Vector.Y;
    VectorAsQuaternion.Z = Vector.Z;
    VectorAsQuaternion.W = 0.0f;
    
    quaternion ResultInQuaternion;
    ResultInQuaternion = Quaternion * VectorAsQuaternion;
    ResultInQuaternion *= Conjugate(Quaternion);
    
    Result.X = ResultInQuaternion.X;
    Result.Y = ResultInQuaternion.Y;
    Result.Z = ResultInQuaternion.Z;
    Result.W = Vector.W;
    return Result;
}
inline mat4
QuaternionToMat4(quaternion Q)
{
    mat4 Result = Mat4Identity();
    
    f32 SquareX = Square(Q.X);
    f32 SquareY = Square(Q.Y);
    f32 SquareZ = Square(Q.Z);
    f32 XY = Q.X * Q.Y;
    f32 XZ = Q.X * Q.Z;
    f32 XW = Q.X * Q.W;
    f32 YW = Q.Y * Q.W;
    f32 YZ = Q.Y * Q.Z;
    f32 ZW = Q.Z * Q.W;
    
    Result.Data[0][0] = 1.0f - 2.0f*SquareY - 2.0f*SquareZ;
    Result.Data[0][1] = 2.0f*XY + 2.0f*ZW;
    Result.Data[0][2] = 2.0f*XZ - 2.0f*YW;
    Result.Data[1][0] = 2.0f*XY - 2.0f*ZW;
    Result.Data[1][1] = 1.0f - 2*SquareX - 2.0f*SquareZ;
    Result.Data[1][2] = 2.0f*YZ + 2.0f*XW;
    Result.Data[2][0] = 2.0f*XZ + 2.0f*YW;
    Result.Data[2][1] = 2.0f*YZ - 2.0f*XW;
    Result.Data[2][2] = 1.0f - 2.0f*SquareX - 2.0f*SquareY;
    
    return Result;
}

inline quaternion
Normalize(quaternion Q)
{
    f32 Length = sqrtf(Square(Q.X) + Square(Q.Y) + Square(Q.Z) + Square(Q.W));
    
    Q.X /= Length;
    Q.Y /= Length;
    Q.Z /= Length;
    Q.W /= Length;
    
    return Q;
}

inline quaternion
RotationToQuaternion(mat4 Mat)
{
    quaternion Quat = {};
    
    float  tr, s, q[4];
    int    i, j, k;
    int nxt[3] = {1, 2, 0};
    tr = Mat.Data[0][0] + Mat.Data[1][1] + Mat.Data[2][2];
    // check the diagonal
    if (tr > 0.0) {
        s = (f32)sqrtf(tr + 1.0f);
        Quat.W = s / 2.0f;
        s = 0.5f / s;
        Quat.X = (Mat.Data[1][2] - Mat.Data[2][1]) * s;
        Quat.Y = (Mat.Data[2][0] - Mat.Data[0][2]) * s;
        Quat.Z = (Mat.Data[0][1] - Mat.Data[1][0]) * s;
    } else {		
        // diagonal is negative
        i = 0;
        if (Mat.Data[1][1] > Mat.Data[0][0]) i = 1;
        if (Mat.Data[2][2] > Mat.Data[i][i]) i = 2;
        j = nxt[i];
        k = nxt[j];
        s = (f32)sqrtf((Mat.Data[i][i] - (Mat.Data[j][j] + Mat.Data[k][k])) + 1.0f);
        q[i] = s * 0.5f;
        if (s != 0.0) s = 0.5f / s;
        q[3] = (Mat.Data[j][k] - Mat.Data[k][j]) * s;
        q[j] = (Mat.Data[i][j] + Mat.Data[j][i]) * s;
        q[k] = (Mat.Data[i][k] + Mat.Data[k][i]) * s;
        Quat.X = q[0];
        Quat.Y = q[1];
        Quat.Z = q[2];
        Quat.W = q[3];
    }
    
    return Quat;
}

inline quaternion
operator*(f32 S, quaternion A)
{
    quaternion Result;
    
    Result.X = A.X * S;
    Result.Y = A.Y * S;
    Result.Z = A.Z * S;
    Result.W = A.W * S;
    
    return Result;
}

inline quaternion
operator+(quaternion A, quaternion B)
{
    quaternion Result;
    
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    Result.W = A.W + B.W;
    
    return Result;
}

inline f32
Dot(quaternion A, quaternion B)
{
    return A.X * B.X + A.Y * B.Y + A.Z * B.Z + A.W * B.W;
}

inline quaternion
Lerp(quaternion A, quaternion B, f32 T)
{
    ASSERT(T >= 0.0f && T <= 1.0f);
    
    quaternion Result;
    
    Result.X = A.X * (1.0f - T) + B.X * T;
    Result.Y = A.Y * (1.0f - T) + B.Y * T;
    Result.Z = A.Z * (1.0f - T) + B.Z * T;
    Result.W = A.W * (1.0f - T) + B.W * T;
    
    Result = Normalize(Result);
    
    return Result;
}

inline quaternion
ShortestLerp(quaternion A, quaternion B, f32 T)
{
    if (Dot(A, B) <= 0.0f)
    {
        A = -1.0f * A;
    }
    
    return Lerp(A, B, T);
}

inline quaternion
Slerp(quaternion A, quaternion B, f32 T)
{
    ASSERT(T >= 0.0f && T <= 1.0f);
    
    f32 Theta = acosf(Dot(A, B));
    f32 STheta = sinf(Theta);
    
    f32 FirstWeight = sinf((1.0f - T) * Theta) / STheta;
    f32 SecondWeight = sinf(T * Theta) / STheta;
    
    quaternion Result;
    Result = FirstWeight * A + SecondWeight * B;
    Result = Normalize(Result);
    
    return Result;
}

inline f32
Lerp(f32 A, f32 B, f32 T)
{
    ASSERT(T >= 0.0f && T <= 1.0f);
    
    return A * (1.0f - T) + B * T;
}

inline v3
Lerp(v3 A, v3 B, f32 T)
{
    ASSERT(T >= 0.0f && T <= 1.0f);
    
    v3 Result = A * (1.0f - T) + B * T;
    
    return Result;
}

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
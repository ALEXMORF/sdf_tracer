#include "renderer.h"

struct loaded_file
{
    char *Data;
    u64 Size;
};

struct input
{
    b32 Left;
    b32 Right;
    b32 Up;
    b32 Down;
};

struct game_state
{
    renderer Renderer;
    
    v3 LightDirection;
    v3 CameraP;
    
    b32 IsInitialized;
};

typedef char *read_entire_file(char *Filename, u64 *MemorySize);
global_variable read_entire_file *ReadEntireFile;
typedef void free_file(void *FileData);
global_variable free_file *FreeFile;
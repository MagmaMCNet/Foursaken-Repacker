#pragma once
#include "filestream.h"
#include "vector.h"
#include <stdbool.h>
#include <raylib.h>

typedef struct
{
    float x, z, y;
} UMBVector3;

typedef struct
{
    int number;

    bool usePreviousIndexData, usePreviousTextureData;

    int numFaces;

    unsigned short* indices;

    int numTextures;

    Vec2* textures;

    int numColors;

    OpaqueColor32* colors;

    int numVertices;

    UMBVector3* vertices, *normals;
} UMBFrame;

typedef struct
{
    int materialIndex, numKeyFrames, numAnimationFrames;

    UMBFrame* frames;
} UMBObject;

typedef struct
{
    char* name, *texturePath, *textureBase;

    bool hasTexture;

    OpaqueColor ambient, diffuse, specular;

    float glossiness;
} UMBMaterial;

typedef struct
{
    int numMaterials;

    UMBMaterial* materials;

    int numObjects;

    UMBObject* objects;
} UMB;

Vec3 umb_vector3_to_vector3(UMBVector3 vector);

UMB* umb_from_stream(Stream* stream);

void umb_delete(UMB* umb);

char* ptr_from_umb(UMB* umb);

Mesh umb_object_to_mesh(UMBObject* object);

Mesh* umb_to_meshes(UMB* umb);
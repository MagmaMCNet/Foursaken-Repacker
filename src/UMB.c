#include <UMB.h>
#include <malloc.h>

UMB* umb_from_stream(Stream* stream)
{
    UMB* umb = (UMB*)malloc(sizeof(UMB));

    umb->numMaterials = stream_int(stream);
    umb->materials = (UMBMaterial*)malloc(sizeof(UMBMaterial) * umb->numMaterials);

    for (int i = 0; i < umb->numMaterials; i++)
    {
        umb->materials[i].name = stream_string(stream);
        umb->materials[i].texturePath = stream_string(stream);
        umb->materials[i].textureBase = stream_string(stream);

        Color* ambient = (Color*)stream_data(stream, sizeof(Color));
        umb->materials[i].ambient = *ambient;

        free(ambient);

        Color* diffuse = (Color*)stream_data(stream, sizeof(Color));
        umb->materials[i].diffuse = *diffuse;

        free(diffuse);

        Color* specular = (Color*)stream_data(stream, sizeof(Color));
        umb->materials[i].specular = *specular;

        free(specular);

        umb->materials[i].glossiness = stream_float(stream);
    }

    umb->numObjects = stream_int(stream);
    umb->objects = (UMBObject*)malloc(sizeof(UMBObject) * umb->numObjects);

    for (int i = 0; i < umb->numObjects; i++)
    {
        UMBObject* object = &(umb->objects[i]);

        object->materialIndex = stream_int(stream);
        object->numKeyFrames = stream_int(stream);
        object->numAnimationFrames = stream_int(stream);

        object->frames = (UMBFrame*)malloc(sizeof(UMBFrame) * object->numKeyFrames);

        for (int j = 0; j < object->numKeyFrames; j++)
        {
            UMBFrame* frame = &(object->frames[j]);

            frame->number = stream_int(stream);

            frame->usePreviousIndexData = stream_short_bool(stream);
            frame->usePreviousTextureData = stream_short_bool(stream);

            frame->numFaces = stream_int(stream);

            if (frame->numFaces > 0 && !frame->usePreviousIndexData)
            {
                frame->indices = (unsigned short*)stream_data(stream, sizeof(unsigned short) * frame->numFaces * 3);
            }

            frame->numTextures = stream_int(stream);

            if (frame->numTextures > 0 && !frame->usePreviousTextureData)
            {
                frame->textures = (Vector2*)stream_data(stream, sizeof(Vector2) * frame->numTextures);
            }

            frame->numColors = stream_int(stream);

            if (frame->numColors > 0 && !frame->usePreviousTextureData)
            {
                frame->colors = (Color32*)stream_data(stream, sizeof(Color32) * frame->numColors);
            }

            frame->numVertices = stream_int(stream);

            if (frame->numVertices > 0)
            {
                frame->vertices = (UMBVector3*)malloc(sizeof(UMBVector3) * frame->numVertices);
                frame->normals = (UMBVector3*)malloc(sizeof(UMBVector3) * frame->numVertices);

                for (int k = 0; k < frame->numVertices; k++)
                {
                    UMBVector3* vertex = (UMBVector3*)stream_data(stream, sizeof(UMBVector3));
                    UMBVector3* normal = (UMBVector3*)stream_data(stream, sizeof(UMBVector3));

                    frame->vertices[k] = *vertex;
                    frame->normals[k] = *normal;

                    free(vertex);
                    free(normal);
                }
            }
        }
    }

    printf("finished reading\n");

    return umb;
}

void umb_frame_delete(UMBFrame frame)
{
    if (frame.numFaces > 0)
    {
        free(frame.indices);
    }

    if (frame.numTextures > 0)
    {
        free(frame.textures);
    }

    if (frame.numColors > 0)
    {
        free(frame.colors);
    }

    if (frame.numVertices > 0)
    {
        free(frame.vertices);
        free(frame.normals);
    }
}

void umb_object_delete(UMBObject object)
{
    if (object.numKeyFrames > 0)
    {
        for (int i = 0; i < object.numKeyFrames; i++)
        {
            umb_frame_delete(object.frames[i]);
        }

        free(object.frames);
    }
}

void umb_material_delete(UMBMaterial material)
{
    if (material.name != NULL)
    {
        free(material.name);
    }

    if (material.texturePath != NULL)
    {
        free(material.texturePath);
    }

    if (material.textureBase != NULL)
    {
        free(material.textureBase);
    }
}

void umb_delete(UMB* umb)
{
    if (umb != NULL)
    {
        if (umb->numMaterials > 0)
        {
            for (int i = 0; i < umb->numMaterials; i++)
            {
                umb_material_delete(umb->materials[i]);
            }

            free(umb->materials);
        }

        if (umb->numObjects > 0)
        {
            for (int i = 0; i < umb->numObjects; i++)
            {
                umb_object_delete(umb->objects[i]);
            }

            free(umb->objects);
        }

        free(umb);
    }
}

Vector3 umb_vector3_to_vector3(UMBVector3 vector)
{
    Vector3 vec3;

    vec3.x = vector.x;
    vec3.y = vector.y;
    vec3.z = -vector.z;
    
    return vec3;
}
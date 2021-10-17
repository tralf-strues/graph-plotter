//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file mesh.h
//! @date 2021-10-06
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef MESH_H
#define MESH_H

#include "sml/containers/array.h"
#include "sml/math/vec3.h"

struct Face
{
    Vec3<int32_t> idxVertices;
    Vec3<int32_t> idxUV;
    Vec3<int32_t> idxNormals;
};

struct Mesh
{
    Array<Face>        faces;
    
    Array<Vec3<float>> vertices;
    Array<Vec3<float>> uv;
    Array<Vec3<float>> normals;
};

#endif // MESH_H
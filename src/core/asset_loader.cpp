#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <cassert>
#include <string>

#include "core/asset_loader.h"


core::AssetLoader::AssetLoader()
{
}

std::unique_ptr<core::Mesh> core::AssetLoader::loadMesh(const char* modelDir, const char* modelFilename)
{
    std::string modelPath{modelDir};
    modelPath += modelFilename;

    Assimp::Importer importer;

    std::cout << "Loading " << modelPath << std::endl;

    //Load meshes triangulated and with face normals (for flat shading)
    const aiScene* scene = importer.ReadFile(modelPath,
        aiProcess_Triangulate |
        aiProcess_GenNormals);

    if (!scene) {
        std::cerr << "ERROR::ASSET_LOADER::MESH: Couldn't load mesh " << modelPath << std::endl;
    }
    
    if (scene->mNumMeshes != 1) {
        std::cerr << "ERROR::ASSET_LOADER::MESH: Loader expects to only load 1 mesh, user supplied " << scene->mNumMeshes << std::endl;
    }
    
    return buildMesh(scene->mMeshes[0]);
}






std::unique_ptr<core::Mesh> core::AssetLoader::buildMesh(aiMesh * mesh){

    std::vector<VertexData> vertices;
    std::vector<GLuint> indices;


    //Load vertices
    for(int i = 0;i < mesh->mNumVertices;i++){
        VertexData v{
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z,
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        };
        vertices.push_back(v);
    }


    //Load indices
    for(int i = 0;i < mesh->mNumFaces;i++){
        aiFace face = mesh->mFaces[i];
        for(int j = 0;j < face.mNumIndices;j++){
            indices.push_back(face.mIndices[j]);
        }
    }
 

    return std::make_unique<Mesh>( vertices, indices ,false);
}


#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <memory>

#include <assimp/scene.h>

#include "core/mesh.h"

namespace core {

    class AssetLoader {
    public:
        AssetLoader();
        std::unique_ptr<Mesh> loadMesh(const char* modelDir, const char* modelFilename);
    private:
        std::unique_ptr<Mesh> buildMesh(aiMesh* mesh);

    };

}

#endif
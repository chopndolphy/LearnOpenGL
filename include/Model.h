#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model {
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;
    
    Model(const char* path, bool gamma = false) : gammaCorrection(gamma) {
        loadModel(path);
    }
    void Draw(Shader &shader) {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].Draw(shader);
        }
    }
private:

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    static unsigned int TextureFromFile(const char *path, const std::string &directory);
};
#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4
struct DrawElementsIndirectCommand {
    unsigned int count;
    unsigned int instanceCount;
    unsigned int firstIndex;
    int baseVertex;
    unsigned int baseInstance;
};

struct Vertex {
glm::vec3 Position;
glm::vec3 Normal;
glm::vec2 TexCoord;
glm::vec3 Tangent;
glm::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};
struct Texture {
    std::string type;
    std::string path;
    GLuint64 handle;
    GLuint id;
};
class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    GLuint vertexDataBuffer;
    GLuint indexDataBuffer;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);
    void Draw(std::vector<DrawElementsIndirectCommand> &cmdBuffer);
private:
    unsigned int VBO;
    unsigned int EBO;

    void setupMesh();
};

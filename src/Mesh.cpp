#include "Mesh.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <string>
#include <vector>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}
void Mesh::Draw(Shader &shader) {
    //unsigned int diffuseNr  = 1;
    //unsigned int specularNr = 1;
    //unsigned int normalNr   = 1;
    //unsigned int heightNr   = 1;
    //for (unsigned int i = 0; i < textures.size(); i++) {
        //glActiveTexture(GL_TEXTURE0 + i);
        //std::string number;
        //std::string name = textures[i].type;
        //if (name == "texture_diffuse") {
            //number = std::to_string(diffuseNr++);
        //} else if (name == "texture_specular") {
            //number = std::to_string(specularNr++);
        //} else if (name == "texture_normal") {
            //number = std::to_string(normalNr++);
        //} else if (name == "texture_height") {
            //number = std::to_string(heightNr++);
        //}
        //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        //glBindTexture(GL_TEXTURE_2D, textures[i].id);
    //}
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexDataBuffer);

    //glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

    //glActiveTexture(GL_TEXTURE0);
}
void Mesh::Draw(std::vector<DrawElementsIndirectCommand> &cmdBuffer) {
    //unsigned int diffuseNr  = 1;
    //unsigned int specularNr = 1;
    //unsigned int normalNr   = 1;
    //unsigned int heightNr   = 1;
    //for (unsigned int i = 0; i < textures.size(); i++) {
        //glActiveTexture(GL_TEXTURE0 + i);
        //std::string number;
        //std::string name = textures[i].type;
        //if (name == "texture_diffuse") {
            //number = std::to_string(diffuseNr++);
        //} else if (name == "texture_specular") {
            //number = std::to_string(specularNr++);
        //} else if (name == "texture_normal") {
            //number = std::to_string(normalNr++);
        //} else if (name == "texture_height") {
            //number = std::to_string(heightNr++);
        //}
        //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        //glBindTexture(GL_TEXTURE_2D, textures[i].id);
    //}
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertexDataBuffer);
    glBind

    //glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

    //glActiveTexture(GL_TEXTURE0);
}
void Mesh::setupMesh() {
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    glCreateBuffers(1, &vertexDataBuffer);
    glCreateBuffers(1, &indexDataBuffer);

    //glBindVertexArray(VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glNamedBufferStorage(
            vertexDataBuffer,
            sizeof(Vertex) * vertices.size(),
            (const void*)vertices.data(),
            GL_DYNAMIC_STORAGE_BIT);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glNamedBufferStorage(
            indexDataBuffer, 
            sizeof(unsigned int) * indices.size(), 
            (const void*)indices.data(),
            GL_DYNAMIC_STORAGE_BIT);


    // vertex positions
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    //glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
    // vertex tangent
    //glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    //glEnableVertexAttribArray(4);
    //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        // ids
        //glEnableVertexAttribArray(5);
        //glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
        // weights
        //glEnableVertexAttribArray(6);
        //glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

    //glBindVertexArray(0);
}

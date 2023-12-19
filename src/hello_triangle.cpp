#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "stb_image.h"
#include "SimpleGui.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow*, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = (float)SCR_WIDTH / 2.0f;
float lastY = (float)SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;



int main() {
    // glfw: intitialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloTriangle", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    // config global opengl state
    glEnable(GL_DEPTH_TEST);
    // Shader config
    std::filesystem::path shaderAbsPath = std::filesystem::absolute("./shaders/");

    Shader lightCubeShader((shaderAbsPath / "light_cube.vert").string().c_str(), (shaderAbsPath / "light_cube.frag").string().c_str());
    Shader lightingShader((shaderAbsPath / "lighting.vert").string().c_str(), (shaderAbsPath / "lighting.frag").string().c_str());
    Shader planeShader((shaderAbsPath / "plane.vert").string().c_str(), (shaderAbsPath / "plane.frag").string().c_str());

    // load models
    std::filesystem::path backpackAbsPath = std::filesystem::absolute("./models/backpack/");
    std::filesystem::path cubeAbsPath = std::filesystem::absolute("./models/cube/");
    std::filesystem::path chessboardAbsPath = std::filesystem::absolute("./models/chess/");

    Model ourModel((backpackAbsPath / "backpack.obj").string().c_str());
    Model cubeModel((cubeAbsPath / "cube.obj").string().c_str());
    Model chessboardModel((chessboardAbsPath / "chess.obj").string().c_str());

    float lightColor[4] = {0.8f, 0.5f, 0.0f, 1.0f};
    float skyColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};

    float planeX = 20.0f;
    float planeZ = 20.0f;
    float planeVertices[] = {
        -(planeX / 2.0f), 0.0f, -(planeZ / 2.0f),   0.0f, 0.0f, 
         (planeX / 2.0f), 0.0f, -(planeZ / 2.0f),   1.0f, 0.0f,
        -(planeX / 2.0f), 0.0f,  (planeZ / 2.0f),   0.0f, 1.0f,
         (planeX / 2.0f), 0.0f,  (planeZ / 2.0f),   1.0f, 1.0f
    };
    unsigned int planeIndices[] = {
        0, 1, 2,
        1, 2, 3
    };
    unsigned int planeVBO, planeVAO, planeEBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeEBO);

    glBindVertexArray(planeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int planeTexture;
    // texture 1
    // ---------
    glGenTextures(1, &planeTexture);
    glBindTexture(GL_TEXTURE_2D, planeTexture); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(std::filesystem::absolute("./textures/grass.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        SimpleGui::renderWindow(lightColor, skyColor);

        // input
        processInput(window);

        // render
        glClearColor(skyColor[0], skyColor[1], skyColor[2], skyColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, planeTexture);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        planeShader.activate_shader();
        planeShader.setMat4("projection", projection);
        planeShader.setMat4("view", view);
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
        planeShader.setMat4("model", model);
        planeShader.setInt("planeTexture", 0);
        planeShader.setVec3("viewPos", camera.Position);
        planeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        planeShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        planeShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);

        planeShader.setVec3("pointLight.position", glm::vec3(0.7f, 0.2f, 3.0f));
        planeShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
        planeShader.setVec3("pointLight.diffuse", lightColor[0], lightColor[1], lightColor[2]);
        planeShader.setFloat("pointLight.constant", 1.0f);
        planeShader.setFloat("pointLight.linear", 0.09f);
        planeShader.setFloat("pointLight.quadratic", 0.032f);
        glBindVertexArray(planeVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);

        lightingShader.activate_shader();

        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("shininess", 10.0f);

        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

        lightingShader.setVec3("pointLight.position", glm::vec3(0.7f, 0.2f, 3.0f));
        lightingShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLight.diffuse", lightColor[0], lightColor[1], lightColor[2]);
        lightingShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLight.constant", 1.0f);
        lightingShader.setFloat("pointLight.linear", 0.09f);
        lightingShader.setFloat("pointLight.quadratic", 0.032f);

        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        lightingShader.setMat4("model", model);
        ourModel.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        lightingShader.setMat4("model", model);
        chessboardModel.Draw(lightingShader);

        lightCubeShader.activate_shader();
        lightCubeShader.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.7f, 0.2f, 3.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        lightCubeShader.setMat4("model", model);
        cubeModel.Draw(lightCubeShader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // de-allocate resources
    lightingShader.delete_shader();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate
    glfwTerminate();
    return 0;
}
// glfw: process all input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        camera.ProcessKeyboard(LOOKUP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        camera.ProcessKeyboard(LOOKDOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        camera.ProcessKeyboard(LOOKLEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)  
        camera.ProcessKeyboard(LOOKRIGHT, deltaTime);
}
// glfw: window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
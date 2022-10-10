#include "EarthGL.hpp"
#include "Primitives.hpp"
#include "Graphics/Shader.hpp"
#include "Camera.hpp"
#include "MouseInput.hpp"

int screenWidth = 1366, screenHeight = 768;

void Resize(GLFWwindow *window, int width, int height) {
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, screenWidth, screenHeight);
}

void ShowGLVersion() {
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

void UpdateDeltaTime() {
    auto currentFrame = (float) glfwGetTime();
    deltaTime = currentFrame - lastFrameTime;
    lastFrameTime = currentFrame;
}

void UpdateCamera(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::Get().Move(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::Get().Move(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::Get().Move(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::Get().Move(RIGHT, deltaTime);

    Camera::Get().Update(deltaTime);
}

int main() {
    assert(glfwInit() == GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "EarhGL", nullptr, nullptr);
    assert(window);

    MouseInput::Init(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowPos(window, 800, 200);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, Resize);

    assert(glewInit() == GLEW_OK);
    ShowGLVersion();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
//    uint quadVAO = Primitives::LoadGeometry();

    uint sphereNumVertices, sphereNumIndices;
    uint sphereVAO = Primitives::LoadSphere(sphereNumVertices, sphereNumIndices);

    Shader shader("Vertex.vert", "Fragment.frag");
    Texture texture("earth.jpg", GL_RGB);

    Camera::Init(screenWidth, screenHeight, vec3{0, 0, -2});

    glClearColor(0, 0.3, 0.3, 1);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        texture.ActivateAndBind(GL_TEXTURE0);
        shader.SetTexture("uTextureSampler", texture);

        mat4 model = rotate(identity<mat4>(), radians(90.0f), {1, 0, 0});
        mat4 view = Camera::Get().GetViewMatrix();
        mat4 projection = Camera::Get().GetProjectionMatrix();

        shader.SetMat4("model", model);
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);

        //glBindVertexArray(quadVAO);
        //glDrawArrays(GL_TRIANGLES, 0, Primitives::QuadVertices->size());
        //glDrawElements(GL_TRIANGLES, Primitives::QuadIndices.size() * 3, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphereNumIndices * 3, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwPollEvents();
        glfwSwapBuffers(window);

        UpdateCamera(window);
        UpdateDeltaTime();
    }

//    glDeleteVertexArrays(1, &quadVAO);
    glDeleteVertexArrays(1, &sphereVAO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
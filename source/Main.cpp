#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <Texture.hpp>
#include <memory>

#include "Shader.hpp"
#include "Mesh.hpp"
#include "RigidBody.hpp"
#include "Scene.hpp"
#include "CollisionDetection.hpp"
#include "Camera.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GUI_Parameters.hpp"
#include "Helpers.hpp"

using namespace gra;

GUI_Parameters guiParameters;

unsigned int WIDTH = 800;
unsigned int HEIGHT = 600;
constexpr unsigned int kTargetFps = 60;

glm::vec3 Position = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

float speed = 0.1f;
float sensitivity = 8.0f;

Scene scene;
gra::CollisionDetector detector;
Shader* shader;

bool firstClick = true;

Camera mainCamera(glm::vec3(0.0f, 3.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), WIDTH,
                  HEIGHT, 300.0f, 0.1f, 60.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);
void window_size_callback(GLFWwindow* window, int width, int height);
void processCamera(GLFWwindow *window, Camera* camera);

void setColor(const std::shared_ptr<Mesh>& mesh, v3 col)
{
    mesh->specularTexture = new Texture(glm::vec4(col.x, col.y, col.z, 1.0));
    mesh->updateBuffers();
}

void guiSetup(GLFWwindow* window, ImGuiIO& io) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Controls");
    ImGui::SliderFloat("Complaince", &guiParameters.complaince, 0.0f, 1.0f); 
    ImGui::SliderFloat("Restitution", &guiParameters.restitution, 0.0f, 1.0f); 

    if (ImGui::Button("Freeze All Bodies")) {
        for (auto &cur_mesh : scene.meshes) {
            cur_mesh->v = v3(0.0);
        }
    }
    if (ImGui::CollapsingHeader("Camera Settings")) {
        ImGui::SliderFloat("Sensitivity", &mainCamera.rotationSensitivity, 10.0f, 200.0f);
    }
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
    ImGui::Render();
}

void DrawScene()
{
	/*constexpr auto block_size = 1.0;
    auto color_dark = glm::vec4(0.5, 0.5, 0.5, 1.0);*/

	auto color_white = glm::vec4(1, 1, 1, 1.0);
    auto scene_obj = helpers::CreateMeshHelper("scene.obj", shader);
    scene_obj->x = glm::vec3(0, -0.1, 0);
    scene_obj->setColor(color_white);
    scene_obj->isStatic = true;
    scene.addMesh(scene_obj);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Construct the window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D ENGINE", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create the GLFW window\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);;
    // glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    // Handle view port dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        HEIGHT = height;
        WIDTH = width;
    });
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    shader = helpers::CreateShaderHelper("newshader");

    DrawScene();

    /*std::shared_ptr<RigidBody> obj = std::make_shared<RigidBody>((kModelFolder + "cube.obj").c_str(), *shader);
    obj->x = glm::vec3(0.0,0.0,0.0);
    obj->setColor(glm::vec4(0.1, 0.3, 0.6, 1.0));

     std::shared_ptr<RigidBody> obj2 = std::make_shared<RigidBody>((kModelFolder + "sphere.obj").c_str(), *shader);
     setColor(obj2, v3(0.0, 1.0, 0.5)); 
     obj2->x = glm::vec3(10.0,10.0,10.0);
     obj2->isStatic = true;

    scene.addMesh(obj);
	scene.addMesh(obj2);*/
    detector.addScene(scene);

    glEnable(GL_DEPTH_TEST);
    // This is the render loop
    double last_frame_time = 0.0f;

    while (!glfwWindowShouldClose(window)) 
    {
        guiSetup(window, io);

        const auto current_time = glfwGetTime();
        const auto delta = current_time - last_frame_time;
        if (delta >= 1.0 / kTargetFps) 
        {
            last_frame_time = current_time;
            // Clear the screen
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mainCamera.proccessWindow(window);
            mainCamera.setShaderValues(shader);

            
            scene.simulatePhysics(1.0 / (kTargetFps) );
            scene.drawScene();

			for (int i = 0; i < scene.meshes.size(); i++)
			{
				scene.meshes[i]->applyForce(glm::vec3(0.0, -9.8, 0.0));
			}
            detector.detectCollisions();

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    shader->terminate();
    glfwTerminate();

    return 0;
}

void processCamera(GLFWwindow *window, Camera* camera) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double newMouseX, newMouseY;
        glfwGetCursorPos(window, &newMouseX, &newMouseY);

        if (!firstClick) {
            firstClick = true;
            float normalMouseX = (newMouseX - camera->height/2) / camera->height;
            float normalMouseY = (newMouseY - camera->width/2) / camera->width;
            camera->curScreenX = normalMouseX;
            camera->curScreenY = normalMouseY;
        }
        camera->rotAroundCenter(newMouseX, newMouseY);

    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        if (firstClick) {
            firstClick = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera->worldPosition.y += 0.01f;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // std::cout << xpos << std::endl;
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    mainCamera.width = width;
    mainCamera.height = height;
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Position = (glm::length(Position) + static_cast<float>(-yoffset) * 0.4f) * glm::normalize(Position);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        std::shared_ptr<RigidBody> obj2 = helpers::CreateMeshHelper("cube.obj", shader);
        obj2->x = mainCamera.worldPosition + glm::normalize(mainCamera.viewDirection);
        obj2->v = glm::normalize(mainCamera.viewDirection)*8.0f;
        setColor(obj2, v3(0.1, 0.8, 0.9)); 
        scene.addMesh(obj2);
        detector.addScene(scene);
    }
}
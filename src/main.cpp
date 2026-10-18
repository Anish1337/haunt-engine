#define GLFW_INCLUDE_VULKAN
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_raii.hpp>

class HauntApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanUp();
    }

private:
    GLFWwindow* window = nullptr;

    void initWindow() {

        // check glfw
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        // dont use opengl
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // disable window resize
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // check vulkan loader
        if (!glfwVulkanSupported()) {
            glfwTerminate();
            throw std::runtime_error("Vulkan is not supported");
        }

        // describe window
        window = glfwCreateWindow(800, 600, "Haunt Engine", nullptr, nullptr);

        // window check
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create window");
        }

        // wayland debugging
        std::cout << "GLFW window created successfully!\n";
        std::cout << "Vulkan supported: " << glfwVulkanSupported() << '\n';
        std::cout << "GLFW platform: "
                  << (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND ? "Wayland"
                      : glfwGetPlatform() == GLFW_PLATFORM_X11   ? "X11"
                                                                 : "Other")
                  << '\n';
    }
    // something here later
    void initVulkan() {};

    void mainLoop() {
        // event loop
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanUp() {
        // close window
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    try {
        HauntApplication app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

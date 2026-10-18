#define GLFW_INCLUDE_VULKAN
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
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
    // hold the handle to instance + raii
    vk::raii::Context context;
    vk::raii::Instance instance = nullptr; // destroyed first!

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
    // init vulkan library
    void initVulkan() { createInstance(); }

    void createInstance() {
        // struct with app info
        constexpr vk::ApplicationInfo appInfo{
            .pApplicationName = "Haunt Engine",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Haunt Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14};

        // extension to interact with window
        // Get the required instance extensions from GLFW.
        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions =
            glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // Check if the required GLFW extensions are supported by the Vulkan
        // implementation.
        if (!glfwExtensions) {
            throw std::runtime_error(
                "Failed to retrieve required GLFW extensions");
        }
        auto extensionProperties =
            context.enumerateInstanceExtensionProperties();
        for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
            if (std::ranges::none_of(
                    extensionProperties, [glfwExtension = glfwExtensions[i]](
                                             auto const& extensionProperty) {
                        return std::strcmp(extensionProperty.extensionName,
                                           glfwExtension) == 0;
                    })) {
                throw std::runtime_error(
                    "Required GLFW extension not supported: " +
                    std::string(glfwExtensions[i]));
            }
        }

        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = glfwExtensionCount,
            .ppEnabledExtensionNames = glfwExtensions};
        // finally crate instance
        instance = vk::raii::Instance(context, createInfo);
        // wayland debug
        std::cout << "Vulkan instance created successfully!\n";
    }

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

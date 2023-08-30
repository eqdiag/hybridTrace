#include <iostream>

#include "core/window.h"

core::Window::Window(
    App& app,
    int width,
    int height,
    const char* title,
    void (*framebuffer_size_callback)(GLFWwindow*, int, int),
    void (*cursor_position_callback)(GLFWwindow*, double, double),
    void (*mouse_button_callback)(GLFWwindow*, int, int, int),
    void (*key_callback)(GLFWwindow*, int, int, int, int),
    void (*scroll_callback)(GLFWwindow*, double, double)
)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    mRawWindow = glfwCreateWindow(width,height, title, nullptr, nullptr); // Windowed    

    app.setDimensions(width, height);

    if (!mRawWindow) {
        std::cerr << "Error: Failed to create window!\n";
        glfwTerminate();
    }

    glfwMakeContextCurrent(mRawWindow);

    //Load in opengl functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    //Set user data accessible for callbacks
    glfwSetWindowUserPointer(mRawWindow, static_cast<void*>(&app));


    glfwSetFramebufferSizeCallback(mRawWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(mRawWindow, cursor_position_callback);
    glfwSetMouseButtonCallback(mRawWindow, mouse_button_callback);
    glfwSetKeyCallback(mRawWindow, key_callback);
    glfwSetScrollCallback(mRawWindow, scroll_callback);

}

core::Window::~Window()
{
    glfwDestroyWindow(mRawWindow);
    glfwTerminate();
}

const char* core::Window::getGlslVersion()
{
    return mGlslVersion;
}

GLFWwindow* core::Window::getRawWindow()
{
    return mRawWindow;
}

void core::Window::pollEvents()
{
    glfwPollEvents();
}

void core::Window::update(App& app)
{
}

void core::Window::swapBuffers()
{
    glfwSwapBuffers(mRawWindow);
}

bool core::Window::closed() const
{
    return glfwWindowShouldClose(mRawWindow);
}


void core::Window::close()
{
    glfwSetWindowShouldClose(mRawWindow, GL_TRUE);
}

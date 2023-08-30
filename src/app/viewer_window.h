#ifndef VIEWER_WINDOW_H
#define VIEWER_WINDOW_H

#include "core/window.h"

#include "app/viewer.h"

class ViewerWindow : public core::Window {
public:
	ViewerWindow(Viewer& app, int width = 1000, int height = 800);
	void update(core::App& app) override;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double dx, double dy);
void handleInput(GLFWwindow* window, Viewer& editor);


#endif
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/app.h"

namespace core {

	class Window {
	public:
		Window(
			App& app,
			int width = 1000,
			int height = 800,
			const char* title = "Window",
			void (*framebuffer_size_callback)(GLFWwindow*, int, int) = nullptr,
			void (*cursor_position_callback)(GLFWwindow*, double, double) = nullptr,
			void (*mouse_button_callback)(GLFWwindow*, int, int, int) = nullptr,
			void (*key_callback)(GLFWwindow*, int, int, int, int) = nullptr,
			void (*scroll_callback)(GLFWwindow*, double, double) = nullptr
		);
		~Window();

		//Get OpenGL glsl version being used
		const char* getGlslVersion();

		//Get underlying handle to window api window
		GLFWwindow* getRawWindow();

		//Check for any events
		void pollEvents();

		//Any update logic based on an app
		virtual void update(App& app);

		//Swap buffer being written to to front
		void swapBuffers();

		//Check if window is closed
		bool closed() const;

		//Close window
		void close();

	protected:
		GLFWwindow* mRawWindow{};
		const char* mGlslVersion{ "#version 330 core" };
	};

}


#endif
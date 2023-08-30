#ifndef UI_H
#define UI_H

#include "core/window.h"

namespace core {
	class Ui {
	public:
		Ui(Window& window);
		void newFrame();
		virtual void update();
		void render();
		void shutdown();
	};
}

#endif
#ifndef APP_H
#define APP_H

namespace core {

	class App {
	public:
		App();
		virtual ~App();
		virtual void init();
		virtual void update();
		virtual void render();

		void setDimensions(int width, int height);

	protected:
		int mWindowWidth{};
		int mWindowHeight{};

	};

}

#endif
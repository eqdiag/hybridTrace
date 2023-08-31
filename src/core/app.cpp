#include "app.h"

core::App::App():
	mWindowWidth{},
	mWindowHeight{}
{
}

core::App::~App()
{
}

void core::App::init() {

}

void core::App::update()
{
}

void core::App::render()
{
}

void core::App::setDimensions(int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
}

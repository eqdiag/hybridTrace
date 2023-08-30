#ifndef VIEWER_UI_H
#define VIEWER_UI_H

#include "core/ui.h"

class ViewerUi : public core::Ui {
public:
	ViewerUi(core::Window& window, Viewer& viewer);
	void update() override;
private:
	Viewer& mViewer;
};

#endif
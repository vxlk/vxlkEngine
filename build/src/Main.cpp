#include "Window.h"

int main(void)
{
	#ifdef VX_GL
		unsigned int mainWindowID = WindowManager::getInstance().addWindow(1064, 860, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		WindowManager::getInstance().getWindowByID(mainWindowID)->update();
	#endif
		return 0;
}
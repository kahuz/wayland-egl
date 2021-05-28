#include "wayland_manager.h"

int main() 
{
	WLManager wl_manager;

	wl_manager.Init();

	wl_manager.CreateWindowWithEGLContext("is my window", WINDOW_WIDTH, WINDOW_HEIGHT);

	wl_manager.Render();

	exit(0);
}

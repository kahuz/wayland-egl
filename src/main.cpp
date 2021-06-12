#include "wayland_manager.h"

int main() 
{
	//create WLManager A method
	/*
	WLManager wl_manager;
	wl_manager.CreateWindowWithEGLContext("is my window", WINDOW_WIDTH, WINDOW_HEIGHT);
	*/
	//create WLManager B method
	WLManager wl_manager("title", 1920, 720); 
	//running wayland render loop
	wl_manager.Render();

	exit(0);
}

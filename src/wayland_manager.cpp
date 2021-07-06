#include "wayland_manager.h"
#include "gl_manager.h"

#define TEST_CODE // test GL Manager 

#ifdef TEST_CODE
GLShader *testShader;

const char vShaderStr[] =  
"attribute vec4 vPosition;    \n"
"void main()                  \n"
"{                            \n"
"   gl_Position = vPosition;  \n"
"}                            \n";

const char fShaderStr[] =  
"precision mediump float;\n"
"void main()                                  \n"
"{                                            \n"
"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
"}                                            \n";

#endif

static struct wl_shell_surface_listener shell_surface_listener = {
	&shell_surface_ping,
	&shell_surface_configure,
	&shell_surface_popup_done
};

static struct wl_seat_listener seat_listener = 
{
	seat_handle_capabilities,	
};

static struct wl_keyboard_listener keyboard_listeners = {
	keyboard_handle_keymap,
	keyboard_handle_enter, keyboard_handle_leave,
	keyboard_handle_key, keyboard_handle_modifiers
};

static struct wl_pointer_listener pointer_listeners = {
	pointer_handle_enter, pointer_handle_leave,
	pointer_handle_motion,pointer_handle_button, pointer_handle_axis,
};

static void global_registry_handle(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version) 
{
	WLComponent *component = (WLComponent *)data;
//	Log("Got a registry event for %s id %d\n", interface, id);
	if (strcmp(interface, "wl_compositor") == 0)
	{
		Log("Create wl compositor\n");
		component->compositor = (wl_compositor *)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
	}
	else if (strcmp(interface, "wl_shell") == 0)
	{
		Log("Create wl shell\n");
		component->shell = (wl_shell *)wl_registry_bind(registry, id, &wl_shell_interface, 1);
	}
	else if( strcmp(interface, "wl_seat") == 0)
	{
		Log("Create wl seat\n");
		component->seat = (wl_seat *)wl_registry_bind(registry, id, &wl_seat_interface, 1);

		wl_seat_add_listener( component->seat, &seat_listener, &component->input_dev);
	}
}

static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id) 
{
	Log("Got a registry losing event for %d\n", id);
}

const struct wl_registry_listener registry_listener = {
	global_registry_handle,
	global_registry_remover
};


void shell_surface_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial) 
{
	wl_shell_surface_pong(shell_surface, serial);
}

void shell_surface_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) 
{
	WLDispInfo *disp_info = (WLDispInfo *)data;
	wl_egl_window_resize(disp_info->egl_info.native_window, width, height, 0, 0);
}

void shell_surface_popup_done(void *data, struct wl_shell_surface *shell_surface) 
{
}


static void seat_handle_capabilities(void *data, struct wl_seat *in_seat, uint32_t caps)
{
	WLInputDev *dev = (WLInputDev *)data;
	if( caps & WL_SEAT_CAPABILITY_POINTER ) 
	{
		struct wl_pointer* in_pointer = wl_seat_get_pointer( in_seat );
		wl_pointer_add_listener( in_pointer, &pointer_listeners, &dev->wl_cursor);
		//dev->mouse = in_pointer;
	}
	else if( !(caps & WL_SEAT_CAPABILITY_POINTER) ) 
	{
		wl_pointer_destroy( dev->mouse );
		dev->mouse = NULL;
	}

	if( caps & WL_SEAT_CAPABILITY_KEYBOARD ) 
	{
		struct wl_keyboard* kbd = wl_seat_get_keyboard( in_seat );
		wl_keyboard_add_listener( kbd, &keyboard_listeners, NULL );
		dev->keyboard = kbd;
	}
	else if( !(caps & WL_SEAT_CAPABILITY_KEYBOARD) ) 
	{
		wl_keyboard_destroy( dev->keyboard );
		dev->keyboard = NULL;
	}
}

static void keyboard_handle_keymap( void* data, struct wl_keyboard* keyboard,  uint32_t format, int fd, uint32_t size )
{
}

static void keyboard_handle_enter( void* data, struct wl_keyboard* keyboard,  uint32_t serial, struct wl_surface* surface, struct wl_array* key )
{
	Log( "Keyboard focused.\n" );
}

static void keyboard_handle_leave( void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface )
{
	Log( "Keyboard lost focus.\n");
}

static void keyboard_handle_key( void* data, struct wl_keyboard* keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state )
{
	Log( "Key %d state : %d\n", key, state );

	if(key == KEY_Q )
	{
		exit(1);
	}
}

static void  keyboard_handle_modifiers( void* data, struct wl_keyboard* keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group )
{
	Log( "Modifiers depressed %d, latched %d, locked %d, group = %d\n", mods_depressed, mods_latched, mods_locked, group );
}


static void pointer_handle_enter( void* data,  struct wl_pointer* pointer, uint32_t serial,  struct wl_surface* surface,  wl_fixed_t sx, wl_fixed_t sy )
{
	WLCursor *cursor = (WLCursor *)data;

	cursor->x = wl_fixed_to_double(sx);
	cursor->y = wl_fixed_to_double(sy);
	cursor->serial = serial;
}  

static void pointer_handle_leave( void* data, struct wl_pointer* pointer, uint32_t serial, struct wl_surface* surface )
{
	WLCursor *cursor = (WLCursor *)data;

	cursor->serial = serial;
}

static void pointer_handle_motion( void* data, struct wl_pointer* pointer, uint32_t time, wl_fixed_t sx, wl_fixed_t sy )
{

	WLCursor *cursor = (WLCursor *)data;

	cursor->x = wl_fixed_to_double(sx);
	cursor->y = wl_fixed_to_double(sy);

	Log( "Moving x : %d y : %d\n", (int)cursor->x, (int)cursor->y);
}

static void pointer_handle_button( void* data, struct wl_pointer* pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t status )
{
	if(button == BTN_LEFT)
	{
		if( status == WL_POINTER_BUTTON_STATE_PRESSED )
		{
			Log("left button press\n");
		}
		else if ( status == WL_POINTER_BUTTON_STATE_RELEASED )
		{
			Log("left button released\n");
		}
	}

	if(button == BTN_RIGHT)
	{
		if( status == WL_POINTER_BUTTON_STATE_PRESSED )
		{
			Log("right button press\n");
		}
		else if ( status == WL_POINTER_BUTTON_STATE_RELEASED )
		{
			Log("right button released\n");
		}
	}

}

static void pointer_handle_axis( void* data, struct wl_pointer* pointer, uint32_t time, uint32_t axis, wl_fixed_t value )
{

}
WLManager::WLManager()
{
	Init();
}

WLManager::WLManager(char *window_title, int window_width, int window_height)
{
	Init();
	CreateWindowWithEGLContext(window_title, window_width, window_height);
}

WLManager::~WLManager()
{
	DeinitEGL();
	DeinitWindow();
	Log("Display disconnected !\n");
}

//Initializing and Registering Objects
void WLManager::Init()
{
	InitWLClient();
	InitWLSurface();
}

void WLManager::InitWLClient()
{
	disp_info.wl_disp = (wl_display *)wl_display_connect(NULL);

	if(disp_info.wl_disp == NULL)
	{	
		Log("Can't create wayland display\n");
		exit(1);	
	}

	disp_info.registry = (wl_registry *)wl_display_get_registry(disp_info.wl_disp);

	//binding wayland compositor & wayland shell on system
	wl_registry_add_listener(disp_info.registry, &registry_listener, &wl_component /* is send to listner*/);

	// This call the attached listener global_registry_handle
	wl_display_dispatch(disp_info.wl_disp);
	// Wait until all events have been processed
	wl_display_roundtrip(disp_info.wl_disp);

	if(wl_component.compositor == NULL)
	{
		Log("Can't create wayland compositor\n");
		exit(1);
	}

	if(wl_component.shell == NULL)
	{
		Log("Can't create wayland shell\n");
		exit(1);
	}

	if(wl_component.seat == NULL)
	{
		Log("Can't create wayland seat\n");
		wl_component.exist_seat = false;
	}
	else
	{
		wl_component.exist_seat = true;
	}

	Log("Okay, we got a compositor and a shell... That's something !\n");
	disp_info.egl_info.native_display = disp_info.wl_disp;

}

void WLManager::InitWLSurface()
{
	window.surface = wl_compositor_create_surface(wl_component.compositor);

	//if (surface == NULL) {
	if (window.surface == NULL) 
	{
		Log("No Compositor surface ! Yay....\n");
		exit(1);
	}
	else 
	{
		Log("Got a compositor surface !\n");
	}

	//create window remote manager ( wl_shell )
	window.shell_surface = wl_shell_get_shell_surface(wl_component.shell, window.surface);

	//move window to top.
	wl_shell_surface_set_toplevel(window.shell_surface);
}

EGLBoolean WLManager::CreateWindowWithEGLContext(const char* title, int width, int height) 
{
	EGLBoolean ret = EGL_FALSE;

	//created surface regions.
	disp_info.region = wl_compositor_create_region(wl_component.compositor);

	//added region infomation.
	wl_region_add(disp_info.region, 0, 0, width, height);
	//added region into surface.
	wl_surface_set_opaque_region(window.surface, disp_info.region);

	//is not doing
	wl_shell_surface_set_title(window.shell_surface, title);

	//created egl native window.
	disp_info.egl_window = wl_egl_window_create(window.surface, width, height);

	if (disp_info.egl_window == EGL_NO_SURFACE) 
	{
		Log("Err EGL No Surface\n");
		exit(1);
	}
	else
	{
		Log("Create EGL Surface\n");
	}

	disp_info.width = width;
	disp_info.height = height;
	disp_info.egl_info.native_window = disp_info.egl_window;

	ret =  CreateEGLContext( &disp_info.egl_info );

#ifdef TEST_CODE
	testShader = new GLShader(vShaderStr, fShaderStr);
	testShader->SetGLAttribLocation(GL_VERTEX_SHADER, "vPosition");
#endif
	wl_shell_surface_add_listener(window.shell_surface, &shell_surface_listener, &disp_info/* is send to listner*/);

	return ret;
}

EGLBoolean WLManager::CreateEGLContext (EGLInfo *egl_info)
{
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLConfig config;
	EGLint attribs[] =
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE,        8,
		EGL_GREEN_SIZE,      8,
		EGL_BLUE_SIZE,       8,
		EGL_NONE
	};

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	egl_info->display = eglGetDisplay( egl_info->native_display );

	if ( egl_info->display == EGL_NO_DISPLAY )
	{
		Log("Err No EGL Display\n");
		return EGL_FALSE;
	}

	// Initialize EGL
	if ( !eglInitialize(egl_info->display, &majorVersion, &minorVersion) )
	{
		Log("Err No Initialisation\n");
		return EGL_FALSE;
	}

	// Get configs
	if ( (eglGetConfigs(egl_info->display, NULL, 0, &numConfigs) != EGL_TRUE) || (numConfigs == 0))
	{
		Log("Err No configuration\n");
		return EGL_FALSE;
	}

	// Choose config
	if ( (eglChooseConfig(egl_info->display, attribs, &config, 1, &numConfigs) != EGL_TRUE) || (numConfigs != 1))
	{
		Log("Err No configuration\n");
		return EGL_FALSE;
	}

	// Create a surface
	egl_info->surface = eglCreateWindowSurface(egl_info->display, config, egl_info->native_window, NULL);

	if ( egl_info->surface == EGL_NO_SURFACE )
	{
		Log("Err No surface\n");
		return EGL_FALSE;
	}

	// Create a GL context
	egl_info->context = eglCreateContext(egl_info->display, config, EGL_NO_CONTEXT, contextAttribs );

	if ( egl_info->context == EGL_NO_CONTEXT )
	{
		Log("Err No context\n");
		return EGL_FALSE;
	}

	// Make the context current
	if ( !eglMakeCurrent(egl_info->display, egl_info->surface, egl_info->surface, egl_info->context) )
	{
		Log("Could not make the current window current\n");
		return EGL_FALSE;
	}

	return EGL_TRUE;
}

void WLManager::Draw()
{
	glClearColor(0.5, 0.3, 0.0, 1.0);

#ifdef TEST_CODE
	GLfloat vVertices[] = {  
		0.0f,  0.5f, 0.0f, 
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f };

	// Set the viewport
	glViewport ( 0, 0, disp_info.width, disp_info.height);

	// Clear the color buffer
	glClear ( GL_COLOR_BUFFER_BIT );

	// Use the program object
	glUseProgram ( testShader->program );

	glEnableVertexAttribArray( testShader->vert_member.at("vPosition") );
	glVertexAttribPointer ( testShader->vert_member.at("vPosition"), 3, GL_FLOAT, GL_FALSE, 0, vVertices );

	glDrawArrays ( GL_TRIANGLES, 0, 3 );
#endif
}

void WLManager::RefreshWindow(EGLInfo egl_info)
{ 
	eglSwapBuffers(egl_info.display, egl_info.surface);
}

void WLManager::Render()
{
	uint32_t frame_no = 0;
	uint32_t resize_type = 0;

	if(is_rendering == false)
	{
		is_rendering = true;
	}

	while(is_rendering)
	{
		// send updated display rendering infomation to wayland compositor.
		//see more wl_display_dispatch_pending into https://manpages.debian.org/experimental/libwayland-doc/wl_display.3.en.html page.
		wl_display_dispatch_pending(disp_info.egl_info.native_display);

		/*
		resize_type |= WL_SHELL_SURFACE_RESIZE_TOP * 0;
		resize_type |= WL_SHELL_SURFACE_RESIZE_BOTTOM * (disp_info.height+frame_no);
		resize_type |= WL_SHELL_SURFACE_RESIZE_LEFT * 0;
		resize_type |= WL_SHELL_SURFACE_RESIZE_RIGHT * (disp_info.width+frame_no);
		//window resize func
		wl_egl_window_resize(disp_info.egl_info.native_window, disp_info.width + frame_no, disp_info.height + frame_no, 0, 0);

		frame_no++;

		if(frame_no > 200)
		{
			frame_no = 0;
		}
		*/

		//gles draw function
		Draw();
		//buffer swap
		RefreshWindow(disp_info.egl_info);
	}

}

void WLManager::Start()
{
	is_rendering = true;
}

void WLManager::Stop()
{
	is_rendering = false;
}

void WLManager::DeinitWindow()
{
	wl_display_disconnect(disp_info.egl_info.native_display);
	wl_egl_window_destroy(disp_info.egl_info.native_window);
	wl_shell_surface_destroy(window.shell_surface);
	wl_surface_destroy(window.surface);
}

void WLManager::DeinitEGL()
{
	eglDestroySurface(disp_info.egl_info.display, disp_info.egl_info.surface);
	eglDestroyContext(disp_info.egl_info.display, disp_info.egl_info.context);
}

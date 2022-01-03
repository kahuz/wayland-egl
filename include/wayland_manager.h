#ifndef __WAYLAND_MANAGER_H__
#define __WAYLAND_MANAGER_H__ 1

// wayland header
#include <wayland-client.h>
#include <wayland-server.h>
#include <wayland-client-protocol.h>
#include <wayland-egl.h> // Wayland EGL MUST be included before EGL headers
#include <wayland-cursor.h>
#include <linux/input.h> //BTN_LEFT, BTN_RIGHT, BTN_MIDDLE

// gl header
#include <GLES2/gl2.h>
#include <EGL/egl.h>

// system header
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#if DEBUG
#define Log( fmt, args...) printf("[%s : %d] " fmt "\n", __func__, __LINE__, ## args) 
#else
#define Log( fmt, args...)
#endif  // #if DEBUG

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

#define KEY_Q 16

struct EGLInfo
{
	/// Native System informations
	EGLNativeDisplayType native_display;
	EGLNativeWindowType native_window;
	/// EGL display
	EGLDisplay  display;
	/// EGL context
	EGLContext  context;
	/// EGL surface
	EGLSurface  surface;
}typedef EGLInfo;

struct WLDispInfo
{
	struct wl_display *wl_disp;
	struct wl_registry *registry;
	struct wl_region *region;
	struct wl_egl_window *egl_window;

	EGLInfo egl_info;

	uint16_t width;
	uint16_t height;

}typedef WLDispInfo;

//use mouse & touchpad
struct WLCursor
{
	int x;
	int y;
	uint32_t serial;
}typedef WLCursor;

struct WLInputDev
{
	struct wl_pointer *mouse;
	struct wl_keyboard *keyboard;
	struct wl_touch *touchpad;

	WLCursor wl_cursor;

}typedef WLInputDev;

struct WLComponent
{
	struct wl_compositor *compositor;
	struct wl_shell *shell;
	struct wl_seat *seat;

	WLInputDev input_dev;
	bool exist_seat;
}typedef WLComponent;

struct WLWindow
{
	struct wl_surface *surface;
	struct wl_shell_surface *shell_surface;

}typedef WLWindow;

class WLManager
{
	public:
		WLDispInfo disp_info;
		WLComponent wl_component;
		WLWindow window;
	private:

		bool is_rendering = false;

	public:
		WLManager();
		WLManager(char *window_title, int window_width, int window_height);

		~WLManager();

		void Init();
		void InitWLClient();
		void InitWLSurface();

		void DeinitWindow();
		void DeinitEGL();

		void Render();
		void Draw();
		void Start();
		void Stop();
		void RefreshWindow(EGLInfo egl_info);

		EGLBoolean CreateEGLContext (EGLInfo *egl_info);
		EGLBoolean CreateWindowWithEGLContext(const char* title, int width, int height);

	private:
};

// wayland global event handle functions
static void global_registry_handle(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version); 
static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id); 
void shell_surface_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial);
void shell_surface_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height); 
void shell_surface_popup_done(void *data, struct wl_shell_surface *shell_surface);
static void seat_handle_capabilities(void *data, struct wl_seat *in_seat, uint32_t caps);
static void keyboard_handle_keymap( void* data, struct wl_keyboard* keyboard,  uint32_t format, int fd, uint32_t size );
static void keyboard_handle_enter( void* data, struct wl_keyboard* keyboard,  uint32_t serial, struct wl_surface* surface, struct wl_array* key );
static void keyboard_handle_leave( void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface );
static void keyboard_handle_key( void* data, struct wl_keyboard* keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state );
static void keyboard_handle_modifiers( void* data, struct wl_keyboard* keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group );
static void pointer_handle_enter( void* data,  struct wl_pointer* pointer, uint32_t serial,  struct wl_surface* surface,  wl_fixed_t sx, wl_fixed_t sy );
static void pointer_handle_leave( void* data, struct wl_pointer* pointer, uint32_t serial, struct wl_surface* surface );
static void pointer_handle_motion( void* data, struct wl_pointer* pointer, uint32_t time, wl_fixed_t sx, wl_fixed_t sy );
static void pointer_handle_button( void* data, struct wl_pointer* pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t status );
static void pointer_handle_axis( void* data, struct wl_pointer* pointer, uint32_t time, uint32_t axis, wl_fixed_t value );
static void touch_handle_down(void *data, struct wl_touch *wl_touch, uint32_t serial, uint32_t time, struct wl_surface *surface, int32_t id, wl_fixed_t x_w, wl_fixed_t y_w);
static void touch_handle_up(void *data, struct wl_touch *wl_touch, uint32_t serial, uint32_t time, int32_t id);
static void touch_handle_motion(void *data, struct wl_touch *wl_touch, uint32_t time, int32_t id, wl_fixed_t x_w, wl_fixed_t y_w);
static void touch_handle_frame(void *data, struct wl_touch *wl_touch);
static void touch_handle_cancel(void *data, struct wl_touch *wl_touch);

#endif //#ifndef __WAYLAND_MANAGER_H__

#ifndef __GL_MANAGER_H__
#define __GL_MANAGER_H__
// gl header
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <map>

static std::map<int, const char*> glErrorMap = {
	//GL error Check
	{GL_NO_ERROR,						"GL_NO_ERROR"},
	{GL_INVALID_ENUM,					"GL_INVALID_ENUM"},
	{GL_INVALID_VALUE,					"GL_INVALID_VALUE"},
	{GL_OUT_OF_MEMORY,					"GL_INVALID_FRAMEBUFFER_OPERATION"},
	{GL_INVALID_FRAMEBUFFER_OPERATION,	"GL_OUT_OF_MEMORY"},
};

static std::map<int, const char*> eglErrorMap = {
	//EGL error Check
    {EGL_SUCCESS, "EGL_SUCCESS"},
    {EGL_NOT_INITIALIZED, "EGL_NOT_INITIALIZED"},
    {EGL_BAD_ACCESS, "EGL_BAD_ACCESS"},
    {EGL_BAD_ALLOC, "EGL_BAD_ALLOC"},
    {EGL_BAD_ATTRIBUTE, "EGL_BAD_ATTRIBUTE"},
    {EGL_BAD_CONTEXT, "EGL_BAD_CONTEXT"},
    {EGL_BAD_CONFIG, "EGL_BAD_CONFIG"},
    {EGL_BAD_CURRENT_SURFACE, "EGL_BAD_CURRENT_SURFACE"},
    {EGL_BAD_DISPLAY, "EGL_BAD_DISPLAY"},
    {EGL_BAD_SURFACE, "EGL_BAD_SURFACE"},
    {EGL_BAD_MATCH, "EGL_BAD_MATCH"},
    {EGL_BAD_PARAMETER, "EGL_BAD_PARAMETER"},
    {EGL_BAD_NATIVE_PIXMAP, "EGL_BAD_NATIVE_PIXMAP"},
    {EGL_BAD_NATIVE_WINDOW, "EGL_BAD_NATIVE_WINDOW"},
    {EGL_CONTEXT_LOST, "EGL_CONTEXT_LOST"},
};

//general functions
int CheckError();

class GLShader
{
	public:
		GLuint program;
		GLuint v_shader;
		GLuint f_shader;

		std::map<const char*, GLint> vert_member;
		std::map<const char*, GLint> frag_member;

	private:

	public:
		GLShader();
		GLShader(const char* vertex_src, const char* fragment_src);

		~GLShader();

		int SetGLAttribLocation(GLenum type, const char* attri_name);
		int SetGLUniformLocation(GLenum type, const char* uniform_name);

	private:
		GLuint CreateShader(GLenum type, const char *shader_src);

};

class GLRenderer
{
	public:
//		GLShader shader[];

	private:

	public:
//		Render();

	private:
};

#endif // #ifndef __GL_MANAGER_H__

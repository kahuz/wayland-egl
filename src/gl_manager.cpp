#include <memory>
#include <iostream>
#include <fstream>
#include <string>

#include "gl_manager.h"
#include "Logger.h"

int CheckError()
{
	int err_type = 0;
	err_type = glGetError();

	if(err_type != GL_NO_ERROR)
	{
		Err("%s", glErrorMap.at(err_type));
		return true;
	}
	return false;
}

GLShader::GLShader()
{

}

GLShader::GLShader(const char* vertex_src, const char* fragment_src)
{
	program = glCreateProgram();
    v_shader = CreateShader(GL_VERTEX_SHADER, vertex_src);
	//add your vertex shader path
    //v_shader = LoadShader(GL_VERTEX_SHADER, vertex_src);
	Log("vertex src\n %s",vertex_src);

	f_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_src);
	//add your fragment shader path
	//f_shader = LoadShader(GL_FRAGMENT_SHADER, fragment_src);
	Log("fragment src\n %s",fragment_src);

	glAttachShader(program, v_shader);
	glAttachShader(program, f_shader);
	glLinkProgram(program);
}

GLShader::~GLShader()
{
	glDetachShader(program, v_shader);
	glDetachShader(program, f_shader);
	glDeleteProgram(program);
	glDeleteShader(v_shader);
	glDeleteShader(f_shader);
}

GLuint GLShader::CreateShader(GLenum type, const char *shader_src)
{
    GLint status = 0;
    GLuint shader_id = glCreateShader(type);

    glShaderSource(shader_id, 1, &shader_src, NULL);
    
	glCompileShader(shader_id);
    
	//check compile result
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    
	// if status value is GL_FALSE, that is error state
	if (status == GL_FALSE) 
	{
        GLint l;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &l);
        char *msg = (char*)malloc(l);
        glGetShaderInfoLog(shader_id, l, NULL, msg);
        Err("compiling shader: %s", msg);
        free(msg);
        glDeleteShader(shader_id);
        shader_id = 0;
    }

    return shader_id;
}

GLuint GLShader::LoadShader(GLenum type, const char *shader_path)
{
	GLuint shader_id = -1;
	std::string shader_src;

	std::ifstream shaderStream(shader_path, std::ios::in);
	
	if(shaderStream.is_open())
	{
		std::string cur_line = "";

		while(getline(shaderStream, cur_line))
		{
			shader_src += "\n" + cur_line;
		}
		shaderStream.close();
	}else
	{
		Err("Don't open %s", shader_path);
		getchar();
		return 0;
	}

	shader_id = CreateShader(type, shader_src.c_str());

	return shader_id;
}

int GLShader::SetGLAttribLocation(GLenum type, const char* attri_name)
{
	GLint tmp_loc = glGetAttribLocation(program, attri_name);

	Log("key [%s] value[%d]",attri_name, tmp_loc);
	if(type == GL_VERTEX_SHADER)
	{
		vert_member.insert(std::pair<const char*, GLint>(attri_name, tmp_loc));
	}
	else if(type == GL_FRAGMENT_SHADER)
	{
		frag_member.insert(std::pair<const char*, GLint>(attri_name, tmp_loc));
	}

	return 0;
}

int GLShader::SetGLUniformLocation(GLenum type, const char* uniform_name)
{
	GLint tmp_uniform = glGetUniformLocation(program, uniform_name);

	Log("key [%s] value[%d]", uniform_name, tmp_uniform);
	if(type == GL_VERTEX_SHADER)
	{
		vert_member.insert(std::pair<const char*, GLint>(uniform_name, tmp_uniform));
	}
	else if(type == GL_FRAGMENT_SHADER)
	{
		frag_member.insert(std::pair<const char*, GLint>(uniform_name, tmp_uniform));
	}

	return 0;
}

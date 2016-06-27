#pragma once

#include "utilities.h"

#include <GL/glew.h>
#include <OpenGL/glu.h>

GLint glvuGetInteger(GLenum e);
bool glvuStateClear();
void _check_gl_error(const char *file, int line);
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)
void printProgramLog( GLuint program );
void printShaderLog( GLuint shader );

GLuint loadTexture(const std::string filename, int &width, int &height);


class PNGLoadException : public VException {
public:
	PNGLoadException(const std::string &e) : VException(e) { }
};

#ifndef GL_OBJS_H
#define GL_OBJS_H
#include "gl.h"

/* EBO abstraction */
struct glo_elmask {
	enum  { GLO_VEC, GLO_EBO } tag;
	union {
		struct { GLuint *buf; GLsizei len; } vec;
		objid                                ebo;
	};
};

// TODO: constructor macros for elmasks

/* VAO abstraction */
struct glo_object { objid vao; };

// TODO: methods for glo_object

/* VBO abstraction */
struct glo_buffer { objid vbo; };

// TODO: constructor macros for VBO


#endif//GL_OBJS_H

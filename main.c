#include "gl.h"
#include "common.h"
#include "shaders.h"
#include "gl_objs.h"
#include <stdio.h>
#include <math.h>

//#define DEFAULT_WINDOW_W 1280
//#define DEFAULT_WINDOW_H  720
#define DEFAULT_WINDOW_W 480
#define DEFAULT_WINDOW_H 480
#define DEFAULT_WINDOW_T "game"

static bool init_external_libs()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return false;

	return true;
}

static void window_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	int retval = 0;
	if (!init_external_libs()) {
		retval = 1; goto exit; }

	/* Initialize the window. */
	GLFWwindow* win = glfwCreateWindow(DEFAULT_WINDOW_W, DEFAULT_WINDOW_H,
	                                   DEFAULT_WINDOW_T, NULL, NULL);
	if (!win) { retval = 1; goto glfwCleanup; }
	glfwMakeContextCurrent(win);
	gladLoadGL();
	window_resize_callback(win, DEFAULT_WINDOW_W, DEFAULT_WINDOW_H);
	glfwSetFramebufferSizeCallback(win, window_resize_callback);

	objid prog = srcs2progs_a(MK_CHARBUF(32),
	                 ((glprog_source[]){vert_shader, frag_shader}))->id;

	const float verts[] = {
		+0.0f, +0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		+0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	GLuint indicies[] = {
		0, 1, 2
	};

	GLint chosenColor = glGetUniformLocation(prog, "chosenColor");
	objid args; glGenVertexArrays(1, &args);
	glBindVertexArray(args);
	{
		objid EBO; glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), &indicies[0], GL_STATIC_DRAW);

		objid verts_bo; glGenBuffers(1, &verts_bo);
		glBindBuffer(GL_ARRAY_BUFFER, verts_bo);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), NULL);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
			glEnableVertexAttribArray(1);
		}
		glBindVertexArray(0); /* necessary or else the delete will unbind */
		glDeleteBuffers(1, &verts_bo);
	}

	/* Event loop. */
	while (!glfwWindowShouldClose(win)) {
		/* Process relevant input events. */
		// TODO

		/* Perform game activities. */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		#if 1
		glBindVertexArray(args);
		glUseProgram(prog);
		glUniform1f(chosenColor, sin(glfwGetTime())/2.f+.5f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		#endif

		/* Yield time to GLFW. */
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

glfwCleanup:
	glfwTerminate();

exit:
	return retval;
}

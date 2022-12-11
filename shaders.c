#include "gl.h"
#include "common.h"
#include "shaders.h"
#include "util.h"
#include <string.h>
// Living place for basic shaders.

#if TODOREMOVE
#define vert_shader_t\
	"#version 330 core\n"\
	"layout (location = 0) in vec3 aPos;\n"\
	"layout (location = 1) in vec3 aClr;\n"\
	"out vec3 vClr;\n"\
	"void main()\n"\
	"{\n"\
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"\
	"vClr = aClr;\n"\
	"}\n"

#define frag_shader_t\
	"#version 330 core\n"\
	"in vec3 vClr;\n"\
	"out vec4 FragColor;\n"\
	"uniform float chosenColor;\n"\
	"void main()\n"\
	"{\n"\
	"FragColor = vec4(vClr*chosenColor, 1.0);\n"\
	"}\n"

const struct shader_source vert_shader = { vert_shader_t, GL_VERTEX_SHADER   };
const struct shader_source frag_shader = { frag_shader_t, GL_FRAGMENT_SHADER };
#endif

// Utilities -- can later separate from shader definitions.

typedef objid  glprog_shobjs[5];

static objid prepare_shprog(glprog_shobjs shos)
{
	if (!shos[0]) return 0;

	objid prog = glCreateProgram();
	if (!prog) return 0;

	for (size_t i = 0; i < sizeof(glprog_shobjs)/sizeof(*shos); ++i)
		if (shos[i]) glAttachShader(prog, shos[i]);
	glLinkProgram(prog);

	return prog;
}

static objid prepare_shprog_debug(glprog_shobjs shos, char **out)
{
	objid retv = prepare_shprog(shos);
	GLint succ;

	glGetProgramiv(retv, GL_LINK_STATUS, &succ);
	if (!succ) {
		static char log[4096];
		glGetProgramInfoLog(retv, sizeof(log), NULL, &log[0]);
		log[sizeof(log)-1] = '\0';
		*out = &log[0];
	} else
		*out = NULL;

	return retv;
}

static objid compile_shader(const struct shader_source *sh) {
	objid shader_o = glCreateShader(sh->type);
	if (!shader_o) return 0;

	glShaderSource(shader_o, 1, (const char *const *)&sh->text, NULL);
	glCompileShader(shader_o);

	return shader_o;
}

static objid compile_shader_debug(const struct shader_source *sh, char **out)
{
	objid retv = compile_shader(sh);
	GLint succ;

	glGetShaderiv(retv, GL_COMPILE_STATUS, &succ);
	if (!succ) {
		static char log[4096];
		glGetShaderInfoLog(retv, sizeof(log), NULL, &log[0]);
		log[sizeof(log)-1] = '\0';

		*out = &log[0];
	} else
		*out = NULL;

	return retv;
}

static
void fillprog(charbuf cb, struct glprog* out, objid pid)
{
	out->id = pid;
	GLuint  i;
	GLint   cnt, _;
	GLsizei len;
	size_t  use = 0;

	glGetProgramiv(pid, GL_ACTIVE_ATTRIBUTES, &cnt);
	for (i = 0; i < cnt; ++i) {
		out->attrs[i].name = cb->buf+use;
		glGetActiveAttrib(pid, i, cb->cap-use, &len, &_,
		                  &out->attrs[i].type, out->attrs[i].name);
		use += len + 1;
	}
	if (i < sizeof(out->attrs)/sizeof(*out->attrs)) out->attrs[i].name = 0;

	glGetProgramiv(pid, GL_ACTIVE_UNIFORMS, &cnt);
	for (i = 0; i < cnt; ++i) {
		out->unifs[i].name = cb->buf+use;
		glGetActiveUniform(pid, i, cb->cap-use, &len, &_,
		                   &out->unifs[i].type, out->unifs[i].name);
		use += len + 1;
	}
	if (i < sizeof(out->unifs)/sizeof(*out->unifs)) out->unifs[i].name = 0;
}

prog_v srcs2progs_impl(charbuf cb, prog_v destbuf, size_t cnt, const glprog_source mat[])
{
	size_t len = sizeof(*mat)/sizeof(**mat) * cnt;
	const struct shader_source *lst = &mat[0][0];

	struct shader_source tab[len]; tab[0] = (struct shader_source){0}; // dummy
	objid                ets[len]; ets[0] = (objid               ){0};
	size_t               end = 1;
	for (size_t cur = 0; cur != len; ++cur)
		if (!lookup(sizeof(*tab), &lst[cur], end, tab))
			memcpy(&tab[end++], &lst[cur], sizeof(struct shader_source));

	prog_v retv = NULL;
	for (size_t i = 1; i < end; ++i)
		if (!(ets[i] = compile_shader(&tab[i]))) goto shdCleanup;


	glprog_shobjs shos;
	for (size_t i = 0; i < cnt; ++i) {
		for (size_t j = 0; j < sizeof(*mat)/sizeof(**mat); ++j) {
			const struct shader_source* q = lookup(sizeof(*tab), &mat[i][j], end, tab);
			shos[j] = ets[q-tab];
		}
		fillprog(cb, &destbuf[i], prepare_shprog(shos));
	}
	retv = destbuf;

shdCleanup:
	for (size_t i = 1; ets[i]; ++i) glDeleteShader(ets[i]);

	return retv;
}

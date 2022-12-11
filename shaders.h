#include "common.h"
#include "gl.h"
#include "util.h"

struct shader_source {
	char   *text;
	GLenum  type;
};

struct glprog {
	objid id;

	struct prog_unif {
		char  *name;
		GLenum type;
	} unifs[16];

	struct prog_attr {
		char  *name;
		GLenum type;
	} attrs[16];
};

typedef struct glprog* prog_v;

typedef struct shader_source glprog_source[5]; // pipeline can be at most 5 long

// determines shader_source size, allocates destbuf
#define srcs2progs_a(cb, arr)\
	srcs2progs_v((cb), sizeof(arr)/sizeof(*(arr)), (arr))

// allocates destbuf
#define srcs2progs_v(cb, l,v)\
	srcs2progs_impl((cb), ALLOCA(sizeof(struct glprog)*(l)), (l), (v))

// parses program sources to generate isspecs and shspecs
prog_v srcs2progs_impl(charbuf cb, prog_v destbuf,
                       size_t cnt, const glprog_source mat[]);


typedef struct {

} glunif;


#if 1
extern const struct shader_source vert_shader;
extern const struct shader_source frag_shader;
#endif

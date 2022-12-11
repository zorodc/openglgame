#ifndef COMMON_H
#define COMMON_H
#include <stdbool.h>
#include <alloca.h>
#include <stddef.h>

#define TODOREMOVE 1 // allow legacy stuff i will remove later
                     // set to zero to find mothballs

#define ALLOCA(s) alloca(s)

typedef unsigned char        uchar;
typedef unsigned short       ushort;
typedef unsigned int         uint;
typedef unsigned long        ulong;
typedef unsigned long long   ullong;

typedef   signed long long    llong;
#endif//COMMON_H

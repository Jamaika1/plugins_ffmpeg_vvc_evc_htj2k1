/* create opj_config_private.h for CMake */
#define OPJ_PACKAGE_VERSION "2.5.0-b0e966e"

/* Not used by openjp2*/
#undef HAVE_MEMORY_H
//#define HAVE_STDLIB_H 1
#undef HAVE_STRINGS_H
#define HAVE_STRING_H 1
#undef HAVE_SYS_STAT_H
#undef HAVE_SYS_TYPES_H
#undef HAVE_UNISTD_H
#define OPJ_HAVE_INTTYPES_H 1
#undef OPJ_HAVE_STDINT_H

/* find whether or not have <malloc.h> */
#undef OPJ_HAVE_MALLOC_H
/* check if function `aligned_alloc` exists */
#undef OPJ_HAVE_ALIGNED_ALLOC
/* check if function `_aligned_malloc` exists */
#undef OPJ_HAVE__ALIGNED_MALLOC
/* check if function `memalign` exists */
#undef OPJ_HAVE_MEMALIGN
/* check if function `posix_memalign` exists */
#undef OPJ_HAVE_POSIX_MEMALIGN

/* config.h.win32.in Merged from two versions generated by configure for gcc and MSVC.  */
/* config.h.  Generated by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */
/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
*/

#define _GLIB_EXTERN extern
#define _GIO_EXTERN extern
#define _GMODULE_EXTERN extern
#define GLIB_DEPRECATED G_DEPRECATED _GLIB_EXTERN
#define GLIB_DEPRECATED_FOR(f) G_DEPRECATED_FOR(f) _GLIB_EXTERN
#define GLIB_VAR _GLIB_EXTERN
#define GOBJECT_VAR _GLIB_EXTERN
#define GLIB_VERSION_MAX_ALLOWED GLIB_VERSION_2_76
#define GLIB_VERSION_MIN_REQUIRED GLIB_VERSION_2_26
#define GOBJECT_VERSION_MAX_ALLOWED GOBJECT_VERSION_2_76
#define GOBJECT_VERSION_MIN_REQUIRED GOBJECT_VERSION_2_26
#define GIO_VERSION_MAX_ALLOWED GIO_VERSION_2_76
#define GIO_VERSION_MIN_REQUIRED GIO_VERSION_2_26
#define GMODULE_VERSION_MAX_ALLOWED GMODULE_VERSION_2_76
#define GMODULE_VERSION_MIN_REQUIRED GMODULE_VERSION_2_26
#define GLIB_COMPILATION
#define GIO_COMPILATION
#define GOBJECT_COMPILATION

#define ALIGNOF_GUINT32 4
#define ALIGNOF_GUINT64 8
#define ALIGNOF_UNSIGNED_LONG 8
#define HAVE__ALIGNED_MALLOC 1

#define XDG_PREFIX _gio_xdg
#define GLIB_RUNSTATEDIR ""
#define GDK_PIXBUF_LIBDIR ""
#define GDK_PIXBUF_LOCALEDIR ""
#define GIO_LAUNCH_DESKTOP ""
#define GDK_PIXBUF_BINARY_VERSION "2.10"
#define HAVE_ROUND 1
#define HAVE_LRINT 1
//#define GDK_PIXBUF_RELOCATABLE 1
#define GDK_PIXBUF_ENABLE_BACKEND 1
#define USE_GMODULE 1

#include "glibconfig.h"

/* #undef CRAY_STACKSEG_END */
/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */
/* Whether to disable memory pools */
/* #undef DISABLE_MEM_POOLS */
/* Whether to enable GC friendliness */
/* #undef ENABLE_GC_FRIENDLY */
/* always defined to indicate that i18n is enabled */
#define ENABLE_NLS 1
/* Define the gettext package to be used */
#define GETTEXT_PACKAGE "gtk4"
/* Define to the GLIB binary age */
#define GLIB_BINARY_AGE 8
/* Byte contents of gmutex */
/* #undef GLIB_BYTE_CONTENTS_GMUTEX */
/* Define to the GLIB interface age */
#define GLIB_INTERFACE_AGE 8
/* Define the location where the catalogs will be installed */
#define GLIB_LOCALE_DIR "NONE/lib/locale"
/* Define to the GLIB major version */
#define GLIB_MAJOR_VERSION 2
/* Define to the GLIB micro version */
#define GLIB_MICRO_VERSION 0
/* Define to the GLIB minor version */
#define GLIB_MINOR_VERSION 76
/* The size of gmutex, as computed by sizeof. */
/* #undef GLIB_SIZEOF_GMUTEX */
/* The size of system_thread, as computed by sizeof. */
#define GLIB_SIZEOF_SYSTEM_THREAD 4
/* alpha atomic implementation */
/* #undef G_ATOMIC_ALPHA */
/* i486 atomic implementation */
#ifndef _MSC_VER
#define G_ATOMIC_I486 1
#endif /* _MSC_VER */
/* ia64 atomic implementation */
/* #undef G_ATOMIC_IA64 */
/* powerpc atomic implementation */
/* #undef G_ATOMIC_POWERPC */
/* sparcv9 atomic implementation */
/* #undef G_ATOMIC_SPARCV9 */
/* x86_64 atomic implementation */
/* #undef G_ATOMIC_X86_64 */
/* Whether glib was compiled with debugging enabled */
#define G_COMPILED_WITH_DEBUGGING "yes"
/* Have inline keyword */
#ifndef _MSC_VER
#define G_HAVE_INLINE 1
#else /* _MSC_VER */
/* #undef G_HAVE_INLINE */
#endif /* _MSC_VER */
/* Have __inline keyword */
#define G_HAVE___INLINE 1
/* Have __inline__ keyword */
#if !defined(_MSC_VER) && !defined(__DMC__)
#define G_HAVE___INLINE__ 1
#else /* _MSC_VER or __DMC__ */
/* #undef G_HAVE___INLINE__ */
#endif /* _MSC_VER or __DMC__ */
/* Source file containing theread implementation */
#define G_THREAD_SOURCE "gthread-win32.c"
/* A 'va_copy' style function */
#ifndef _MSC_VER
#define G_VA_COPY va_copy
#else /* _MSC_VER */
/* #undef G_VA_COPY */
#endif /* _MSC_VER */
/* 'va_lists' cannot be copies as values */
/* #undef G_VA_COPY_AS_ARRAY */
/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1
/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
/* #undef HAVE_ALLOCA_H */
/* Define to 1 if you have the `atexit' function. */
#define HAVE_ATEXIT 1
/* Define to 1 if you have the `bind_textdomain_codeset' function. */
#define HAVE_BIND_TEXTDOMAIN_CODESET 1
/* Define if you have a version of the snprintf function with semantics as
   specified by the ISO C99 standard. */
/* #undef HAVE_C99_SNPRINTF */
/* Define if you have a version of the vsnprintf function with semantics as
   specified by the ISO C99 standard. */
/* #undef HAVE_C99_VSNPRINTF */
/* Have nl_langinfo (CODESET) */
/* #undef HAVE_CODESET */
/* Define to 1 if you have the `dcgettext' function. */
#define HAVE_DCGETTEXT 1
/* Define to 1 if you have the <dirent.h> header file. */
#define HAVE_DIRENT_H 1
/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */
/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */
/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1
/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD 1
/* Define to 1 if you have the `getc_unlocked' function. */
/* #undef HAVE_GETC_UNLOCKED */
/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1
/* define to use system printf */
/* #undef HAVE_GOOD_PRINTF */
/* define to support printing 64-bit integers with format I64 */
#define HAVE_INT64_AND_I64 1
/* Define if you have the 'intmax_t' type in <stdint.h> or <inttypes.h>. */
#ifndef _MSC_VER
#define HAVE_INTMAX_T 1
#else /* _MSC_VER */
/* #undef HAVE_INTMAX_T */
#endif /* _MSC_VER */
/* Define to 1 if you have the <inttypes.h> header file. */
#ifndef _MSC_VER
//#define HAVE_INTTYPES_H 1
#else /* _MSC_VER */
/* #undef HAVE_INTTYPES_H */
#endif /* _MSC_VER */
/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>, and
   declares uintmax_t. */
#ifndef _MSC_VER
//#define HAVE_INTTYPES_H_WITH_UINTMAX 1
#else /* _MSC_VER */
/* #undef HAVE_INTTYPES_H_WITH_UINTMAX */
#endif /* _MSC_VER */
/* Define if you have <langinfo.h> and nl_langinfo(CODESET). */
/* #undef HAVE_LANGINFO_CODESET */
/* Define to 1 if you have the <langinfo.h> header file. */
/* #undef HAVE_LANGINFO_H */
/* Define if your <locale.h> file defines LC_MESSAGES. */
/* #undef HAVE_LC_MESSAGES */
/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1
/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1
/* Define to 1 if you have the `localtime_r' function. */
/* #undef HAVE_LOCALTIME_R */
/* Define if you have the 'long double' type. */
#define HAVE_LONG_DOUBLE 1
/* Define if you have the 'long long' type. */
#ifndef _MSC_VER
#define HAVE_LONG_LONG 1
#else /* _MSC_VER */
/* #undef HAVE_LONG_LONG */
#endif /* _MSC_VER */
/* define if system printf can print long long */
#define HAVE_LONG_LONG_FORMAT 1
/* Define to 1 if you have the `lstat' function. */
/* #undef HAVE_LSTAT */
/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1
/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1
/* Define to 1 if you have the `mkstemp' function. */
/* #undef HAVE_MKSTEMP */
/* Define to 1 if you have the `nanosleep' function. */
/* #undef HAVE_NANOSLEEP */
/* Define to 1 if you have the `nl_langinfo' function. */
/* #undef HAVE_NL_LANGINFO */
/* Have non-POSIX function getpwuid_r */
/* #undef HAVE_NONPOSIX_GETPWUID_R */
/* Define to 1 if you have the `on_exit' function. */
/* #undef HAVE_ON_EXIT */
/* Define to 1 if you have the `poll' function. */
/* #undef HAVE_POLL */
/* Have POSIX function getpwuid_r */
/* #undef HAVE_POSIX_GETPWUID_R */
/* Have function pthread_attr_setstacksize */
/* #undef HAVE_PTHREAD_ATTR_SETSTACKSIZE */
/* Define to 1 if the system has the type `ptrdiff_t'. */
#define HAVE_PTRDIFF_T 1
/* Define to 1 if you have the <pwd.h> header file. */
/* #undef HAVE_PWD_H */
/* Define to 1 if you have the `readlink' function. */
/* #undef HAVE_READLINK */
/* Define to 1 if you have the <sched.h> header file. */
/* #undef HAVE_SCHED_H */
/* Define to 1 if you have the `setenv' function. */
/* #undef HAVE_SETENV */
/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1
/* Define to 1 if you have the `snprintf' function. */
#ifndef _MSC_VER
#define HAVE_SNPRINTF 1
#ifdef __DMC__
#define snprintf _snprintf
#endif
#else /* _MSC_VER */
/* #undef HAVE_SNPRINTF */
#endif /* _MSC_VER */
/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1
/* Define to 1 if you have the <stdint.h> header file. */
#ifndef _MSC_VER
#define HAVE_STDINT_H 1
#else /* _MSC_VER */
/* #undef HAVE_STDINT_H */
#endif /* _MSC_VER */
/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares
   uintmax_t. */
#ifndef _MSC_VER
#define HAVE_STDINT_H_WITH_UINTMAX 1
#else /* _MSC_VER */
/* #undef HAVE_STDINT_H_WITH_UINTMAX */
#endif /* _MSC_VER */
/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1
/* Define to 1 if you have the `stpcpy' function. */
/* #undef HAVE_STPCPY */
/* Define to 1 if you have the `strcasecmp' function. */
#if !defined(_MSC_VER) && !defined(__DMC__)
#define HAVE_STRCASECMP 1
#else /* _MSC_VER or __DMC__ */
/* #undef HAVE_STRCASECMP */
#endif /* _MSC_VER or __DMC__ */
/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1
/* Define to 1 if you have the <strings.h> header file. */
#if !defined(_MSC_VER) && !defined(__DMC__)
#define HAVE_STRINGS_H 1
#else /* _MSC_VER or __DMC__ */
/* #undef HAVE_STRINGS_H */
#endif /* _MSC_VER or __DMC__ */
/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1
/* Have functions strlcpy and strlcat */
/* #undef HAVE_STRLCPY */
/* Define to 1 if you have the `strncasecmp' function. */
#if !defined(_MSC_VER) && !defined(__DMC__)
#define HAVE_STRNCASECMP 1
#else /* _MSC_VER or __DMC__ */
/* #undef HAVE_STRNCASECMP */
#endif /* _MSC_VER or __DMC__ */
/* Define to 1 if you have the `strsignal' function. */
/* #undef HAVE_STRSIGNAL */
/* Define to 1 if you have the `symlink' function. */
/* #undef HAVE_SYMLINK */
/* Define to 1 if you have the <sys/param.h> header file. */
#if !defined(_MSC_VER) && !defined(__DMC__)
#define HAVE_SYS_PARAM_H 1
#else /* _MSC_VER or __DMC__ */
/* #undef HAVE_SYS_PARAM_H */
#endif /* _MSC_VER or __DMC__ */
/* Define to 1 if you have the <sys/poll.h> header file. */
/* #undef HAVE_SYS_POLL_H */
/* found fd_set in sys/select.h */
/* #undef HAVE_SYS_SELECT_H */
/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1
/* Define to 1 if you have the <sys/times.h> header file. */
/* #undef HAVE_SYS_TIMES_H */
/* Define to 1 if you have the <sys/time.h> header file. */
#ifndef _MSC_VER
#define HAVE_SYS_TIME_H 1
#else /* _MSC_VER */
/* #undef HAVE_SYS_TIME_H */
#endif /* _MSC_VER */
/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1
/* Define to 1 if you have the <unistd.h> header file. */
#ifndef _MSC_VER
//#define HAVE_UNISTD_H 1
#else /* _MSC_VER */
/* #undef HAVE_UNISTD_H */
#endif /* _MSC_VER */
/* Define if your printf function family supports positional parameters as
   specified by Unix98. */
/* #undef HAVE_UNIX98_PRINTF */
/* Define to 1 if you have the `unsetenv' function. */
/* #undef HAVE_UNSETENV */
/* Define to 1 if you have the <values.h> header file. */
#if !defined(_MSC_VER) && !defined(__DMC__)
#define HAVE_VALUES_H 1
#else /* _MSC_VER or __DMC__ */
/* #undef HAVE_VALUES_H */
#endif /* _MSC_VER or __DMC__ */
/* Define to 1 if you have the `vasprintf' function. */
#define HAVE_VASPRINTF 1
/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1
/* Define to 1 if you have the `vsnprintf' function. */
#ifndef _MSC_VER
#define HAVE_VSNPRINTF 1
#ifdef __DMC__
#define vsnprintf _vsnprintf
#endif
#else /* _MSC_VER */
/* #undef HAVE_VSNPRINTF */
#endif /* _MSC_VER */
/* Define if you have the 'wchar_t' type. */
#define HAVE_WCHAR_T 1
/* Define if you have the 'wint_t' type. */
#define HAVE_WINT_T 1
/* Have a working bcopy */
/* #undef HAVE_WORKING_BCOPY */
/* didn't find fd_set */
#define NO_FD_SET 1
/* global 'sys_errlist' not found */
#define NO_SYS_ERRLIST 1
/* global 'sys_siglist' not found */
#define NO_SYS_SIGLIST 1
/* global 'sys_siglist' not declared */
#define NO_SYS_SIGLIST_DECL 1
/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://bugzilla.gnome.org/enter_bug.cgi?product=glib"
/* Define to the full name of this package. */
#define PACKAGE_NAME "glib"
/* Define to the full name and version of this package. */
#define PACKAGE_STRING "glib 2.76.0"
/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "glib"
/* Define to the version of this package. */
#define PACKAGE_VERSION "2.76.0"
/* Maximum POSIX RT priority */
/* #undef POSIX_MAX_PRIORITY */
/* Minimum POSIX RT priority */
/* #undef POSIX_MIN_PRIORITY */
/* The POSIX RT yield function */
/* #undef POSIX_YIELD_FUNC */
/* whether realloc (NULL,) works */
#define REALLOC_0_WORKS 1
/* Define if you have correct malloc prototypes */
#ifndef _MSC_VER
#define SANE_MALLOC_PROTOS 1
#else /* _MSC_VER */
/* #undef SANE_MALLOC_PROTOS */
#endif /* _MSC_VER */
/* The size of a `char', as computed by sizeof. */
#define SIZEOF_CHAR 1
/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4
/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4
/* The size of a `long long', as computed by sizeof. */
#ifndef _MSC_VER
#define SIZEOF_LONG_LONG 8
#else /* _MSC_VER */
#define SIZEOF_LONG_LONG 0
#endif /* _MSC_VER */
/* The size of a `short', as computed by sizeof. */
#define SIZEOF_SHORT 2
/* The size of a `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 4
/* The size of a `void *', as computed by sizeof. */
#define GLIB_SIZEOF_VOID_P 8
/* The size of a `__int64', as computed by sizeof. */
#define SIZEOF___INT64 8
/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */
/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1
/* Using GNU libiconv */
//#define USE_LIBICONV_GNU 1
/* Using a native implementation of iconv in a separate library */
/* #undef USE_LIBICONV_NATIVE */
/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */
/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */
/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */
/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */
/* Define to long or long long if <inttypes.h> and <stdint.h> don't define. */
#ifndef _MSC_VER
/* #undef intmax_t */
#else /* _MSC_VER */
#define intmax_t __int64
#endif /* _MSC_VER */
/* Define to empty if the C compiler doesn't support this keyword. */
/* #undef signed */
/* Define to `unsigned' if <sys/types.h> does not define. */
/* #undef size_t */
#define GLIB_STATIC_COMPILATION 1

#if ((__GNUC__ \
      ? defined __GNUC_STDC_INLINE__ && __GNUC_STDC_INLINE__ \
      : (199901L <= __STDC_VERSION__ \
         && !defined __HP_cc \
         && !(defined __SUNPRO_C && __STDC__))) \
     && !defined __APPLE__)
# define _GL_INLINE inline
# define _GL_EXTERN_INLINE extern inline
#elif (2 < __GNUC__ + (7 <= __GNUC_MINOR__) && !defined __STRICT_ANSI__ \
       && !defined __APPLE__)
# if __GNUC_GNU_INLINE__
   /* __gnu_inline__ suppresses a GCC 4.2 diagnostic.  */
#  define _GL_INLINE extern inline __attribute__ ((__gnu_inline__))
# else
#  define _GL_INLINE extern inline
# endif
# define _GL_EXTERN_INLINE extern
#else
# define _GL_INLINE static _GL_UNUSED
# define _GL_EXTERN_INLINE static _GL_UNUSED
#endif
#if 4 < __GNUC__ + (6 <= __GNUC_MINOR__)
# if defined __GNUC_STDC_INLINE__ && __GNUC_STDC_INLINE__
#  define _GL_INLINE_HEADER_CONST_PRAGMA
# else
#  define _GL_INLINE_HEADER_CONST_PRAGMA \
     _Pragma ("GCC diagnostic ignored \"-Wsuggest-attribute=const\"")
# endif
  /* Suppress GCC's bogus "no previous prototype for 'FOO'"
     and "no previous declaration for 'FOO'"  diagnostics,
     when FOO is an inline function in the header; see
     <http://gcc.gnu.org/bugzilla/show_bug.cgi?id=54113>.  */
# define _GL_INLINE_HEADER_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wmissing-prototypes\"") \
    _Pragma ("GCC diagnostic ignored \"-Wmissing-declarations\"") \
    _GL_INLINE_HEADER_CONST_PRAGMA
# define _GL_INLINE_HEADER_END \
    _Pragma ("GCC diagnostic pop")
#else
# define _GL_INLINE_HEADER_BEGIN
# define _GL_INLINE_HEADER_END
#endif



#ifndef ODE_CONFIG_H
#define ODE_CONFIG_H

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix). */
/* #undef HAVE_ALLOCA_H */

/* Use the Apple OpenGL framework. */
/* #undef HAVE_APPLE_OPENGL_FRAMEWORK */

/* Define to 1 if you have the `gettimeofday' function. */
/* #undef HAVE_GETTIMEOFDAY */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `isnan' function. */
/* #undef HAVE_ISNAN */

/* Define to 1 if you have the `isnanf' function. */
/* #undef HAVE_ISNANF */

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define to 1 if you have the `pthread_attr_setstacklazy' function. */
/* #undef HAVE_PTHREAD_ATTR_SETSTACKLAZY */

/* Define to 1 if you have the `pthread_condattr_setclock' function. */
/* #undef HAVE_PTHREAD_CONDATTR_SETCLOCK */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
/* #undef HAVE_SYS_TIME_H */

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
/* #undef HAVE_UNISTD_H */

/* Define to 1 if you have the `_isnan' function. */
#define HAVE__ISNAN 1

/* Define to 1 if you have the `_isnanf' function. */
#define HAVE__ISNANF 1

/* Define to 1 if you have the `__isnan' function. */
/* #undef HAVE___ISNAN */

/* Define to 1 if you have the `__isnanf' function. */
/* #undef HAVE___ISNANF */

/* compiling for a pentium on a gcc-based platform? */
#define PENTIUM 1

/* compiling for a X86_64 system on a gcc-based platform? */
#define X86_64_SYSTEM 1

/* Try to identify the platform */
#if defined(_XENON)
#define ODE_PLATFORM_XBOX360
#elif defined(SN_TARGET_PSP_HW)
#define ODE_PLATFORM_PSP
#elif defined(SN_TARGET_PS3)
#define ODE_PLATFORM_PS3
#elif defined(_MSC_VER) || defined(__CYGWIN32__) || defined(__MINGW32__)
#define ODE_PLATFORM_WINDOWS
#elif defined(__linux__)
#define ODE_PLATFORM_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#define ODE_PLATFORM_OSX
#else
#error "Need some help identifying the platform!"
#endif

/* Additional platform defines used in the code */
#if defined(ODE_PLATFORM_WINDOWS) && !defined(WIN32)
#define WIN32
#endif

#if defined(__CYGWIN32__) || defined(__MINGW32__)
#define CYGWIN
#endif

#if defined(ODE_PLATFORM_OSX)
#define macintosh
#endif

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "typedefs.h"

#endif // ODE_CONFIG_H

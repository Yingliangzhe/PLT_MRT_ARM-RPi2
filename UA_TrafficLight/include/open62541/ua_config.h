#ifndef UA_CONFIG_H_
#define UA_CONFIG_H_

#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE 500
# define _DEFAULT_SOURCE
#endif

#define UA_LOGLEVEL 300
/* #undef UA_ENABLE_MULTITHREADING */
#define UA_ENABLE_METHODCALLS
#define UA_ENABLE_SUBSCRIPTIONS
/* #undef UA_ENABLE_TYPENAMES */
/* #undef UA_ENABLE_EMBEDDED_LIBC */
#define UA_ENABLE_GENERATE_NAMESPACE0
/* #undef UA_ENABLE_EXTERNAL_NAMESPACES */
#define UA_ENABLE_NODEMANAGEMENT

/* #undef UA_ENABLE_NONSTANDARD_UDP */
/* #undef UA_ENABLE_NONSTANDARD_STATELESS */

/**
 * Function Export
 * --------------- */
#ifdef _WIN32
# ifdef UA_DYNAMIC_LINKING
#  ifdef __GNUC__
#   define UA_EXPORT __attribute__ ((dllexport))
#  else
#   define UA_EXPORT __declspec(dllexport)
#  endif
# else
#  ifdef __GNUC__
#   define UA_EXPORT __attribute__ ((dllexport))
#  else
#   define UA_EXPORT __declspec(dllimport)
#  endif
# endif
#else
# if __GNUC__ || __clang__
#  define UA_EXPORT __attribute__ ((visibility ("default")))
# else
#  define UA_EXPORT
# endif
#endif

/**
 * Inline Functions
 * ---------------- */
#ifdef _MSC_VER
# define UA_INLINE __inline
#else
# define UA_INLINE inline
#endif

/**
 * Non-aliasing pointers
 * -------------------- */
#ifdef _MSC_VER
# define UA_RESTRICT __restrict
#elif defined(__GNUC__)
# define UA_RESTRICT __restrict__
#else
# define UA_RESTRICT restrict
#endif

/**
 * Function attributes
 * ------------------- */
#ifdef __GNUC__
# define UA_FUNC_ATTR_MALLOC __attribute__((malloc))
# define UA_FUNC_ATTR_PURE __attribute__ ((pure))
# define UA_FUNC_ATTR_CONST __attribute__((const))
# define UA_FUNC_ATTR_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
# define UA_FUNC_ATTR_MALLOC
# define UA_FUNC_ATTR_PURE
# define UA_FUNC_ATTR_CONST
# define UA_FUNC_ATTR_WARN_UNUSED_RESULT
#endif

/**
 * Integer Endianness
 * ------------------ */
#if defined(_WIN32) || (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) \
                        && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__))
# define htole16(x) (x)
# define htole32(x) (x)
# define htole64(x) (x)
# define le16toh(x) (x)
# define le32toh(x) (x)
# define le64toh(x) (x)
#else
# if defined(__ANDROID__)
#  include <endian.h>
#  define le16toh(x) letoh16(x)
#  define le32toh(x) letoh32(x)
#  define le64toh(x) letoh64(x)
# elif defined(__linux__)
#  include <endian.h>
# elif defined(__OpenBSD__)
#  include <sys/endian.h>
# elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
#  include <sys/endian.h>
#  define le16toh(x) letoh16(x)
#  define le32toh(x) letoh32(x)
#  define le64toh(x) letoh64(x)
# elif defined(__APPLE__)
#  include <libkern/OSByteOrder.h>
#  define htole16(x) OSSwapHostToLittleInt16(x)
#  define htole32(x) OSSwapHostToLittleInt32(x)
#  define htole64(x) OSSwapHostToLittleInt64(x)
#  define le16toh(x) OSSwapLittleToHostInt16(x)
#  define le32toh(x) OSSwapLittleToHostInt32(x)
#  define le64toh(x) OSSwapLittleToHostInt64(x)
# elif defined(__QNX__) || defined(__QNXNTO__)
#  include <gulliver.h>
#  define htole16(x) ENDIAN_LE16(x)
#  define htole32(x) ENDIAN_LE32(x)
#  define htole64(x) ENDIAN_LE64(x)
#  define le16toh(x) ENDIAN_LE16(x)
#  define le32toh(x) ENDIAN_LE32(x)
#  define le64toh(x) ENDIAN_LE64(x)
# else
#  define UA_ENCODING_INTEGER_GENERIC
#  warning No native function for endianness conversion available. Use a slow generic conversion.
# endif
#endif

/**
 * Float Endianness
 * ---------------- */

/* Demote error to a warning on clang. There is no robust way to detect float
 * endianness here. On x86/x86-64, floats are always in the right IEEE 754
 * format. Then, the "Unknown float representation warning" can be ignored or
 * disabled and the UA_ENCODING_FLOAT_GENERIC definition removed to get faster
 * speed. */
#if defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-W#warnings"
#endif

#if !defined(_WIN32)
# if !defined(__FLOAT_WORD_ORDER__) || !defined(__BYTE_ORDER__) || \
     !defined(__ORDER_LITTLE_ENDIAN__) || !defined(__ORDER_BIG_ENDIAN__)
#  define UA_ENCODING_FLOAT_GENERIC
#  warning Unknown float representation. Use a slow manual IEEE 754 conversion.
/* Replace UA_ENCODING_FLOAT_GENERIC with the following section (and adjust
   accordingly to your archtecture) if little-endian IEEE 754 can be achieved
   with a simple byte-swap. For example for middle-endian encoding on some
   ARM CPUs. */
/* #define UA_ENCODING_FLOAT_SWAP */
/* #define UA_swap32(u32) ((u32 >> 24) | ((u32 << 8) & 0x00FF0000) | ((u32 >> 8) & 0x0000FF00) | (u32 << 24)) */
/* #define UA_swap64(u64) ((u64 >> 56) | ((u64 << 40) & 0x00FF000000000000) | ((u64 << 24) & 0x0000FF0000000000) | \ */
/*                        ((u64 << 8) & 0x000000FF00000000) | ((u64 >> 8) & 0x00000000FF000000) |                 \ */
/*                        ((u64 >> 24) & 0x0000000000FF0000) | ((u64 >> 40) & 0x000000000000FF00) | (u64 << 56)) */
# endif
#endif

#if defined(__clang__)
#pragma GCC diagnostic pop
#endif

/**
 * Embed unavailable libc functions
 * -------------------------------- */
#include <stddef.h>
#ifdef UA_ENABLE_EMBEDDED_LIBC
  void *memcpy(void *UA_RESTRICT dest, const void *UA_RESTRICT src, size_t n);
  void *memset(void *dest, int c, size_t n);
  size_t strlen(const char *s);
  int memcmp(const void *vl, const void *vr, size_t n);
#else
# include <string.h>
#endif

#endif /* UA_CONFIG_H_ */

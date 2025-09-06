#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

//Structs and enums definitions
enum __clar_loglevel : uint8_t {
  CLAR_LOG_FATAL = 0,
  CLAR_LOG_ERROR = 1,
  CLAR_LOG_WARNING = 2,
  CLAR_LOG_INFO = 3,
  CLAR_LOG_DEBUG = 4
};

enum __clar_outputmask : uint8_t {
  CLAR_OUT_STDOUT = (1 << 0),
  CLAR_OUT_FILE =   (1 << 1)
};

struct __clar_logtarget {
  FILE* file;
  uint8_t output_mask;
};

struct Clarifier {
  enum __clar_loglevel loglevel;
  struct __clar_logtarget logtarget;
  const char* format;
};
// Internals
extern struct Clarifier __clar_default_fmt;
extern void __clar_log(
    struct Clarifier* clar,
    enum __clar_loglevel loglevel,
    const char* fmt,
    va_list* args
);
// API
inline struct Clarifier clar_create_logger(
    enum __clar_loglevel level,
    struct __clar_logtarget target,
    const char* fmt  
    ) {
  return (struct Clarifier){ level, target, fmt };
}

inline struct __clar_logtarget clar_create_logtarget(
    const char* filename,
    uint8_t outmask
    ) {
  struct __clar_logtarget target;
  target.file = fopen(filename, "w");
  if (!target.file) {
    printf("Failed to open file %s to log", filename);
    abort();
  }
  target.output_mask = outmask;
  return target;
}

inline void clar_set_format(const char* restrict fmt) {
  if (__clar_default_fmt.format) {
    free(__clar_default_fmt.format);
    __clar_default_fmt.format = NULL;
  }
  __clar_default_fmt.format = strdup(fmt);
}

#define __CLAR_EXPAND(x) x
#define __CLAR_GET_MACRO(_1, name, ...) name
#define CLAR_INIT(...) __CLAR_EXPAND(__CLAR_GET_MACRO(__VA_ARGS__, __CLAR_INIT2, __CLAR_INIT1)(__VA_ARGS__))

#define __CLAR_INIT1() 

#define __CLAR_INIT2(arg) \
  __clar_default_fmt.format = strdup(arg);

static inline void clar_log(
    enum __clar_loglevel loglevel,
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, loglevel, fmt, &args);

  va_end(args);
}
static inline void clar_log_with(
    struct Clarifier* clar,
    enum __clar_loglevel loglevel,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&clar, loglevel, fmt, &args);

  va_end(args);
}

static inline void clar_debug(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_DEBUG, fmt, &args);

  va_end(args);
}

static inline void clar_debug_with(
    struct Clarifier* clar,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  __clar_log(clar, CLAR_LOG_DEBUG, fmt, &args);

  va_end(args);
}

static inline void clar_info(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_INFO, fmt, &args);

  va_end(args);
}
static inline void clar_info_with(
    struct Clarifier* clar,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  __clar_log(clar, CLAR_LOG_DEBUG, fmt, &args);

  va_end(args);
}

static inline void clar_warn(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_WARNING, fmt, &args);

  va_end(args);
}

static inline void clar_warn_with(
    struct Clarifier* clar,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  __clar_log(clar, CLAR_LOG_WARNING, fmt, &args);

  va_end(args);
}

static inline void clar_err(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_ERROR, fmt, &args);

  va_end(args);
}

static inline void clar_err_with(
    struct Clarifier* clar,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  __clar_log(clar, CLAR_LOG_ERROR, fmt, &args);

  va_end(args);
}

static inline void clar_fatal(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_FATAL, fmt, &args);

  va_end(args);
}

static inline void clar_fatal_with(
    struct Clarifier* clar,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  __clar_log(clar, CLAR_LOG_FATAL, fmt, &args);

  va_end(args);
}

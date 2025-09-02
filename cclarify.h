#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

// Some useful macroses
#define CRED     "\x1b[31m"
#define CGREEN   "\x1b[32m"
#define CYELLOW  "\x1b[33m"
#define CRESET   "\x1b[0m"

//Structs and enums definitions
enum __clar_loglevel : uint8_t {
  CLAR_LOG_FATAL = 0,
  CLAR_LOG_ERROR = 1,
  CLAR_LOG_WARNING = 2,
  CLAR_LOG_INFO = 3,
  CLAR_LOG_DEBUG = 4
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

// Internal data
extern const char* __clar_descs[];
extern char* __clar_fmt;
extern bool __clar_logging_enabled;

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

#define __CLAR_EXPAND(x) x
#define __CLAR_GET_MACRO(_1, name, ...) name
#define CLAR_INIT(...) __CLAR_EXPAND(__CLAR_GET_MACRO(__VA_ARGS__, __CLAR_INIT2, __CLAR_INIT1)(__VA_ARGS__))

#define __CLAR_INIT1() 

#define __CLAR_INIT2(arg) \
  __clar_fmt = strdup(arg);

void clar_log(
    enum __clar_loglevel loglevel,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  if (!__clar_fmt) {
    vprintf(fmt, args);
    return;
  }
  for (uint32_t i = 0; i < strlen(__clar_fmt); ++i) {
    switch (__clar_fmt[i]) {
      case '%':
        switch (__clar_fmt[i + 1]) {
          case 'S':
            vprintf(fmt, args);
            break;
          case 'd':
            fputs(__clar_descs[(uint8_t)loglevel], stdout);
            break;
          case '%':
            putchar('%');
            break;
        }
        ++i;
        break;
      default:
        putchar(__clar_fmt[i]);
        break;
    }
  }
}

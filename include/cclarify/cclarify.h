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
  const char* filename;
  uint8_t output_mask;
  uint16_t max_files;
  uint32_t max_size;
  uint32_t cur_size;
};

struct clarifier {
  enum __clar_loglevel loglevel;
  struct __clar_logtarget logtarget;
  const char* format;
};
// Internals
extern struct clarifier __clar_default_fmt;
extern void __clar_log(
    struct clarifier* clar,
    enum __clar_loglevel loglevel,
    const char* fmt,
    va_list* args
);
// API
static inline struct clarifier clar_create_logger(
    enum __clar_loglevel level,
    struct __clar_logtarget target,
    const char* fmt  
    ) {
  return (struct clarifier){ level, target, fmt };
}

static inline struct __clar_logtarget clar_create_logtarget(
    const char* filename,
    uint8_t outmask
    ) {
  struct __clar_logtarget target;
  target.file = fopen(filename, "w");
  target.filename = filename;
  if (!target.file) {
    printf("[[cclarify]] Failed to open file %s to log", filename);
  }
  target.output_mask = outmask;
  return target;
}

static inline void clar_destroy_logger(struct clarifier* clar) {
  if (clar->logtarget.file) {
    fclose(clar->logtarget.file);
    clar->logtarget.file = NULL;
  }
  clar->logtarget.output_mask = 0;
  clar->format = NULL;
}

static inline void clar_destroy_logtarget(struct clarifier* clar) {
  if (clar->logtarget.file) {
    fclose(clar->logtarget.file);
  }
  memset(&clar->logtarget, '\0', sizeof(__clar_logtarget));
}

static inline void clar_set_global_rotation(const char* filename, uint16_t max_file_count, uint32_t max_size) {
  __clar_default_fmt.logtarget.filename = filename;
  __clar_default_fmt.logtarget.max_files = max_file_count;
  __clar_default_fmt.logtarget.max_size = max_size;
}

static inline void clar_set_global_format(const char* restrict fmt) {
  __clar_default_fmt.format = fmt;
}

static inline void clar_set_global_loglevel(enum __clar_loglevel loglevel) {
  __clar_default_fmt.loglevel = loglevel;
}

static inline void clar_set_global_logger(struct clarifier* clar) {
  memcpy(&__clar_default_fmt, clar, sizeof(struct clarifier));
}

static inline void clar_set_rotation(struct clarifier* clar, const char* filename, uint16_t max_file_count, uint32_t max_size) {
  clar->logtarget.filename = filename;
  clar->logtarget.max_files = max_file_count;
  clar->logtarget.max_size = max_size;
}


#define __CLAR_EXPAND(x) x
#define __CLAR_GET_MACRO(_1, name, ...) name
#define CLAR_INIT(...) __CLAR_EXPAND(__CLAR_GET_MACRO(__VA_ARGS__, __CLAR_INIT2, __CLAR_INIT1)(__VA_ARGS__))

#define __CLAR_INIT1() 

#define __CLAR_INIT2(arg) \
  __clar_default_fmt.format = arg;

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
    struct clarifier* clar,
    enum __clar_loglevel loglevel,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  __clar_log(clar, loglevel, fmt, &args);

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
    struct clarifier* clar,
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
    struct clarifier* clar,
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
    struct clarifier* clar,
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
    struct clarifier* clar,
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
    struct clarifier* clar,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  __clar_log(clar, CLAR_LOG_FATAL, fmt, &args);

  va_end(args);
}

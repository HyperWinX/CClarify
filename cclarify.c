#include <__stdarg_va_list.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "cclarify.h"

#define CRED     "\x1b[31m"
#define CYELLOW  "\x1b[33m"
#define CRESET   "\x1b[0m"


const char* __clar_descs[] = {
  "fatal",
  "error",
  "warning",
  "info",
  "debug"
};

const char* __clar_colors[] = {
  CRED,
  CRED,
  CYELLOW,
  CRESET,
  CRESET
};

struct Clarifier __clar_default_fmt;

char* __clar_fmt = NULL;
char* __clar_buf = NULL;
bool __clar_logging_enabled = true;

[[gnu::visibility("hidden")]]
uint32_t __clar_get_fmt_size(
    const char* restrict fmt,
    va_list* args
    ) {
  return vsnprintf(NULL, 0, fmt, *args);
}

[[gnu::visibility("hidden")]]
void sputs(
    char* buf,
    const char* src,
    uint32_t* offset
    ) {
  uint32_t len = strlen(src);
  memcpy(buf, src, len);
  *offset += len;
}

[[gnu::visibility("hidden")]]
void __clar_format(
    struct Clarifier* clar,
    enum __clar_loglevel loglevel,
    char* buf,
    char* fmt,
    va_list* args
    ) {
  if (!clar) {
    clar = &__clar_default_fmt;
  }

  uint32_t offset = 0;

  for (uint32_t i = 0; i < strlen(clar->format); ++i) {
    switch (clar->format[i]) {
      case '%':
        switch (clar->format[i + 1]) {
          case 'S':
            offset += vsnprintf(buf + offset, 0xFFFF, fmt, *args);
            break;
          case 'd':
            sputs(buf + offset, __clar_colors[(uint8_t)loglevel], &offset);
            sputs(buf + offset, __clar_descs[(uint8_t)loglevel], &offset);
            sputs(buf + offset, CRESET, &offset);
            break;
          case '%':
          default:
            buf[offset++] = '%';
            break;
        }
        ++i;
        break;
      default:
        buf[offset++] = clar->format[i];
        break;
    }
  }
  buf[offset++] = '\n';
}

void __clar_log(
    struct Clarifier* clar,
    enum __clar_loglevel loglevel,
    const char* fmt,
    va_list* args
    ) {
  uint32_t sz = __clar_get_fmt_size(fmt, args) + 2048;
  char* buf = alloca(sz);
  if (!buf) {
    return;
  }
  memset(buf, 0x00, sz);

  __clar_format(clar, loglevel, buf, fmt, args);

  if (clar->logtarget.output_mask & CLAR_OUT_STDOUT) {
    fputs(buf, stdout);
  }
  if (clar->logtarget.output_mask & CLAR_OUT_FILE &&
      clar->logtarget.file) {
    fwrite(buf, 1, sz, clar->logtarget.file);
  }
}

void clar_log(
    enum __clar_loglevel loglevel,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, loglevel, fmt, &args);

  va_end(args);
}

void clar_log_fd(
    struct Clarifier clar,
    const char* restrict fmt,
    ...
    ) {
  va_list args;
  va_start(args, fmt);
  char* buf = alloca(__clar_get_fmt_size(fmt, &args) + 2048);
  va_end(args);

}

void clar_debug(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_DEBUG, fmt, &args);

  va_end(args);
}

void clar_info(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_INFO, fmt, &args);

  va_end(args);
}

void clar_warn(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_WARNING, fmt, &args);

  va_end(args);
}

void clar_err(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_ERROR, fmt, &args);

  va_end(args);
}

void clar_fatal(
    const char* restrict fmt,
    ...) {
  va_list args;
  va_start(args, fmt);

  __clar_log(&__clar_default_fmt, CLAR_LOG_FATAL, fmt, &args);

  va_end(args);
}

[[gnu::constructor]]
void __clar_construct() {
  __clar_default_fmt.format = strdup("[%d] %S");
  __clar_default_fmt.loglevel = CLAR_LOG_DEBUG;
  __clar_default_fmt.logtarget.output_mask = CLAR_OUT_STDOUT;
}

[[gnu::destructor]]
void __clar_destruct() {
  if (__clar_default_fmt.format) {
    free(__clar_default_fmt.format);
  }
}

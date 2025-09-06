#include <__stdarg_va_list.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "cclarify.h"

#define CRED     "\x1b[31m"
#define CYELLOW  "\x1b[33m"
#define CRESET   "\x1b[0m"


const char* __clar_descs[] = {
  "fatal",
  "error",
  "warn",
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
uint32_t __clar_format(
    struct Clarifier* clar,
    enum __clar_loglevel loglevel,
    char* buf,
    char* fmt,
    va_list* args,
    bool colors
    ) {
  if (!clar) {
    clar = &__clar_default_fmt;
  }

  uint32_t offset = 0;
  time_t cur_time = time(NULL);
  struct tm* timeinfo = localtime(&cur_time);

  for (uint32_t i = 0; i < strlen(clar->format); ++i) {
    switch (clar->format[i]) {
      case '%':
        switch (clar->format[i + 1]) {
          case 'Y':
            offset += strftime(buf + offset, 0xFFFF, "%Y", timeinfo);
            break;
          case 'M':
            offset += strftime(buf + offset, 0xFFFF, "%b", timeinfo);
            break;
          case 'd':
            offset += strftime(buf + offset, 0xFFFF, "%a", timeinfo);
            break;
          case 'D':
            offset += strftime(buf + offset, 0xFFFF, "%d", timeinfo);
            break;
          case 'H':
            offset += strftime(buf + offset, 0xFFFF, "%H", timeinfo);
            break;
          case 'm':
            offset += strftime(buf + offset, 0xFFFF, "%M", timeinfo);
            break;
          case 's':
            offset += strftime(buf + offset, 0xFFFF, "%S", timeinfo);
            break;
          case 'l':
            offset += vsnprintf(buf + offset, 0xFFFF, fmt, *args);
            break;
          case 'x':
            if (colors) {
              sputs(buf + offset, __clar_colors[(uint8_t)loglevel], &offset);
              sputs(buf + offset, __clar_descs[(uint8_t)loglevel], &offset);
              sputs(buf + offset, CRESET, &offset);
            } else {
              sputs(buf + offset, __clar_descs[(uint8_t)loglevel], &offset);
            }
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
  return offset;
}

void __clar_log(
    struct Clarifier* clar,
    enum __clar_loglevel loglevel,
    const char* fmt,
    va_list* args
    ) {
  if (loglevel > clar->loglevel) {
    return;
  }
  uint32_t sz = __clar_get_fmt_size(fmt, args) + 2048;
  char* buf = alloca(sz);
  if (!buf) {
    return;
  }
  memset(buf, 0x00, sz);

  if (clar->logtarget.output_mask & CLAR_OUT_STDOUT) {
    __clar_format(clar, loglevel, buf, fmt, args, true);
    fputs(buf, stdout);
  }
  if (clar->logtarget.output_mask & CLAR_OUT_FILE &&
      clar->logtarget.file) {
    uint32_t size = __clar_format(clar, loglevel, buf, fmt, args, false);
    fwrite(buf, 1, size, clar->logtarget.file);
  }
}

void clar_log(
    enum __clar_loglevel loglevel,
    const char* restrict fmt,
    ...
);

[[gnu::constructor]]
void __clar_construct() {
  __clar_default_fmt.format = "%Y %M %d %D %H:%m:%s [%x] %l";
  __clar_default_fmt.loglevel = CLAR_LOG_DEBUG;
  __clar_default_fmt.logtarget.output_mask = CLAR_OUT_STDOUT;
}


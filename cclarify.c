#include <stdbool.h>
#include <stdlib.h>

const char* __clar_descs[] = {
  "fatal",
  "error",
  "warning",
  "info",
  "debug"
};

char* __clar_fmt = NULL;
char* __clar_buf = NULL;
bool __clar_logging_enabled = true;



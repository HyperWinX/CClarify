#include "cclarify.h"

int main(){
	CLAR_INIT("%Y %M %d %D %H:%m:%s [%x] %l");
  struct Clarifier clar = clar_create_logger(CLAR_LOG_DEBUG, clar_create_logtarget("test.log", CLAR_OUT_FILE), "[%x] %l");
  clar_debug("test");
  clar_info("test");
  clar_warn("test");
  clar_err("test");
  clar_fatal("test");
  clar_fatal_with(&clar, "test");
  clar_log(CLAR_LOG_DEBUG, "Test %s %d", "HyperWin", 5);

  clar_debug("I'll try print a message with loglevel lower than the current global loglevel.");
  clar_set_global_loglevel(CLAR_LOG_WARNING);
  clar_info("Message that should not be seen");
  clar_set_global_loglevel(CLAR_LOG_DEBUG);
  clar_debug("See? It works. Same with custom loggers.");
  clar.loglevel = CLAR_LOG_WARNING;
  clar_info_with(&clar, "You shouldn't see this.");
}

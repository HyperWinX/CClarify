#include "cclarify.h"

int main(){
	CLAR_INIT("%Y %M %d %D %H:%m:%s [%x] %l");
  clar_debug("test");
  clar_info("test");
  clar_warn("test");
  clar_err("test");
  clar_fatal("test");
  clar_log(CLAR_LOG_DEBUG, "Test %s %d", "HyperWin", 5);
}

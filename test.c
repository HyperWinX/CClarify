#include "cclarify.h"

int main(){
	CLAR_INIT("[%d] %S");
  clar_log(CLAR_LOG_DEBUG, "Test %s %d", "HyperWin", 5);
}

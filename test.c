#include "cclarify.h"

int main(){
	GLOBAL_INIT();
	Clarifier clar;
	int dupped = dup(fileno(stdout));
	init_loggerd(&clar, dupped);
	unsigned int t;
	ASSIGN_VAL(clar, t, 5);
}

#include "cclarify.h"

int main(){
	GLOBAL_INIT();
	Clarifier clar;
	int dupped = dup(fileno(stdout));
	init_loggerd(&clar, dupped);
	unsigned int t = 666;
	ASSIGN(clar, t, 5);
	MSG(clar, "Bruh", INFO);
	MSG(clar, "Bruh", WARNING);
	MSG(clar, "Bruh", ERROR);
}

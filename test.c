#include "cclarify.h"

void func(){}
int func_assign(){return 1;}

int main(){
	GLOBAL_INIT();
	Clarifier clar;
	int dupped = dup(fileno(stdout));
	init_loggerd(&clar, dupped);
	uint64_t t = 666;
	ASSIGN(clar, t, 5);
	MSG(clar, "Bruh", INFO);
	MSG(clar, "Bruh", WARNING);
	MSG(clar, "Bruh", ERROR);
	EXEC(clar, func());
	MSG(clar, "Lets assign function return value to variable!", INFO);
	ASSIGN(clar, t, func_assign());
	DISPLAY(clar, t);
}

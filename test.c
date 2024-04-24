#include "cclarify.h"

void func(){}
int func_assign(){return 1;}

int main(){
	GLOBAL_INIT();
	Clarifier clar;
	int dupped = dup(fileno(stdout));
	init_loggerd(clar, dupped);
	SET_DEFAULT(clar);
	uint64_t t = 666;
	ASSIGN(clar, t, 5);
	MSG(clar, "Bruh", INFO);
	MSG(clar, "Bruh", WARNING);
	MSG(clar, "Bruh", ERROR);
	EXEC(clar, func());
	MSG(clar, "Lets assign function return value to variable!", INFO);
	ASSIGN(clar, t, func_assign());
	DISPLAY(clar, t);
	void* ptr1 = malloc(16);
	void* ptr2 = calloc(16, 1);
	ptr1 = realloc(ptr1, 32);
	free(ptr1);
	free(ptr2);
	FILE* fd = fopen("testfile", "rw");
	char buf[16];
	fwrite(buf, 1, 16, fd);
	rewind(fd);
	fread(buf, 1, 16, fd);
	fclose(fd);
}

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// Some useful macroses
#define CRED     "\x1b[31m"
#define CGREEN   "\x1b[32m"
#define CYELLOW  "\x1b[33m"
#define CRESET   "\x1b[0m"

//Structs and enums definitions
typedef enum Output{
	FILEO,
	STDOUTO,
	ALL
} Output;

typedef enum MsgType{
	INFO,
	WARNING,
	ERROR
} MsgType;

typedef struct Clarifier{
	Output outmode;
	int cclarify_stdout;
	FILE* cclarify_fd;
	char* prompt;
	void (*log)(char*,MsgType);
} Clarifier;

// Variables
int cclarify_stdout = 0;
FILE* cclarify_fd;
Output outmode;

// Functions
#define GLOBAL_INIT() \
		char buffer[512] = {0}
#define CLEARBUF() memset(buffer, 0x00, strlen(buffer))
#define ASSIGN_VAL(var, val) \
		_Generic((var), \
		uint8_t: unsigned_assign, \
		uint16_t: unsigned_assign, \
		uint32_t: unsigned_assign, \
		uint64_t: unsigned_assign, \
		int8_t: signed_assign, \
		int16_t: signed_assign, \
		int32_t: signed_assign, \
		int64_t: signed_assign, \
		float: float_assign, \
		double: double_assign, \
		char*: charp_assign, \
		void*: voidp_assign)((uint64_t)var, (uint64_t)val, buffer)

void unsigned_assign(uint64_t var, uint64_t val, char* buffer) {
		snprintf(buffer, sizeof(buffer), "%s%s Assigning value \"%lu\" to %s, old value: %lu%s", CGREEN, prompt, (uint64_t)val, #var, var, CRESET);
		log(buffer, INFO);
		var = val;
		CLEARBUF();
}

void log(char* msg, MsgType type){
	switch(type){
		case INFO:
			fputs(cclarify_stdout, buffer);
			break;
		case ERROR:
			break;
	}
}
void init_logger(Clarifier* obj, int descriptor){
	obj->outmode = STDOUT;
	obj->cclarify_stdout = descriptor;
}

void init_logger(Clarifier* obj, FILE* fd){
	obj->outmode = FILEO;
	obj->cclarify_fd = fd;
}

void init_logger(Clarifier* obj, int descriptor, FILE* fd){
	obj->outmode = ALL;
	obj->cclarify_stdout = descriptor;
	obj->cclarify_fd = fd;
}

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
	void (*write)(struct Clarifier*,char*,MsgType);
} Clarifier;

// Variables
int cclarify_stdout = 0;
FILE* cclarify_fd;
Output outmode;
char* prompt = "==>";

void log_write(char* msg, MsgType type);

// Functions
void write_buf(Clarifier* obj, char* msg, MsgType type){
	write(obj->cclarify_stdout, msg, strlen(msg));
}
void write_fd(Clarifier* obj, char* msg, MsgType type){
	fprintf(obj->cclarify_fd, "%s", msg);
}

void write_all(Clarifier* obj, char* msg, MsgType type){
	write(obj->cclarify_stdout, msg, strlen(msg));
	fprintf(obj->cclarify_fd, "%s", msg);
}

#define GLOBAL_INIT() \
		char buffer[512] = {0}
#define CLEARBUF() memset(buffer, 0x00, strlen(buffer))
#define ASSIGN_VAL(obj, var, val) \
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
		void*: voidp_assign)(obj, var, #var, val, buffer)

void unsigned_assign(Clarifier obj, uint64_t var, char* strvar, uint64_t val, char* buffer) {
		snprintf(buffer, 512, "%s%s Assigning value \"%lu\" to %s, old value: %lu%s\n", CGREEN, prompt, (uint64_t)val, strvar, var, CRESET);
		log_write(buffer, INFO);
		var = val;
		CLEARBUF();
}

void signed_assign(Clarifier obj, int64_t var, char* strvar, int64_t val, char* buffer){ }

void float_assign(Clarifier obj, float var, char* strvar, float val, char* buffer){ }

void double_assign(Clarifier obj, double var, char* strvar, double val, char* buffer){ }

void charp_assign(Clarifier obj, char* var, char* strvar, char* val, char* buffer){ }

void voidp_assign(Clarifier obj, void* var, char* strvar, void* val, char* buffer){ }

void log_write(char* msg, MsgType type){
	switch(type){
		case INFO:
			write(cclarify_stdout, msg, strlen(msg));
			break;
		case ERROR:
			break;
	}
}
void init_loggerd(Clarifier* obj, int descriptor){
	obj->outmode = STDOUTO;
	obj->cclarify_stdout = descriptor;
	obj->write = &write_buf;
}

void init_loggerf(Clarifier* obj, FILE* fd){
	obj->outmode = FILEO;
	obj->cclarify_fd = fd;
	obj->write = &write_fd;
}

void init_loggerfd(Clarifier* obj, int descriptor, FILE* fd){
	obj->outmode = ALL;
	obj->cclarify_stdout = descriptor;
	obj->cclarify_fd = fd;
	obj->write = &write_all;
}

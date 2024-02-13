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
char* strings[] = {
		"%s%s Assigning value \"%lu\" to %s, old value: %lu%s\n",
		"%s%s Assigning value \"%ld\" to %s, old value: %ld%s\n",
		"%s%s Assigning value \"%lf\" to %s, old value: %lf%s\n",
		"%s%s Assigning value \"%s\" to %s, old value: %s%s\n",
		"%s%s Assigning value \"%p\" to %s, old value: %s%s\n"
};

void log_write(char* msg, MsgType type);

// Functions
char* ul_dummy(){return strings[0];}
char* dl_dummy(){return strings[1];}
char* lf_dummy(){return strings[2];}
char* s_dummy(){return strings[3];}
char* vp_dummy(){return strings[4];}
char* getcol(MsgType type){
	switch (type){
		case INFO:
			return CGREEN;
		case WARNING:
			return CYELLOW;
		case ERROR:
			return CRED;
	}
}
void write_buf(Clarifier* obj, char* msg, MsgType type){
	write(obj->cclarify_stdout, getcol(type), 5);     write(obj->cclarify_stdout, msg, strlen(msg));
    write(obj->cclarify_stdout, CRESET, 1);
}
void write_fd(Clarifier* obj, char* msg, MsgType type){
	fprintf(obj->cclarify_fd, "%s", msg);
}

void write_all(Clarifier* obj, char* msg, MsgType type){
	write(obj->cclarify_stdout, getcol(type), 5);
	write(obj->cclarify_stdout, msg, strlen(msg));
	write(obj->cclarify_stdout, CRESET, 1);
	fprintf(obj->cclarify_fd, "%s", msg);
}

#define GLOBAL_INIT() \
		char cclarify_buffer[512] = {0}; \
		char cclarify_col = 0; \
		char* cclarify_str = 0;
#define CLEARBUF() memset(cclarify_buffer, 0x00, strlen(cclarify_buffer))
#define ASSIGN(obj, var, val) \
		cclarify_str = _Generic(var, \
						uint8_t: ul_dummy, \
						uint16_t: ul_dummy, \
						uint32_t: ul_dummy, \
						uint64_t: ul_dummy, \
						int8_t: dl_dummy, \
						int16_t: dl_dummy, \
						int32_t: dl_dummy, \
						int64_t: dl_dummy, \
						float: lf_dummy, \
						double: lf_dummy, \
						char*: s_dummy, \
						void*: vp_dummy)(); \
		snprintf(cclarify_buffer, sizeof(cclarify_buffer), cclarify_str, CGREEN, prompt, val, #var, var, CRESET); \
		obj.write(&obj, cclarify_buffer, INFO); \
		CLEARBUF();
#define MSG(obj, msg, type) \
		snprintf(cclarify_buffer, sizeof(cclarify_buffer), "%s %s\n", prompt, msg); \
		obj.write(&obj, cclarify_buffer, type); \
		CLEARBUF();

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

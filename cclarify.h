#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

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
int cclarify_depth = 0;
FILE* cclarify_fd;
Output outmode;
char* prompt = "==>";
char* assign_strings[] = {
	"%s Assigning value \"%u\" to %s, old value: %u",
	"%s Assigning value \"%lu\" to %s, old value: %lu\n",
	"%s Assigning value \"%d\" to %s, old value: %d\n",
	"%s Assigning value \"%ld\" to %s, old value: %ld\n",
	"%s Assigning value \"%f\" to %s, old value: %f\n",
	"%s Assigning value \"%lf\" to %s, old value: %lf\n",
	"%s Assigning value \"%s\" to %s, old value: %s\n",
	"%s Assigning value \"%p\" to %s, old value: %p\n"
};

char* display_strings[] = {
	"%s Variable %s value: \"%u\"\n",
	"%s Variable %s value: \"%lu\"\n",
	"%s Variable %s value: \"%d\"\n",
	"%s Variable %s value: \"%ld\"\n",
	"%s Variable %s value: \"%f\"\n",
	"%s Variable %s value: \"%lf\"\n",
	"%s Variable %s value: \"%s\"\n",
	"%s Variable %s value: \"%p\"\n"
};

void log_write(char* msg, MsgType type);

// Functions
// Dummies
char* u_dummy_assign(){return assign_strings[0];}
char* lu_dummy_assign(){return assign_strings[1];}
char* d_dummy_assign(){return assign_strings[2];}
char* ld_dummy_assign(){return assign_strings[3];}
char* f_dummy_assign(){return assign_strings[4];}
char* lf_dummy_assign(){return assign_strings[5];}
char* s_dummy_assign(){return assign_strings[6];}
char* vp_dummy_assign(){return assign_strings[7];}

char* u_dummy_display(){return display_strings[0];}
char* lu_dummy_display(){return display_strings[1];}
char* d_dummy_display(){return display_strings[2];}
char* ld_dummy_display(){return display_strings[3];}
char* f_dummy_display(){return display_strings[4];}
char* lf_dummy_display(){return display_strings[5];}
char* s_dummy_display(){return display_strings[6];}
char* vp_dummy_display(){return display_strings[7];}

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
	write(obj->cclarify_stdout, getcol(type), 5);
    write(obj->cclarify_stdout, msg, strlen(msg));
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

void write_data(Clarifier* restrict clar, char* restrict buffer, size_t size, MsgType type, char* restrict format, ...){
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, size, format, args);
	clar->write(clar, buffer, type);
	va_end(args);
}

#define GLOBAL_INIT() \
	char cclarify_buffer[512] = {0}; \
	char cclarify_col = 0; \
	char* cclarify_str = 0;
#define CLEARBUF() memset(cclarify_buffer, 0x00, strlen(cclarify_buffer))
#define ASSIGN(obj, var, val) \
	cclarify_str = _Generic(var, \
		uint8_t: u_dummy_assign, \
		uint16_t: u_dummy_assign, \
		uint32_t: u_dummy_assign, \
		uint64_t: lu_dummy_assign, \
		int8_t: d_dummy_assign, \
		int16_t: d_dummy_assign, \
		int32_t: d_dummy_assign, \
		int64_t: ld_dummy_assign, \
		float: f_dummy_assign, \
		double: lf_dummy_assign, \
		char*: s_dummy_assign, \
		void*: vp_dummy_assign)(); \
	write_data(&obj, cclarify_buffer, sizeof(cclarify_buffer), INFO, cclarify_str, prompt, val, #var, var); \
	var = val; \
	CLEARBUF();
#define EXEC(obj, func) \
	snprintf(cclarify_buffer, sizeof(cclarify_buffer), "%s Starting execution of function %s\n", prompt, #func); \
	obj.write(&obj, cclarify_buffer, INFO); \
	cclarify_depth++; \
	CLEARBUF(); \
	func; \
	snprintf(cclarify_buffer, sizeof(cclarify_buffer), "%s Execution of function %s finished\n", prompt, #func); \
	obj.write(&obj, cclarify_buffer, INFO); \
	cclarify_depth--; \
	CLEARBUF();
#define MSG(obj, msg, type) \
	snprintf(cclarify_buffer, sizeof(cclarify_buffer), "%s %s\n", prompt, msg); \
	obj.write(&obj, cclarify_buffer, type); \
	CLEARBUF();
#define DISPLAY(obj, var) \
	cclarify_str = _Generic(var, \
		uint8_t: u_dummy_display, \
		uint16_t: u_dummy_display, \
		uint32_t: u_dummy_display, \
		uint64_t: lu_dummy_display, \
		int8_t: d_dummy_display, \
		int16_t: d_dummy_display, \
		int32_t: d_dummy_display, \
		int64_t: ld_dummy_display, \
		float: f_dummy_display, \
		double: lf_dummy_display, \
		char*: s_dummy_display, \
		void*: vp_dummy_display)(); \
	write_data(&obj, cclarify_buffer, sizeof(cclarify_buffer), INFO, cclarify_str, prompt, #var, var); \
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
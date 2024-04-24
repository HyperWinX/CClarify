#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

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
Clarifier cclarify_default = {0};
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
	"%s Variable \"%s\" value: \"%u\"\n",
	"%s Variable \"%s\" value: \"%lu\"\n",
	"%s Variable \"%s\" value: \"%d\"\n",
	"%s Variable \"%s\" value: \"%ld\"\n",
	"%s Variable \"%s\" value: \"%f\"\n",
	"%s Variable \"%s\" value: \"%lf\"\n",
	"%s Variable \"%s\" value: \"%s\"\n",
	"%s Variable \"%s\" value: \"%p\"\n"
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
	snprintf(cclarify_buffer, sizeof(cclarify_buffer), "%s Starting execution of function \"%s\"\n", prompt, #func); \
	obj.write(&obj, cclarify_buffer, INFO); \
	cclarify_depth++; \
	CLEARBUF(); \
	func; \
	snprintf(cclarify_buffer, sizeof(cclarify_buffer), "%s Execution of function \"%s\" finished\n", prompt, #func); \
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
#define SET_DEFAULT(obj) cclarify_default = obj;
#define init_loggerd(obj, desc) _init_loggerd(&obj, desc);
void _init_loggerd(Clarifier* obj, int descriptor){
	obj->outmode = STDOUTO;
	obj->cclarify_stdout = descriptor;
	obj->write = &write_buf;
}

#define init_loggerf(obj, fd) _init_loggerd(&obj, fd);
void _init_loggerf(Clarifier* obj, FILE* fd){
	obj->outmode = FILEO;
	obj->cclarify_fd = fd;
	obj->write = &write_fd;
}

#define init_loggerfd(obj, desc, fd) _init_loggerfd(&obj, desc, fd);
void _init_loggerfd(Clarifier* obj, int descriptor, FILE* fd){
	obj->outmode = ALL;
	obj->cclarify_stdout = descriptor;
	obj->cclarify_fd = fd;
	obj->write = &write_all;
}

/* All function wrappers */

void* __real_malloc(size_t);
void* __real_calloc(size_t,size_t);
void* __real_realloc(void*,size_t);
void __real_free(void*);

FILE* __real_fopen(const char*,const char*);
size_t __real_fread(void* restrict,size_t,size_t,FILE* restrict);

void* __wrap_malloc(size_t size){
	char cclarify_buffer[128];
	char buf[64];
	void* ptr = __real_malloc(size);
	if (ptr){
		snprintf(buf, sizeof(buf), "Successful malloc() call, size=%ld", size);
		MSG(cclarify_default, buf, INFO);
	} else {
		snprintf(buf, sizeof(buf), "Failed malloc() call, size=%ld", size);
	}
	return ptr;
}

void* __wrap_calloc(size_t num, size_t size){
	char cclarify_buffer[128];
	char buf[128];
	void* ptr = __real_calloc(num, size);
	if (ptr){
		snprintf(buf, sizeof(buf), "Successful calloc() call, bs=%ld, count=%ld", size, num);
		MSG(cclarify_default, buf, INFO);
	} else {
		snprintf(buf, sizeof(buf), "Failed calloc() call, bs=%ld, count=%ld", size, num);
		MSG(cclarify_default, buf, ERROR);
	}
	return ptr;
}

void* __wrap_realloc(void* old_ptr, size_t new_size){
	char cclarify_buffer[128];
	char buf[128];
	void* ptr = __real_realloc(old_ptr, new_size);
	if (ptr){
		snprintf(buf, sizeof(buf), "Successful realloc() call, old_ptr=%p, new_size=%ld", old_ptr, new_size);
		MSG(cclarify_default, buf, INFO);
	} else {
		snprintf(buf, sizeof(buf), "Failed realloc() call, old_ptr=%p, new_size=%ld, nothing to do", old_ptr, new_size);
		MSG(cclarify_default, buf, ERROR);
	}
	return ptr;
}

void __wrap_free(void* ptr){
	char cclarify_buffer[128];
	char buf[64];
	__real_free(ptr);
	snprintf(buf, sizeof(buf), "Freed memory block with address %p", ptr);
	MSG(cclarify_default, buf, INFO);
}

FILE* __wrap_fopen(const char* filename, const char* mode){
	char cclarify_buffer[512];
	char buf[512];
	FILE* ptr = __real_fopen(filename, mode);
	if (ptr){
		snprintf(buf, sizeof(buf), "Successful fopen() call, filename=%s, mode=%s", filename, mode);
		MSG(cclarify_default, buf, INFO);
	} else {
		snprintf(buf, sizeof(buf), "Failed fopen() call, filename=%s, mode=%s", filename, mode);
		MSG(cclarify_default, buf, ERROR);
	}
	return ptr;
}

size_t __wrap_fread(void* restrict buffer, size_t size, size_t count, FILE* restrict stream){
	char cclarify_buffer[128];
	char buf[128];
	if (!size || !count){
		snprintf(buf, sizeof(buf), "Fread() was called with size=%ld, count=%ld, nothing to do", size, count);
		MSG(cclarify_default, buf, ERROR);
		return 0;
	}
	size_t read = __real_fread(buffer, size, count, stream);
	if (read != count){
		if (feof(stream)){
			snprintf(buf, sizeof(buf), "Fread() call error: unexpected end of file. Buf=%p, size=%ld, count=%ld, fd=%p, blocks read: %ld", buf, size, count, stream, read);
			MSG(cclarify_default, buf, INFO);
		} else if (ferror(stream)){
			snprintf(buf, sizeof(buf), "Fread() call error: unknown error. Buf=%p, size=%ld, count=%ld, fd=%p", buf, size, count, stream);
			MSG(cclarify_default, buf, INFO);
		}
		return read;
	}
	snprintf(buf, sizeof(buf), "Successful fread() call, buf=%p, size=%ld, count=%ld, fd=%p", buf, size, count, stream);
	MSG(cclarify_default, buf, INFO);
	return read;
}

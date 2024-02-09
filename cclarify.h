
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

#define log_assign(var, val) _Generic((var)

void log_stdout(char* msg, MsgType type){
	switch(type){
		case ERROR:
				fprintf(cclarify_stdout, 
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
	obj->cclarify_stdout = fd;
	obj->cclarify_fd = fd;
}

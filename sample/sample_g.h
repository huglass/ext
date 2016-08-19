#ifndef PHP_SAMPLE_GLOBAL_H
#define PHP_SAMPLE_GLOBAL_H

#define DEBUG

typedef struct _php_sample_descriptor_data {
	char *filename;
	FILE *fp;
} php_sample_descriptor_data;


// define about resource
#define PHP_SAMPLE_DESCRIPTOR_RES_NAME "Sample File Descriptor"

extern int le_sample_descriptor;
extern int le_sample_descriptor_persist;


/*
helper macro for debug
*/
#ifdef DEBUG
#define MY_FI(x) php_printf("EXE " PHP_SAMPLE_EXTNAME":" #x "\n")
//#define INFO(fmt, ...) php_printf("[%s@%s:%d] " fmt, __FUNCTION__, __FILE__,  __LINE__, ##__VA_ARGS__)
#define INFO(fmt, args...) php_printf("[%s@%s:%d]" fmt, __FUNCTION__, basename(__FILE__),  __LINE__, ##args)
#else
//#define MY_FI(x)    
//#define INFO(fmt, args...)    
#define MY_FI(x) do{}while(0)
#define INFO(fmt, args...) do{}while(0)
#endif



PHP_MINIT_FUNCTION(sample);

#endif
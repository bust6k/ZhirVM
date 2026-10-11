#include "../lib/zhirtypes.h"


void*  (*kalloc)(u32 size);
void   (*printf)(char* fmt,...);
void   (*print_color)(char* string,u32 color);
struct file* (*open)(char disk_letter,char* path);

INIT void init(void* _resolve_function(char* name)) {
    kalloc = _resolve_function("_alloc");
    printf = _resolve_function("_printf");
    print_color = _resolve_function("_print_color");
    open = _resolve_function("_open");
    LOGI("the ZhirVM interpreter initialized");
    //struct file* foo = open('A',"./vmeste_mi_edynyi.zo");

}



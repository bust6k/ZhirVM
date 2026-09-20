#include "zhirvm_types.c"

#define INT_MAX 2147483647
#define NOT_ACTUAL -1

int is_digit(char digit) {
return digit >= 0 && digit <= 9;
}

int is_alpha(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int is_space(char c) {
    return c == ' '  || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

int is_identifier(char* buf) {
for(int i = 0;i < strlen(buf);i++) {
if(is_space(buf[i])) kernel_panic(6);
if(i == 0 && is_number(buf[i])) kernel_panic(6);
if(!is_alpha((int)buf[i])) kernel_panic(6);
}
return 1;
}

int is_number(char* buf) {
for(int i = 0;i < strlen(buf);i++) {
if(!is_digit(buf[i])) return 0;
}
return 1;
}
//string to integer
int sti(char* buf) {
int r = 0;
for(int i = 0;i < strlen(buf);i++) {
if(!is_digit(buf[i])) kernel_panic(6);
if(r > (INT_MAX - (buf[i] - '0') / 10)) kernel_panic(6);
r = r * 10 + (buf[i] - '0');
}
return r;
}

char* get_string(char* buf) {
char* res = (char*)kalloc(strlen(buf) + 1);
int i = 0;
for(;i <strlen(buf);i++) {
if(buf[i] == '"') break;
res[i] = buf[i];
}
res[i] = '\0';

return res;
}

void do_lex(char* buf) {
if(!strcmp(buf,"u32")){buf+=3;return u32_type;}
else if(!strcmp(buf,"u16")){buf+=3;return u16_type;}
else if(!strcmp(buf,"u8")){buf+=2;return u16_type;}
else if(!strcmp(buf,"str")){buf+=3;return string_as_type;}
else if(!strcmp(buf,"fp")){buf+=2;return float_type;}
else if(!strcmp(buf,"if")){buf+=2;return u16_type;}
else if(!strcmp(buf,"udf")){buf+=3;return udf_statement;}
else if(!strcmp(buf,"while")){buf+=5;return while_statement;}
else if(!strcmp(buf,"else")){buf+=4;return else_statement;}
else if(!strcmp(buf,"ret")){buf+=3;return return_statement;}
else if(!strcmp(buf,"var")){buf+=3;return var_statement;}
else if(!strcmp(buf,"varar")){buf+=5;return varar_statement;}
else if(!strcmp(buf,"class")){buf+=3;return class_statement;}
else if(!strcmp(buf,"reg")){buf+=3;return register_statement;}
else if(!strcmp(buf,"goto")){buf+=3;return goto_statement;}
else if(!strcmp(buf,"==")){buf+=2;return equals;}
else if(!strcmp(buf,">")){buf+=1;return greater_than;}
else if(!strcmp(buf,"<")){buf+=1;return lesser_than;}
else if(!strcmp(buf,"=")){buf+=1;return assign;}
else if(!strcmp(buf,"^")){buf+=1;return xor;}
else if(!strcmp(buf,"~")){buf+=1;return not;}
else if(!strcmp(buf,"&")){buf+=1;return and;}
else if(!strcmp(buf,"|")){buf+=1;return or;}
else if(!strcmp(buf,"(")){buf+=1;return left_paren;}
else if(!strcmp(buf,")")){buf+=1;return right_paren;}
else if(!strcmp(buf,"/")){buf+=1;return divide;}
else if(!strcmp(buf,"*")){buf+=1;return multiply;}
else if(!strcmp(buf,"-")){buf+=1;return minus;}
else if(!strcmp(buf,"+")){buf+=1;return plus;}
else if(!strcmp(buf,"{")){buf+=1;return left_brace;}
else if(!strcmp(buf,"}")){buf+=1;return right_brace;}
else if(is_number(buf)){
int num = sti(buf); 
num1 = num << 8; 
buf += strlen(buf); 
return number | num1;
}
else if(is_identifier(buf)) {
char* ptr = (char*)kalloc(strlen(buf));
ptr = buf;
ptr1 = ptr << 8;
buf += strlen(buf);
return identifier | ptr1; 
}
else if(buf[0] == '"') {
buf++;
char* str = get_string(buf);
str1 = str << 8;
buf += strlen(buf) - 1;
return string_as_type | str1;
}
if(is_space(buf[0])){return NOT_ACTUAL;}
}

int lex(struct file* file) {
	int size = file->getsize(file);
	char* buf = (char*)kalloc(size);
	file->read(file,buf,size,0);
        do_lex(buf);

	return 0;
}

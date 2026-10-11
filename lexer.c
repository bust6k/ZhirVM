#include "zhirvm_types.c"
#include "../lib/string.h"
#include "../lib/zhirtypes.h"

#define INT_MAX 2147483647
#define NOT_ACTUAL -1

void*  (*kalloc)(u32 size);
void   (*printf)(char* fmt,...);
void   (*print_color)(char* string,u32 color);
struct file* (*open)(char disk_letter,char* path);
Token* lex(struct file* file);

int read_count = 0;
char* buf = 0;

int is_digit(char digit) {
    return digit >= '0' && digit <= '9';
}

int is_alpha(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int is_space(char c) {
    return c == ' '  || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

int is_number(char* buf) {
    for(int i = 0;i < strlen(buf);i++) {
        if(!is_digit(buf[i])) return 0;
    }
    return 1;
}

void init_string(struct file* file) {
    int size = file->getsize(file);
    buf = (char*)kalloc(size+1);
    file->read(file,buf,size,0);
    buf[size] = '\0';
}

Token* make_token_number(int val) {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->number.val = val;

return token;
}

Token* make_token_identifier(char* name) {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->identifier.name = name;

return token;
}



Token* make_token_string(char* val) {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->string.val = val;

return token;
}


Token* make_token_keyword(int kind) {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->keyword.kind = kind;

return token;
}

Token* make_token_operation(int op) {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->operation.op = op;

return token;
}


Token* make_token_null() {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->null.not_actual = NOT_ACTUAL;

return token;
}





char* read_string(void) {
    while (*buf && is_space(*buf)) buf++;
    if (!*buf) return 0;

    char* token = buf;

    if (*buf == '"') {
        buf++;
        while (*buf && *buf != '"') buf++;
        if (*buf == '"') buf++;
        if (*buf) { *buf = '\0'; buf++; }
        return token;
    }

    while (*buf && !is_space(*buf)) buf++;
    if (*buf) { *buf = '\0'; buf++; }
    return token;
}

int is_identifier(char* buf) {
    for(int i = 0;i < strlen(buf);i++) {
        if(is_space(buf[i])) LOGE("you're so stupid!");
        if(i == 0 && is_number(buf)) LOGE("I wanna fuck your asshole!");
        if(!is_alpha((int)buf[i])) LOGE("I think you should go to hospital.");
    }
    return 1;
}

//string to integer
int sti(char* buf) {
    int r = 0;
    for(int i = 0;i < strlen(buf);i++) {
        if(!is_digit(buf[i])) LOGE("you can't hack the system!");
        if(r > (INT_MAX - (buf[i] - '0') / 10)) LOGE("I have care you that it's dangerous...");
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

char* get_identifier(char* buf) {
    char* res = (char*)kalloc(strlen(buf) + 1);
    int i = 0;
    for(;i <strlen(buf);i++) {
        if(is_space(buf[i])) break;
        res[i] = buf[i];
    }
    res[i] = '\0';

    return res;
}

Token* do_lex(void) {
    char* tok = read_string();
    if (!tok) return make_token_null();

    if(!strcmp(tok,"u32")) return make_token_keyword(u32_type);
    else if(!strcmp(tok,"u16")) return make_token_keyword(u16_type);
    else if(!strcmp(tok,"u8")) return make_token_keyword(u8_type);
    else if(!strcmp(tok,"str")) return make_token_keyword(string_as_type);
    else if(!strcmp(tok,"fp")) return make_token_keyword(float_type);
    else if(!strcmp(tok,"if")) return make_token_keyword(if_statement);
    else if(!strcmp(tok,"udf")) return make_token_keyword(udf_statement);
    else if(!strcmp(tok,"whl")) return make_token_keyword(while_statement);
    else if(!strcmp(tok,"else")) return make_token_keyword(else_statement);
    else if(!strcmp(tok,"ret")) return make_token_keyword(return_statement);
    else if(!strcmp(tok,"var")) return make_token_keyword(var_statement);
    else if(!strcmp(tok,"varar")) return make_token_keyword(varar_statement);
    else if(!strcmp(tok,"class")) return make_token_keyword(class_statement);
    else if(!strcmp(tok,"reg")) return make_token_keyword(register_statement);
    else if(!strcmp(tok,"goto")) return make_token_keyword(goto_statement);
    else if(!strcmp(tok,"==")) return make_token_operation(equals);
    else if(!strcmp(tok,">")) return make_token_operation(greater_than);
    else if(!strcmp(tok,"<")) return make_token_operation(lesser_than);
    else if(!strcmp(tok,"=")) return make_token_operation(assign);
    else if(!strcmp(tok,"^")) return make_token_operation(xor);
    else if(!strcmp(tok,"~")) return make_token_operation(not);
    else if(!strcmp(tok,"&")) return make_token_operation(and);
    else if(!strcmp(tok,"|")) return make_token_operation(or);
    else if(!strcmp(tok,"(")) return make_token_operation(left_paren);
    else if(!strcmp(tok,")")) return make_token_operation(right_paren);
    else if(!strcmp(tok,"/")) return make_token_operation(divide);
    else if(!strcmp(tok,"*")) return make_token_operation(multiply);
    else if(!strcmp(tok,"-")) return make_token_operation(minus);
    else if(!strcmp(tok,"+")) return make_token_operation(plus);
    else if(!strcmp(tok,"{")) return make_token_operation(left_brace);
    else if(!strcmp(tok,"}")) return make_token_operation(right_brace);
    else if(is_number(tok)) {
        int num = sti(tok);
        return make_token_number(num);
    }
    else if(is_identifier(tok)) {
        char* idt = get_identifier(tok+1);
	return make_token_identifier(idt);
    }
    else if(tok[0] == '"') {
        char* str = get_string(tok + 1);
        return make_token_string(str);
    }

    return make_token_null();
}

INIT void init(void* _resolve_function(char* name)) {
    kalloc = _resolve_function("_alloc");
    printf = _resolve_function("_printf");
    print_color = _resolve_function("_print_color");
    open = _resolve_function("_open");

    struct file* foo = open('A',"./vmeste_mi_edynyi.zo");
    Token* f = lex(foo);

    LOGI("%u",f->keyword.kind);

    LOGI("HELLO FROM PIECE OF SHIT!");
}


Token* lex(struct file* file) {
    static int loaded = 0;
    if (!loaded) {
        init_string(file);
        loaded = 1;
    }
    return do_lex();
}

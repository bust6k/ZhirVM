#include "zhirvm_types.h"
#include "../lib/string.h"
#include "../lib/zhirtypes.h"
#include "parse.h"
#include "lexer.h"
#include "interpret.h"

void* (*kalloc)(u32 size) = 0;
void (*printf)(char* fmt, ...) = 0;
void (*print_color)(char* string, u32 color) = 0;
struct file* (*open)(char disk_letter, char* path) = 0;

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

int is_number_buf(char* buf) {
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
token->kind = number;
token->number.val = val;

return token;
}

Token* make_token_identifier(char* name) {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->kind = identifier;
token->identifier.name = name;

return token;
}



Token* make_token_string(char* val) {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->kind = string;
token->string.val = val;

return token;
}


Token* make_token_keyword(int kind) {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->kind = kind;
token->keyword.kind = kind;

return token;
}

Token* make_token_operation(int op) {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->kind = op;
token->operation.op = op;

return token;
}


Token* make_token_null() {
Token* token = (Token*)kalloc(sizeof(Token) * 1);
token->kind = token_eof;
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

int is_identifier_buf(char* buf) {
    for(int i = 0;i < strlen(buf);i++) {
        if(is_space(buf[i])) return 0;
        if(i == 0 && is_number_buf(buf)) return 0;
        if(!is_alpha((int)buf[i])) return 0;
    }
    return 1;
}

int sti(char* buf) {
    int r = 0;
    for(int i = 0;i < strlen(buf);i++) {
        if(!is_digit(buf[i])) return NOT_ACTUAL;
        if(r > (INT_MAX - (buf[i] - '0') / 10)) return NOT_ACTUAL;
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

static Token* classify_word(char* w) {
    if(!strcmp(w,"u32")) return make_token_keyword(u32_type);
    else if(!strcmp(w,"u16")) return make_token_keyword(u16_type);
    else if(!strcmp(w,"u8")) return make_token_keyword(u8_type);
    else if(!strcmp(w,"str")) return make_token_keyword(string_type);
    else if(!strcmp(w,"fp")) return make_token_keyword(float_type);
    else if(!strcmp(w,"if")) return make_token_keyword(if_keyword);
    else if(!strcmp(w,"udf")) return make_token_keyword(udf_keyword);
    else if(!strcmp(w,"udp")) return make_token_keyword(udp_keyword);
    else if(!strcmp(w,"whl")) return make_token_keyword(while_keyword);
    else if(!strcmp(w,"else")) return make_token_keyword(else_keyword);
    else if(!strcmp(w,"ret")) return make_token_keyword(return_keyword);
    else if(!strcmp(w,"return")) return make_token_keyword(return_keyword);
    else if(!strcmp(w,"var")) return make_token_keyword(var_keyword);
    else if(!strcmp(w,"varar")) return make_token_keyword(varar_keyword);
    else if(!strcmp(w,"call")) return make_token_keyword(call_keyword);
    else if(!strcmp(w,"class")) return make_token_keyword(class_keyword);
    else if(!strcmp(w,"reg")) return make_token_keyword(register_keyword);
    else if(!strcmp(w,"goto")) return make_token_keyword(goto_keyword);

    if(is_number_buf(w)) {
        int num = sti(w);
        if(num != NOT_ACTUAL) return make_token_number(num);
        return make_token_null();
    }

    if(is_identifier_buf(w)) {
        Token* token = (Token*)kalloc(sizeof(Token) * 1);
        token->kind = identifier;
        token->identifier.name = w;
        return token;
    }

    return make_token_null();
}

Token* do_lex(void) {
    for (;;) {
        while (*buf && is_space(*buf)) buf++;
        if (!*buf) return make_token_null();

        char c = *buf;

        if (c == '/' && buf[1] == '/') {
            while (*buf && *buf != '\n') buf++;
            continue;
        }

        if (c == '"') {
            buf++;
            char* start = buf;
            while (*buf && *buf != '"') buf++;
            int len = (int)(buf - start);
            char* res = (char*)kalloc(len + 1);
            for (int i = 0; i < len; i++) res[i] = start[i];
            res[len] = '\0';
            if (*buf == '"') buf++;
            return make_token_string(res);
        }

        if (c == '&' && buf[1] == '&') { buf += 2; return make_token_operation(logical_and); }
        if (c == '|' && buf[1] == '|') { buf += 2; return make_token_operation(logical_or); }
        if (c == '=' && buf[1] == '=') { buf += 2; return make_token_operation(equals); }
        if (c == '!' && buf[1] == '=') { buf += 2; return make_token_operation(dont_equal); }

        switch (c) {
            case '(': buf++; return make_token_operation(left_paren);
            case ')': buf++; return make_token_operation(right_paren);
            case '{': buf++; return make_token_operation(left_brace);
            case '}': buf++; return make_token_operation(right_brace);
            case '[': buf++; return make_token_operation(left_quad_brace);
            case ']': buf++; return make_token_operation(right_quad_brace);
            case ',': buf++; return make_token_operation(comma);
            case '.': buf++; return make_token_operation(dot);
            case ':': buf++; return make_token_operation(colon);
            case '+': buf++; return make_token_operation(plus);
            case '-': buf++; return make_token_operation(minus);
            case '*': buf++; return make_token_operation(multiply);
            case '/': buf++; return make_token_operation(divide);
            case '^': buf++; return make_token_operation(bit_xor);
            case '~': buf++; return make_token_operation(bit_not);
            case '&': buf++; return make_token_operation(bit_and);
            case '|': buf++; return make_token_operation(bit_or);
            case '=': buf++; return make_token_operation(assign);
            case '<': buf++; return make_token_operation(lesser_than);
            case '>': buf++; return make_token_operation(greater_than);
            case '!': buf++; return make_token_operation(logical_not);
        }

        if (is_digit(c) || is_alpha(c) || c == '_') {
            char* start = buf;
            while (*buf && (is_digit(*buf) || is_alpha(*buf) || *buf == '_')) buf++;
            int len = (int)(buf - start);
            char* word = (char*)kalloc(len + 1);
            for (int i = 0; i < len; i++) word[i] = start[i];
            word[len] = '\0';
            return classify_word(word);
        }

        buf++;
    }
}

Token* lex(struct file* file) {
    static int loaded = 0;
    if (!loaded) {
        init_string(file);
        loaded = 1;
    }
    return do_lex();
}

INIT void init(void* _resolve_function(char* name)) {
    kalloc = _resolve_function("_alloc");
    printf = _resolve_function("_printf");
    print_color = _resolve_function("_print_color");
    open = _resolve_function("_open");
    struct file* foo = open('A',"./vmeste_mi_edynyi.zo");
    parse_verbose(foo);
    interpret(nodes);
    LOGI("WE REACHED I!");
   }



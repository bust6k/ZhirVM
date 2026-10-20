#include "vector.h"
#include "lexer.h"
#include "parse.h"
#include "builtins.h"

Value printb(Vector* args){
    Value none; none.kind = VAL_NONE; none.u = 0;

    if (!args || vec_len(args) < 1) return none;

    Value* fmtv = (Value*)vec_get(args, 0);
    if (fmtv->kind != VAL_STRING || !fmtv->s) return none;

    char* fmt = fmtv->s;
    int argi = 1;

    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] == '\\' && fmt[i + 1]) {
            i++;
            if (fmt[i] == 'n') {
                printf("%c", '\n');
            } else if (fmt[i] == '\\') {
                printf("%c", '\\');
            } else {
                printf("%c", '\\');
                printf("%c", fmt[i]);
            }
            continue;
        }

        if (fmt[i] != '%') {
            printf("%c", fmt[i]);
            continue;
        }

        i++;
        if (!fmt[i]) break;

        switch (fmt[i]){
            case 's':{
                if (argi < vec_len(args)) {
                    Value* v = (Value*)vec_get(args, argi);
                    if (v->kind == VAL_STRING && v->s)
                        printf("%s", v->s);
                    else if (v->kind == VAL_U32)
                        printf("%u", v->u);
                }
                argi++;
                break;
            }
            case 'c':{
                if (argi < vec_len(args)) {
                    Value* v = (Value*)vec_get(args, argi);
                    u32 chr = (v->kind == VAL_U32) ? v->u : 0;
                    printf("%c", chr & 0xFF);
                }
                argi++;
                break;
            }
            case 'u':{
                if (argi < vec_len(args)) {
                    Value* v = (Value*)vec_get(args, argi);
                    u32 num = (v->kind == VAL_U32) ? v->u : 0;
                    printf("%u", num);
                }
                argi++;
                break;
            }
            case 'x':{
                if (argi < vec_len(args)) {
                    Value* v = (Value*)vec_get(args, argi);
                    u32 num = (v->kind == VAL_U32) ? v->u : 0;
                    printf("%x", num);
                }
                argi++;
                break;
            }
            case '%':{
                printf("%c", '%');
                break;
            }

            default:{
                printf("%c", '%');
                printf("%c", fmt[i]);
                break;
            }
        }
    }

    return none;
}

#include "../kernel32/linker.c"
#include "lexer.c"
typedef ASTNode AN;

AN* make_ast() {
AN* node = (AN*)kalloc(sizeof(AN) * 1);
return node;
}

AN* make_ast_assign(char* name,AN* val) {
AN* n = make_ast();
n->type = AST_ASSIGN;
n->assign.name = name;
n->assign.value = val; 

return n;
}

AN* make_ast_nassign(AN* name,AN* val) {
AN* n = make_ast();
n->type = AST_EXPR_ASSIGN;
n->ast_assign.name = name;
n->ast_assign.value = val;

return n;
}

AN* make_ast_if(AN* cond,AN* then,AN* else_) {
AN* n = make_ast();
n->type = AST_IF;
n->if_node.cond = cond;
n->if_node.then = then;
n->if_node.else_ = else_;

return n;
}

AN* make_ast_while(AN* cond,AN* body) {
AN* n = make_ast();
n->type = AST_WHILE;
n->while_node.cond = cond;
n->while_node.body = body;

return n;
}

AN* make_ast_return(AN* value) {
AN* n = make_ast();
n->type = AST_RETURN;
n->return_node.value = value;

return n;
}

AN* make_ast_number(int value) {
AN* n = make_ast();
n->type = AST_NUMBER;
n->number.value = value;

return n;
}

AN* make_ast_float_number(char* float_num) {
AN* n = make_ast();
n->type = AST_FL_NUMBER;
n->float_number_node.float_num = float_num;

return n;
}

AN* make_ast_string(char* value) {
AN* n = make_ast();
n->type = AST_STRING;
n->string.value = value;

return n;
}

AN* make_ast_binop(char* op,AN* left,AN* right) {
AN* n = make_ast();
n->type = AST_BINOP;
n->binop.op = op;
n->binop.left = left;
n->binop.right = right;

return n;
}

AN* make_ast_paren(AN* expr) {
AN* n = make_ast();
n->type = AST_PAREN_OP;
n->paren_expr.expr = expr;

return n;
}

AN* make_ast_unop(char* op,AN* operand) {
AN* n = make_ast();
n->type = AST_UNOP;
n->unop.op = op;
n->unop.operand = operand;

return n;
}

AN* make_ast_func(char* ident,struct Vector* args,AN* body) {
AN* n = make_ast();
n->type = AST_FUNC;
n->func_node.ident = ident;
n->func_node.args = args;
n->func_node.body = body;

return n;
}

AN* make_ast_call(AN* name,struct Vector* args) {
AN* n = make_ast();
n->type = AST_CALL;
n->call_node.name = name;
n->call_node.args = args;

return n;
}

int is_type(struct file* file) {
Token* t = lex(file);
if(!t) return 0;
int k = t->keyword.kind;
if(k == u32_type) return 1;
if(k == u16_type) return 1;
if(k == u8_type) return 1;
if(k == bool_type) return 1;
if(k == float_type) return 1;
if(k == string_type) return 1;
return 0;
}

int is_keyword(struct file* file) {
Token* t = lex(file);
if(!t) return 0;
int k = t->keyword.kind;
if(k == if_keyword) return 1;
if(k == else_keyword) return 1;
if(k == return_keyword) return 1;
if(k == while_keyword) return 1;
if(k == break_keyword) return 1;
if(k == continue_keyword) return 1;
if(k == var_keyword) return 1;
if(k == varar_keyword) return 1;
if(k == udf_keyword) return 1;
if(k == class_keyword) return 1;
if(k == register_keyword) return 1;
if(k == goto_keyword) return 1;
if(k == const_keyword) return 1;
if(k == call_keyword) return 1;
return 0;
}

int is_string(struct file* file) {
Token* t = lex(file);
if(!t) return 0;
if(t->null.not_actual == NOT_ACTUAL) return 0;
if(t->string.val) return 1;
return 0;
}

int is_identifier(struct file* file) {
Token* t = lex(file);
if(!t) return 0;
if(t->null.not_actual == NOT_ACTUAL) return 0;
if(t->identifier.name && t->identifier.name[0]) return 1;
return 0;
}



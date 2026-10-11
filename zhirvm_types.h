#include "../lib/zhirtypes.h"
#include "../lib/string.h"
#include "vector.h"

typedef struct ASTNode {
  int type;
  
  union {
    struct {
      char* name;
      struct ASTNode* value;
    } assign;
    struct {
      struct ASTNode* name;
      struct ASTNode* value;
    } ast_assign;
    struct {
      struct ASTNode *cond, *then, *else_;
      int is_elif;
    } if_node;
    struct {
      struct ASTNode *cond, *body;
    } while_node;
    struct {
      char* ident;
      Vector* args;
      struct ASTNode* body;
    } func_node;
    struct {
      struct ASTNode* value;
    } return_node;
    struct {
      int value;
    } number;
    struct {
      char* value;
    } string;
    struct {
      char* op;
      struct ASTNode *left, *right;
    } binop;
    struct {
      struct ASTNode* expr;
    } paren_expr;
    struct {
      char* op;
      struct ASTNode* operand;
    } unop;
    struct {
      struct ASTNode* name;
      struct Vector* args;
    } call_node;
    struct {
    char* float_num;
    } float_number_node;
  };
} ASTNode;

typedef enum ASTNodeKind {
  AST_ASSIGN,
  AST_EXPR_ASSIGN,
  AST_IF,
  AST_WHILE,
  AST_RETURN,
  AST_NUMBER,
  AST_FL_NUMBER,
  AST_STRING,
  AST_BINOP,
  AST_PAREN_OP,
  AST_UNOP,
  AST_FUNC,
  AST_CALL
} ASTNodeKind;

typedef enum {
    if_keyword,
    else_keyword,
    identifier,
    return_keyword,
    while_keyword,
    break_keyword,
    continue_keyword,
    var_keyword,
    varar_keyword,
    udf_keyword,
    class_keyword,
    register_keyword,
    goto_keyword,
    const_keyword,
    call_keyword,
    logical_and,
    logical_or,
    logical_not,
    dont_equal,
    u32_type,
    u16_type,
    u8_type,
    bool_type,
    float_type,
    string_type,
    left_paren,
    right_paren,
    func_lparen,
    func_rparen,
    call_lparen,
    call_rparen,
    left_quad_brace,
    right_quad_brace,
    left_brace,
    right_brace,
    assign,
    equals,
    greater_than,
    lesser_than,
    plus,
    minus,
    multiply,
    divide,
    bit_xor,
    bit_not,
    bit_and,
    bit_or,
    comma,
    dot,
    colon,
    number
} ZhirVMTokens;

typedef struct Token {
    union {
        struct {
            int val;
        } number;

        struct {
            char* name;
        } identifier;

        struct {
            char* val;
        } string;

        struct {
            int kind;
        } keyword;
        //under operation means every operation(for example, and,or,xor....) and every action and sign(left_brace,left_paren,comma,etc..)
        struct {
            int op;
        } operation;
        //under null means that something went wrong during lexing
        struct {
            int not_actual;
        } null;
    };
} Token; 

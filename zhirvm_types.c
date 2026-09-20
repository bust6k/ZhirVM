#include "../lib/zhirtypes.h"
#include "../lib/string.h"

typedef struct ast_node {
  int type;
  
  union {
    struct {
      char* name;
      struct ast_node* value;
    } assign;
    struct {
      struct ast_node* name;
      struct ast_node* value;
    } ast_assign;
    struct {
      struct ast_node *cond, *then, *else_;
      int is_elif;
    } if_node;
    struct {
      struct ast_node *cond, *then, *else_;
    } ternary_node;
    struct {
      char* var;
      struct ast_node *start, *end, *step, *body;
    } for_node;
    struct {
      struct ast_node *cond, *body;
    } while_node;
    struct {
      char* ident;
      struct ast_node* args;
      struct ast_node* body;
      int arg_count;
    } func_node;
    struct {
      char* type;
      char* ident;
      struct ast_node* next;
    } func_arg;
    struct {
      char* cont_name;
      struct ast_node* func_type;
      char* ident;
      struct ast_node* next;
    } func_containter_arg;
    struct {
      struct ast_node* value;
    } return_node;
    struct {
      struct ast_node* value;
      struct ast_node* body;
      struct ast_node* next;
    } switch_case;
    struct {
      struct ast_node* expr;
      struct ast_node* cases;
      struct ast_node* default_body;
      size_t case_count;
    } switch_node;
    struct {
      char* name;
      int n;
    } var;
    struct {
      int value;
    } number;
    struct {
      char* value;
    } string;
    struct {
      char* op;
      struct ast_node *left, *right;
    } binop;
    struct {
      struct ast_node* expr;
    } paren_expr;
    struct {
      struct ast_node* expr;
      struct ast_node* value;
    } index;
    struct {
      struct ast_node* expr;
    } quad_expr;
    
    struct {
    struct ast_node* expr;
    struct ast_node* next;
    size_t count;
    } expr_list_node;

    struct {
      char* op;
      struct ast_node* operand;
    } unop;
    struct {
      struct ast_node* val;
      struct ast_node* next;
    } call_arg;
    struct {
      struct ast_node* name;
      struct ast_node* args;
      int arg_count;
      int magic_num; //this is the num for determinig if call_node is really unallocated and free for use
    } call_node;
    struct {
      struct ast_node* prev;
      struct ast_node* th;
    } switch_block_node;

    struct {
      struct ast_node* expr;
      struct ast_node* switch_blk_node;
    } case_stmt;
    struct {
      struct ast_node* switch_blk_node;
    } default_stmt;
    struct {
      struct ast_node* next;
      struct ast_node* stmt;
      int is_else;
    } block_node;
    struct {
      struct ast_node* stmt;
    } stmt_node;
    struct {
    struct ast_node* expr_list;
    } array_node;
    struct {
    char* float_num;
    } float_number_node;
  };
} ast_node;

typedef enum ast_node_kind {
  AST_ASSIGN,
  AST_EXPR_ASSIGN,
  AST_IF,
  AST_FOR,
  AST_WHILE,
  AST_TERNARY,
  AST_RETURN,
  AST_VARIP,
  AST_VAR,
  AST_VARN,
  AST_NUMBER,
  AST_FL_NUMBER,
  AST_STRING,
  AST_BINOP,
  AST_PAREN_OP,
  AST_QUAD_BRACE_OP,
  AST_INDEX,
  AST_COMMA_OP,
  AST_UNOP,
  AST_FUNC,
  AST_CALL,
  AST_CALL_ARG,
  AST_FUNC_ARG,
  AST_ARR_FUNC_ARG,
  AST_BREAK,
  AST_CONTINUE,
  AST_SWITCH,
  AST_SWITCH_BLOCK,
  AST_CASE,
  AST_CASE_RANGE,
  AST_DEFAULT,
  AST_CONST,
  AST_SIMPLE,
  AST_IMPORT,
  AST_INDICATOR,
  AST_STRATEGY,
  AST_STMT,
  AST_STMTS,
  AST_ARRAY,
  AST_EXPR_LIST
} ast_node_kind;

typedef enum {
    if_statement,
    else_statement,
    for_statement,
    step,
    to,
    by,
    identifier,
    return_statement,
    while_statement,
    break_statement,
    continue_statement,
    switch_statement,
    case_statement,
    default_statement,
    var_statement,
    varar_statement,
    udf_statement,
    class_statement,
    register_statement,
    goto_statement,
    const_statement,
    simple,
    logical_and,
    logical_or,
    logical_not,
    dont_equal,
    question_sign,
    bitwise_and,
    bitwise_or,
    bitwise_not,
    bitwise_shift_to_left,
    bitwise_shift_to_right,
    semicolon,
    import_statement,
    as,
    input_func,
    u32_type,
    u16_type,
    u8_type,
    number,
    bool_type,
    float_type,
    color_type,
    string_as_type,
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
    re_assign,
    equals,
    greater_than,
    lesser_than,
    plus_and_assign,
    minus_and_assign,
    multiply_and_assign,
    divide_and_assign,
    remind_and_assign,
    plus,
    minus,
    multiply,
    divide,
    xor,
    not,
    and,
    or,
    divide_with_remind,
    greater_than_or_equals,
    lesser_than_or_equals,
    bitwise_and_with_equals,
    bitwise_or_with_equals,
    bitwise_xor_with_equals,
    bitwise_not_with_equals,
    bitwise_shift_right_with_equals,
    bitwise_shift_left_with_equals,
    comma,
    dot,
    colon
} zhirvm_tokens;

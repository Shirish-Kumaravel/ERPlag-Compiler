#include "parser.c"

/*
GROUP_NO_34
Aditya Kanthi - 2020A7PS0087P
Shirish Kumaravel - 2020A7PS0131P
Arvind Ram - 2020A7PS1210P
Aryan Kapoor - 2020A7PS1689P 
*/

typedef enum aitem{
        _PROGRAM,
        _MODULE_DECLARATION,
        _MODULE_NODE,
        _DRIVER_MODULE,
        _MODULE,
        _ARRAY_DATA_TYPE,
        _RANGEOP,
        _PRINT_OUTPUT,
        _GET_INPUT,
        _SIGNED_NUM,
        _ARRAY_ELEMENT,
        _LVALUE_ID_STMT,
        _LVALUE_ARR_STMT,
        _MODULE_REUSE_STMT,
        _SIGNED_VALUE,
        _ASSIGNOP,
        _UNARYOP,
        _ARRAY_EXPR_INDEX,
        _SIGNED_EXPR_INDEX,
        _DECLARE_STMT,
        _SWITCH_STMT,
        _CASE_VALUE_STMTS,
        _DEFAULT_STMT,
        _FOR_LOOP,
        _WHILE_LOOP,
        _PLUS,
        _MINUS,
        _MUL,
        _DIV,
        _AND,
        _OR,
        _LE,
        _LT,
        _GE,
        _GT,
        _EQ,
        _NE,
        _NUM,
        _RNUM,
        _INTEGER,
        _REAL,
        _BOOLEAN,
        _TRUE_,
        _FALSE_,
        _LINKED_LIST_NODE,
        _CONDITIONAL_STMT,
        _ITERATIVE_STMT,
        _ID,
        _IF_STMT,
        _ARRAY_ITEM,
        _OUTPUT_LINKED_LIST_NODE,
        _INPUT_LINKED_LIST_NODE,
        _STATEMENT_LINKED_LIST_NODE,
        _PARENT_CASE_VALUE_STMT
}ASTItem;

char* ASTArr[] = {"PROGRAM",
        "MODULE_DECLARATION",
        "MODULE_NODE",
        "DRIVER_MODULE",
        "MODULE",
        "ARRAY_DATA_TYPE",
        "RANGEOP",
        "PRINT_OUTPUT",
        "GET_INPUT",
        "SIGNED_NUM",
        "ARRAY_ELEMENT",
        "LVALUE_ID_STMT",
        "LVALUE_ARR_STMT",
        "MODULE_REUSE_STMT",
        "SIGNED_VALUE",
        "ASSIGNOP",
        "UNARYOP",
        "ARRAY_EXPR_INDEX",
        "SIGNED_EXPR_INDEX",
        "DECLARE_STMT",
        "SWITCH_STMT",
        "CASE_VALUE_STMTS",
        "DEFAULT_STMT",
        "FOR_LOOP",
        "WHILE_LOOP",
        "PLUS",
        "MINUS",
        "MUL",
        "DIV",
        "AND",
        "OR",
        "LE",
        "LT",
        "GE",
        "GT",
        "EQ",
        "NE",
        "NUM",
        "RNUM",
        "INTEGER",
        "REAL",
        "BOOLEAN",
        "TRUE_",
        "FALSE_",
        "LINKED_LIST_NODE",
        "CONDITIONAL_STMT",
        "ITERATIVE_STMT",
        "ID",
        "_IF_STMT",
        "ARRAY_ITEM",
        "OUTPUT_LINKED_LIST_NODE",
        "INPUT_LINKED_LIST_NODE",
        " _STATEMENT_LINKED_LIST_NODE",
        "_PARENT_CASE_VALUE_STMT"
        }; 

typedef struct ast {
    ASTItem astItem;
    grammarItem inputSymbol;
    struct ast* parent;
    struct ast* leftmostChild;
    struct ast* nextSibling;
    int lineNo;
    char* lexeme;
    int intVal;
    float floatVal;  
    int scopeStart;
    int scopeEnd;  
}astNode;
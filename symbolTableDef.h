#include "AST.c"
/*
GROUP_NO_34
Aditya Kanthi - 2020A7PS0087P
Shirish Kumaravel - 2020A7PS0131P
Arvind Ram - 2020A7PS1210P
Aryan Kapoor - 2020A7PS1689P 
*/
#define gsize (30)
#define lsize (30)
typedef struct fxEntry function_entry;
typedef struct lSymbolItem{
    char* varName;
    char* scopMod;
    int scopeSt;
    int scopeEnd;
    ASTItem datatype;
    bool isArray;
    bool isStatic;
    bool isDynamic;
    int arrRanSt;
    int arrRanEnd;
    int width;
    int offset;
    int level;
    bool has_been_reassigned;
   struct lSymbolItem* next;
} identifier_entry;

typedef struct paraList{
    ASTItem datatype;
    char* lex;
    int arrSt;
    int arrEnd;
    bool isArr;
    bool isStatic;
    bool has_been_reassigned;
    int offset;
    int width;
    struct paraList* next;
}para_list;
typedef struct idHash {
    identifier_entry* ID_HASH[lsize];
} identifier_hash;

typedef struct localTable {
    identifier_hash* idTable;
    function_entry* function_head;
    struct localTable* parentTable;
    struct localTable* firstChildTable;
    struct localTable* nextSiblingTable;
} local_table;

struct fxEntry {
    char* func_name;
    local_table* local_table;
    para_list* ip_list;
    para_list* op_list;
    bool redeclaration_allowed_firstpass;
    bool redeclaration_allowed_secondpass;
    struct fxEntry* next;
    int scopeSt;
    int scopeEnd;
};

typedef struct functionHash{
    function_entry* functionHash[gsize];
}function_hash;


typedef struct globalTable {
    function_hash* functionTable;
    function_entry* driver_module;
} global_table;












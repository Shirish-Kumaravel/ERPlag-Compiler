#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/*
GROUP_NO_34
Aditya Kanthi - 2020A7PS0087P
Shirish Kumaravel - 2020A7PS0131P
Arvind Ram - 2020A7PS1210P
Aryan Kapoor - 2020A7PS1689P 
*/


#define NO_OF_KEYWORDS (36)
long long int buffer_size;
typedef enum gitem{
    iterativeStmt, default_stmt, value, caseStmts, caseStmts2, conditionalStmt, declareStmt,
    relationalOp, logicalOp, AnyTerm, abExpr, unary_op, non_term, U, 
    high_op, low_op, factor, factor2,ifStmt,insideIfStmts,elseStmt, exprIndex, exprIndex2, arrExpr, arrExpr2, arrTerm, arrTerm2, 
    arrFactor, term2, term, arithmeticExpr, arithmeticExpr2, expression, idList2, idList, 
    optional, moduleReuseStmt, lvalueARRStmt, lvalueIDStmt, sign, whichStmt, assignmentStmt, 
    simpleStmt, program, moduleDeclarations, moduleDeclaration, otherModules, driverModule, module,
    ret, input_plist, input_plist2, output_plist, output_plist2, dataType, arr_range, var_const,
    type, moduleDef, statements, statement,  ioStmt, print_var, boolConst, N1, arr_index, index2, actual_para_list, actual_para_list2, N7, N8, N9, K, for_index, for_index2, for_sign, for_range, INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM, GET_VALUE,
    PRINT, USE, WITH, PARAMETERS, TAKES, INPUT, RETURNS, FOR, IN, SWITCH, CASE, BREAK, DEFAULT, 
    WHILE, ID, NUM, RNUM, AND, OR, TRUE_, FALSE_, PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, 
    DEF, ENDDEF, DRIVERDEF, DRIVERENDDEF, COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, SQBO, SQBC, 
    BO, BC, IF, THEN, ELSE, ENDIF, PRINTANCESTOR,SUBRANGE,HEXNUM,SIZE, COMMENT,EPS,DUMMY
}grammarItem;

char* grammarArr[]={
    "iterativeStmt","default_stmt","value","caseStmts","caseStmts2","conditionalStmt","declareStmt",
    "relationalOp","logicalOp","AnyTerm","abExpr","unary_op","non_term","U",
    "high_op","low_op","factor","factor2","ifStmt","insideIfStmts","elseStmt","exprIndex", "exprIndex2","arrExpr","arrExpr2","arrTerm","arrTerm2",
    "arrFactor","term2","term","arithmeticExpr","arithmeticExpr2","expression","idList2","idList",
    "optional","moduleReuseStmt","lvalueARRStmt","lvalueIDStmt","sign","whichStmt","assignmentStmt",
    "simpleStmt","program","moduleDeclarations","moduleDeclaration","otherModules","driverModule","module",
    "ret","input_plist","input_plist2","output_plist","output_plist2","dataType","arr_range","var_const",
    "type","moduleDef","statements","statement"," ioStmt","print_var","boolConst"," N1"," arr_index","index2"," actual_para_list"," actual_para_list2"," N7"," N8","N9", "K", "for_index"," for_index2"," for_sign","for_range",
    "INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","PROGRAM","GET_VALUE",
    "PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH","CASE","BREAK","DEFAULT",
    "WHILE","ID","NUM","RNUM","AND","OR","TRUE_","FALSE_","PLUS","MINUS","MUL","DIV","LT","LE","GE","GT","EQ","NE",
    "DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL","COMMA","ASSIGNOP","SQBO","SQBC",
    "BO","BC","IF", "THEN", "ELSE", "ENDIF", "PRINTANCESTOR","SUBRANGE","HEXNUM","SIZE","COMMENT","EPS","DUMMY"
    };

typedef struct {
    grammarItem gItem;
    char *lexeme;
    int lineNum;
    int intVal;
    float floatVal;
} token;


typedef struct keyword_list{
    char* key;
    grammarItem token;
}keyword;

typedef struct hTableItem{
    char* keyword;
    grammarItem token_name;
    struct hTableItem* nextItem;
}hashTableItem;

hashTableItem* hashTable[NO_OF_KEYWORDS];
keyword keywords[NO_OF_KEYWORDS];

#define BUFFER_SIZE (1<<20)

typedef struct {
	char firstBuffer[BUFFER_SIZE];
	char secondBuffer[BUFFER_SIZE];
} twinBuff;

FILE *fp;
char *forwardPtr, *beginPtr;
bool firstBufferFlag, secondBufferFlag;
int lineNo;
bool finishedReading;
bool firstBufferLoadable = true;
bool secondBufferLoadable = true;
twinBuff twinBuffer;
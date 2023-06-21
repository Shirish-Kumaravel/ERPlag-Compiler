#include "lexerDef.h"

/*
GROUP_NO_34
Aditya Kanthi - 2020A7PS0087P
Shirish Kumaravel - 2020A7PS0131P
Arvind Ram - 2020A7PS1210P
Aryan Kapoor - 2020A7PS1689P 
*/

//hashTable functions
void initializeVal();
int hashVal(const char* s);
void printHashTable();
void populateHashTable();
grammarItem findKeyword(char* s);

//lexer functions
FILE* openFile(char *inputFile);
void clearBuffers(twinBuff tbuffer);
FILE* initialiseLexer(char* inputFile);
void reset();
void closeLexer();
void retract(int steps);
char* getLexeme();
FILE *getStream(FILE *fp);
char getNextCharacter(FILE* fp);
token errorHandle(int errCode, char* lex, int line);
token tokenize(char* lex, grammarItem g, int line);
token getNextToken(FILE* fp);
void removeComments(char *testcaseFile, char *cleanFile);




#include "lexerDef.h"

/*
GROUP_NO_34
Aditya Kanthi - 2020A7PS0087P
Shirish Kumaravel - 2020A7PS0131P
Arvind Ram - 2020A7PS1210P
Aryan Kapoor - 2020A7PS1689P 
*/

FILE* openFile(char *inputFile){
    FILE *fileptr = fopen(inputFile, "r");
    if(fileptr==NULL){
        printf("ERROR! File not opened.\n");
    }
    return fileptr;
}

void initializeVal(){
    //initialize array to null
    for(int i = 0; i<NO_OF_KEYWORDS; i++){
        hashTable[i] = NULL;
    }
    keyword table[] = {{"integer", INTEGER},
                {"real", REAL},
                {"boolean", BOOLEAN},
                {"of", OF},
                {"array", ARRAY},
                {"start", START},
                {"end", END},
                {"declare", DECLARE},
                {"module", MODULE},
                {"driver", DRIVER},
                {"program", PROGRAM},
                {"get_value", GET_VALUE},
                {"print", PRINT},
                {"use", USE},
                {"with", WITH},
                {"parameters", PARAMETERS},
                {"takes", TAKES},
                {"input", INPUT},
                {"returns", RETURNS},
                {"for", FOR},
                {"in", IN},
                {"switch", SWITCH},
                {"case", CASE},
                {"break", BREAK},
                {"default", DEFAULT},
                {"while", WHILE},
                {"AND",AND},
                {"OR",OR},
                {"true",TRUE_},
                {"false",FALSE_},
                {"if",IF},
                {"then",THEN},
                {"else",ELSE},
                {"endif",ENDIF},
                {"printAncestor",PRINTANCESTOR},
                {"subrange",SUBRANGE}

    };
    for(int i = 0;i<NO_OF_KEYWORDS;i++)
        keywords[i] = table[i];
}

int hashVal(const char* s) {
    long long p = 31, m = 1e9 + 7;
    long long hash = 0;
    long long p_pow = 1;
    int n = strlen(s);
    for(int i = 0; i < n; i++) {
       // hash = (hash + m + (s[i] - (int)'a' + 1) * p_pow) % m;
       // p_pow = (p_pow * p) % m;
        hash = (hash*31 + s[i])%m;
    }
    //return hash;
   return hash%NO_OF_KEYWORDS;
}

void printHashTable() {
    for(int i = 0;i < NO_OF_KEYWORDS;i++) {
         hashTableItem *curr = hashTable[i];
         printf("%d    ",i);
         while(curr != NULL) {
            printf("%s \n",curr->keyword);
            curr = curr->nextItem;
         }
    }
}
void populateHashTable(){
    //add keywords to hash Table
    for(int i = 0;i<NO_OF_KEYWORDS;i++){
        int val = hashVal(keywords[i].key);
        hashTableItem *curr = hashTable[val];
        hashTableItem *newItem;
        if(curr == NULL){
            //if table enry empty create new node
            newItem = (hashTableItem*) malloc(sizeof(hashTableItem));
            
            newItem->token_name = keywords[i].token;
            newItem->keyword = keywords[i].key;
            newItem->nextItem = NULL;
            hashTable[val] = newItem;
        }
        else{
            //reach end of linked list and add a new node
            while(curr->nextItem!=NULL)
                curr = curr->nextItem;

            newItem = (hashTableItem*) malloc(sizeof(hashTableItem));

            newItem->token_name = keywords[i].token;
            newItem->keyword = keywords[i].key;
            newItem->nextItem = NULL;

            curr->nextItem = newItem;
        }
        //printf("%s \n",newItem->keyword);
        //printHashTable();
    }

}

grammarItem findKeyword(char* s){
    int val = hashVal(s);
    hashTableItem *curr = hashTable[val];
    while(curr != NULL && curr->keyword != NULL ){
        if(strcmp(s,curr->keyword) == 0)
            return curr->token_name;
        curr = curr->nextItem;
    }
    return ID;
}

void clearBuffers(twinBuff tbuffer){
    memset(tbuffer.firstBuffer,0,sizeof(tbuffer.firstBuffer));
    memset(tbuffer.secondBuffer,0,sizeof(tbuffer.secondBuffer));
}

FILE* initialiseLexer(char* inputFile,long long int buff_size){
    buffer_size=buff_size;
    clearBuffers(twinBuffer);
    fp=openFile(inputFile);
    int size = fread(twinBuffer.firstBuffer, sizeof(char),buffer_size,fp);
    //printf("Size read into first buffer in initalise lexer (%d)\n",size);
    //printf("Last character read in the first read (%c)\n",twinBuffer.firstBuffer[size-1]);
    if(size<buffer_size){
        twinBuffer.firstBuffer[size]=EOF;
    }
     
    firstBufferLoadable=false;
    lineNo=1;
    finishedReading=false;
    beginPtr=twinBuffer.firstBuffer;
    forwardPtr=twinBuffer.firstBuffer;
    initializeVal();
    populateHashTable();
    return fp;
}

void initialiselexer(char* inputFile,long long int buf_size){
    initialiseLexer(inputFile,BUFFER_SIZE);
}

void reset(){
    beginPtr=forwardPtr;
}

void closeLexer(){
    fclose(fp);
}

void retract(int steps){
    bool forwardInFirst=(forwardPtr < (twinBuffer.firstBuffer+buffer_size) && forwardPtr >= twinBuffer.firstBuffer);
    if(forwardInFirst && ((forwardPtr-twinBuffer.firstBuffer)<steps)){
        steps = steps - (forwardPtr-twinBuffer.firstBuffer);
        forwardPtr = twinBuffer.secondBuffer + buffer_size - steps;
    }
    else if((!forwardInFirst)&&((forwardPtr-twinBuffer.secondBuffer)<steps)){
        steps = steps - (forwardPtr-twinBuffer.secondBuffer);
        forwardPtr = twinBuffer.firstBuffer + buffer_size - steps;
    }
    else forwardPtr=forwardPtr-steps;

}


char* getLexeme(){
    char* result;
    // printf("Inside get Lexeme {-------------\n");
    bool beginInFirst = (beginPtr < (twinBuffer.firstBuffer+buffer_size) && beginPtr >= twinBuffer.firstBuffer);
    bool forwardInFirst=(forwardPtr < (twinBuffer.firstBuffer+buffer_size) && forwardPtr >= twinBuffer.firstBuffer);;


    if((beginInFirst && forwardInFirst) || (!beginInFirst && !forwardInFirst)) {
        int size = (forwardPtr-beginPtr);
        result = (char*)malloc(size*sizeof(char));
        strncpy(result,beginPtr,size);
    }
    else if(beginInFirst && !forwardInFirst) {
        int sizeInFirst = ((twinBuffer.firstBuffer+buffer_size-1)-beginPtr)+1;
        int sizeInSecond = (forwardPtr-twinBuffer.secondBuffer);
        int size = sizeInFirst+sizeInSecond;
        result = (char*)malloc(sizeof(char)*size);
        strncpy(result,beginPtr,sizeInFirst);
        strncpy(result+sizeInFirst,twinBuffer.secondBuffer,sizeInSecond);
    }
    else {
        int sizeInFirst = (forwardPtr-twinBuffer.firstBuffer);
        int sizeInSecond = ((twinBuffer.secondBuffer+buffer_size-1)-beginPtr)+1;
        int size = sizeInFirst+sizeInSecond;
        result = (char*)malloc(sizeof(char)*size);
        strncpy(result,beginPtr,sizeInSecond);
        strncpy(result+sizeInSecond,twinBuffer.firstBuffer,sizeInFirst);
    }
    return result;
    
}

FILE *getStream(FILE *fp){
    //If the buffer is completed load character stream to next buffer from file

    //called with getNextChar()
    if(forwardPtr==twinBuffer.firstBuffer+buffer_size-1){
        if(secondBufferLoadable){
        int size = fread(twinBuffer.secondBuffer,sizeof(char),buffer_size,fp);
        //printf("Size loaded into second buffer in getStream (%d)\n",size);
        if(size<buffer_size){
            twinBuffer.secondBuffer[size]=EOF;
            //return fp;
        }
        }
        firstBufferLoadable=true;
        forwardPtr = twinBuffer.secondBuffer;
        secondBufferLoadable=false;
    }
    else if(forwardPtr==twinBuffer.secondBuffer+buffer_size-1){
        if(firstBufferLoadable){
        int size = fread(twinBuffer.firstBuffer,sizeof(char),buffer_size,fp);
        //printf("Size loaded into first buffer in getStream (%d)\n",size);
        if(size<buffer_size){
           // printf("size--%d--",size);
            twinBuffer.firstBuffer[size]=EOF;
            //return fp;
        }
        }
        secondBufferLoadable=true;
        forwardPtr = twinBuffer.firstBuffer;
        firstBufferLoadable=false;
    }
    else{
        forwardPtr++;
    }
  return fp;
}

char getNextCharacter(FILE* fp){
    char c = *forwardPtr;
    if(c==EOF)finishedReading=true;
    getStream(fp);
    return c;
}

token errorHandle(int errCode, char* lex, int line){
    token nextToken;
    nextToken.gItem=0;
    nextToken.lexeme=NULL;
    nextToken.intVal=0;
    nextToken.floatVal=0.00;
    nextToken.lineNum=line;
    if(errCode == -2)
        printf("\tLEXICAL ERROR at LINE %d: Unknown character %s\n",line,lex);
    else if(errCode == -3)
        printf("\tLEXICAL ERROR at LINE %d: Length of identifier more than 20 in %s\n",line,lex);
    else
        printf("\tLEXICAL ERROR at LINE %d: Wrong seqence of characters in %s\n",line,getLexeme());

    reset();
    return nextToken;
}

token tokenize(char* lex, grammarItem g, int line){
    token nextToken;
    nextToken.lexeme = lex;
    nextToken.lineNum = line;
    nextToken.gItem = g;
    nextToken.intVal = 0;
    nextToken.floatVal = 0.0;
    switch(g){
        case NUM:
            nextToken.intVal = atoi(lex);
            break;
        case RNUM:
            nextToken.floatVal = atof(lex);
            break;
        case ID:
            nextToken.gItem = findKeyword(nextToken.lexeme);
            if(nextToken.gItem == ID){
                if(strlen(nextToken.lexeme)>20){
                    return errorHandle(-3,lex,line);
                }
            }
            break;
        case HEXNUM:
            nextToken.intVal = 0;
            break;
    }
    reset();
    return nextToken;
}

token getNextToken(FILE* fp){
    reset();
    char c = getNextCharacter(fp);
    int dfaState = 0;
    token newTok;
    while(dfaState>=0){
        if(finishedReading){
            token dummytoken;
            //printf("----------Reached EOF-------------\n");
            dummytoken.gItem = DUMMY;
            dummytoken.lexeme = NULL;
            dummytoken.lineNum = lineNo;
            return dummytoken;
          //  return dummy token here
        }
        switch(dfaState){
            case 0:         
                switch(c){
                    case '\t':
                        reset();
                        c = getNextCharacter(fp);
                        //reset();
                        break;
                    case ' ':
                        reset();
                        c = getNextCharacter(fp);
                        break;
                    case '\r':
                        reset();
                        c = getNextCharacter(fp);
                        break;
                    case '\n':
                        reset();
                        lineNo++;
                        c = getNextCharacter(fp);
                        break;
                    case '+':
                        dfaState = 3;
                        break;
                    case '-':
                        dfaState = 4;
                        break;
                    case '/':
                        dfaState = 5;
                        break;
                    case ',':
                        dfaState = 6;
                        break;
                    case ';':
                        dfaState = 7;
                        break;
                    case '[':
                        dfaState = 8;
                        break;
                    case ']':
                        dfaState = 9;
                        break;
                    case '(':
                        dfaState = 10;
                        break;
                    case ')':
                        dfaState = 11;
                        break;
                    case '=':
                        dfaState = 12;
                        break;
                    case '>':
                        dfaState = 16;
                        break;
                    case '.':
                        dfaState = 31;
                        break;
                    case ':':
                        dfaState = 28;
                        break;
                    case '*':
                        dfaState = 33;
                        break;
                    case '<':
                        dfaState = 22;
                        break;
                    case '!':
                        dfaState = 14;
                        break;
                    default:
                        if(((c>='a' && c<='z') || (c>='A' && c<='Z') || c == '_')&&(c!='X'))
                            dfaState = 48;
                        else if(c=='X')
                            dfaState = 50;
                        else if(c=='@')
                            dfaState =53;
                        else if(c>='0' && c<='9')
                            dfaState = 38;
                        else
                        {
                            dfaState=-2;
                        }
                }
                break;
            case 3:
                return tokenize(getLexeme(),PLUS,lineNo);
                break;
            case 4:
                return tokenize(getLexeme(),MINUS,lineNo);
                break;
            case 5:
                return tokenize(getLexeme(),DIV,lineNo);
                break;
            case 6:
                return tokenize(getLexeme(),COMMA,lineNo);
                break;
            case 7:
                return tokenize(getLexeme(),SEMICOL,lineNo);
                break;
            case 8:
                return tokenize(getLexeme(),SQBO,lineNo);
                break;
            case 9:
                return tokenize(getLexeme(),SQBC,lineNo);
                break;
            case 10:
                return tokenize(getLexeme(),BO,lineNo);
                break;
            case 11:
                return tokenize(getLexeme(),BC,lineNo);
                break;
            case 12:
                c = getNextCharacter(fp);
                if(c == '=')
                    dfaState = 13;
                else{
                    retract(1);
                    dfaState = -4;
                }
                break;
            case 13:
                return tokenize(getLexeme(),EQ,lineNo);
                break;
            case 14:
                c = getNextCharacter(fp);
                if(c == '=')
                    dfaState = 15;
                else{
                    retract(1);
                    dfaState = -4;
                }
                break;
            case 15:
                return tokenize(getLexeme(),NE,lineNo);
                break;
            case 16:
                c = getNextCharacter(fp);
                if(c == '=')
                    dfaState = 17;
                else if(c == '>')
                    dfaState = 19;
                else{
                    retract(1);
                    dfaState = 18;
                }
                break;
            case 17:
                return tokenize(getLexeme(),GE,lineNo);
                break;
            case 18:
                return tokenize(getLexeme(),GT,lineNo);
                break;
            case 19:
                c = getNextCharacter(fp);
                if(c == '>')
                    dfaState = 20;
                else{
                    retract(1);
                    dfaState = 21;
                }
                break;
            case 20:
                return tokenize(getLexeme(),DRIVERENDDEF,lineNo);
                break;
            case 21:
                return tokenize(getLexeme(),ENDDEF,lineNo);
                break;
            case 22:
                c = getNextCharacter(fp);
                if(c == '<')
                    dfaState = 25;
                else if(c == '=')
                    dfaState = 24;
                else{
                    retract(1);
                    dfaState = 23;
                }
                break;
            case 23:
                return tokenize(getLexeme(),LT,lineNo);
                break;   
            case 24:
                return tokenize(getLexeme(),LE,lineNo);
                break; 
            case 25:
                c = getNextCharacter(fp);
                if(c == '<')
                    dfaState = 26;
                else{
                    retract(1);
                    dfaState = 27;
                }
                break;
            case 26:
                return tokenize(getLexeme(),DRIVERDEF,lineNo);
                break;
            case 27:
                return tokenize(getLexeme(),DEF,lineNo);
                break;  
            case 28:
                c = getNextCharacter(fp);
                if(c == '=')
                    dfaState = 29;
                else{
                    retract(1);
                    dfaState = 30;
                }
                break;
            case 29:
                return tokenize(getLexeme(),ASSIGNOP,lineNo);
                break; 
            case 30:
                return tokenize(getLexeme(),COLON,lineNo);
                break;
            case 31:
                c = getNextCharacter(fp);
                if(c == '.')
                    dfaState = 32;
                else{
                    retract(1);
                    dfaState = -4;
                }
                break;
            case 32:
                return tokenize(getLexeme(),RANGEOP,lineNo);
                break;
            case 33:
                c = getNextCharacter(fp);
                if(c == '*')
                    dfaState = 35;
                else{
                    retract(1);
                    dfaState = 34;
                }
                break;
            case 34:
                return tokenize(getLexeme(),MUL,lineNo);
                break;
            case 35:
                c = getNextCharacter(fp);
                if(c == '\n') lineNo++;
                if(c == '*')
                    dfaState = 36;
                
                
                break;
            case 36:
                c = getNextCharacter(fp);
                if(c == '*')
                    dfaState = 37;
                else{
                    dfaState = 35;
                }
                break;
            case 37: 
                return tokenize(getLexeme(),COMMENT,lineNo);
                break;
            case 38:
                c = getNextCharacter(fp);
                if(c == '.')
                    dfaState = 40;
                else if(c>='0' && c<='9'){
                    dfaState = 38;
                }
                // else if((c>='a' && c<='z') || (c>='A' && c<='Z')){
                //     dfaState = -4;
                // }
                else if(c == EOF){
                    dfaState = 39;
                    retract(1);
                    //finishedReading = 0;
                    return tokenize(getLexeme(),NUM,lineNo);
                }
                else{
                    retract(1);
                    dfaState = 39;
                }   
                break;
            case 39:
                //printf("in 39");
                return tokenize(getLexeme(),NUM,lineNo);
                break;
            case 40:
                c = getNextCharacter(fp);
                if(c == '.'){
                    retract(2);
                    dfaState = 41;
                }
                else if(c>='0' && c<='9'){
                    dfaState = 42;
                }
                else if(c=='e' || c=='E'){
                    dfaState = 44;
                }
                else{
                    retract(1);
                    dfaState = -4;
                }   
                break;
            case 41:
                return tokenize(getLexeme(),NUM,lineNo);
                break;
            case 42:
                c = getNextCharacter(fp);
                if(c == 'e' || c == 'E')
                    dfaState = 44;
                else if(c>='0' && c<='9'){
                    dfaState = 42;
                }
                // else if((c>='a' && c<='z') || (c>='A' && c<='Z')){
                //     dfaState = -4;
                // }
                else if(c == EOF){
                    //dfaState = 39;
                    retract(1);
                    //finishedReading = 0;
                    return tokenize(getLexeme(),RNUM,lineNo);
                }
                else{
                    retract(1);
                    dfaState = 43;
                }   
                break;
            case 43:
                return tokenize(getLexeme(),RNUM,lineNo);
                break;
            case 44:
                c = getNextCharacter(fp);
                if(c == '+' || c == '-')
                    dfaState = 45;
                else if(c>='0' && c<='9')
                    dfaState = 46;
                else
                    dfaState = -4;
                break;
            case 45:
                c = getNextCharacter(fp);
                if(c>='0' && c<='9')
                    dfaState = 46;
                else{
                    retract(1);
                    dfaState = -4;
                }
                break;
            case 46:
                c = getNextCharacter(fp);
                if(c>='0' && c<='9'){
                    dfaState = 46;
                }
                // else if((c>='a' && c<='z') || (c>='A' && c<='Z')){
                //     dfaState = -4;
                // }
                else if(c == EOF){
                    retract(1);
                    //finishedReading = 0;
                    return tokenize(getLexeme(),RNUM,lineNo);
                break;
                }
                else{
                    retract(1);
                    dfaState = 47;
                }
                break;
            case 47:             
                return tokenize(getLexeme(),RNUM,lineNo);
                break;
            case 48:
                //fflush(stdout);
                c = getNextCharacter(fp);
                // printf("%c",c);
                // fflush(stdout);
                if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || (c == '_')){
                    dfaState = 48;
                }
                else if(c == EOF){
                    //dfaState = 39;
                    retract(1);
                    //finishedReading = 0;
                    token curr = tokenize(getLexeme(),ID,lineNo);
                    if(curr.lexeme == NULL)
                        dfaState = -3;
                    return curr;
                }
                else{
                    //printf("Before retract");
                    retract(1);
                    //printf("After retract");
                    // printf("Lexeme over");
                    // fflush(stdout);
                    dfaState = 49;
                }
                break;
            case 49:
                newTok = tokenize(getLexeme(),ID,lineNo);
                if(newTok.lexeme == NULL)
                     dfaState = -3;
                return newTok;
                break;
            case 50:
                c = getNextCharacter(fp);
                if((c>='A' && c<='F') || (c>='0' && c<='9')){
                    dfaState = 51;
                }
                else if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || (c == '_')){
                    dfaState = 48;
                }
                break;
            case 51:
                //printf("Inside Case 51!\n");
                c = getNextCharacter(fp);
                if((c>='A' && c<='F') || (c>='0' && c<='9')){
                    dfaState = 51;
                }
                else if(c == EOF){
                    dfaState = 52;
                    retract(1);
                    //finishedReading = 0;
                    return tokenize(getLexeme(),HEXNUM,lineNo);
                }
                else{
                    retract(1);
                    dfaState = 52;
                }
                break;
            case 52:
                //printf("Inside Case 51!\n");
                return tokenize(getLexeme(),HEXNUM,lineNo);
                break;
            case 53:
                c = getNextCharacter(fp);
                if(c!='s'){
                    retract(1);
                    dfaState = -4;
                    break;
                }
                c = getNextCharacter(fp);
                if(c!='i'){
                    retract(1);
                    dfaState = -4;
                    break;
                }
                c = getNextCharacter(fp);
                if(c!='z'){
                    retract(1);
                    dfaState = -4;
                    break;
                }
                c = getNextCharacter(fp);
                if(c!='e'){
                    retract(1);
                    dfaState = -4;
                    break;
                }
                dfaState = 54;
                break;
            case 54:
                return tokenize(getLexeme(),SIZE,lineNo);
                break;
            default:
                dfaState = -2;

        }
        if(dfaState == -2){
            return errorHandle(-2,getLexeme(),lineNo);
        }

        if(dfaState == -4) {
            return errorHandle(-4,getLexeme(),lineNo);
        }
   
             
    }
}



void removeComments(char *testcaseFile, char *cleanFile){
    FILE *fpr = fopen(testcaseFile,"r");
    FILE *fpw = fopen(cleanFile,"w");

    if (!fpr || !fpw) {
        printf("File opening failed!\n");
        return;
    }

    int open = 0;
    int close = 0;
    int comment = 0;
    char c = getc(fpr);
    while(c!=EOF){
        if(comment == 1){
            if(c == '*'){
                if(close == 1){
                    close = 0;
                    comment = 0;
                }
                else
                    close = 1;
            }
            else{
                close = 0;
                if(c == '\n'){
                    putc('\n',fpw);
                    printf("\n");
                }
            }
        }
        else{
            if(c == '*'){
                if(open == 1){
                    open = 0;
                    comment = 1;
                }
                else
                    open = 1;
            }
            else{
                if(open == 1){
                    open = 0;
                    putc('*',fpw);
                    printf("*");
                }
                putc(c,fpw);
                printf("%c",c);
            }
        }
        c = getc(fpr);
    }
    if(open == 1){
        putc('*',fpw);
        printf("*");
    }
    fclose(fpr);
    fclose(fpw);

}

// int main(void){
//     removeComments("languageSample.txt","cleaned.txt");
//     initialiseLexer("languageSample.txt");
//     // char* array[]={"iterativeStmt"," default_stmt"," value"," caseStmt"," caseStmts"," conditionalStmt"," declareStmt","
//     // relationalOp"," logicalOp"," Term8"," AnyTerm"," Term7"," abExpr"," unary_op"," non_term"," U"," 
//     // high_op"," low_op"," factor"," factor2"," exprIndex"," arrExpr"," arrExpr2"," arrTerm"," arrTerm2"," 
//     // arrFactor"," term2"," term"," arithmeticExpr"," arithmeticExpr2"," expression"," idList2"," idList"," 
//     // optional"," moduleReuseStmt"," lvalueARRStmt"," lvalueIDStmt"," sign"," whichStmt"," assignmentStmt"," 
//     // simpleStmt"," program"," moduleDeclarations"," moduleDeclaraion"," otherModules"," driverModule"," module","
//     // ret"," input_plist"," input_plist2"," output_plist"," output_plist2"," dataType"," arr_range"," var_const","
//     // type"," moduleDef"," statements"," statement","  ioStmt"," print_var"," which_ID"," for_range"," 
//     // INTEGER"," REAL"," BOOLEAN"," OF"," ARRAY"," START"," END"," DECLARE"," MODULE"," DRIVER"," PROGRAM"," GET_VALUE","
//     // PRINT"," USE"," WITH"," PARAMETERS"," TAKES"," INPUT"," RETURNS"," FOR"," IN"," SWITCH"," CASE"," BREAK"," DEFAULT"," 
//     // WHILE"," ID"," NUM"," RNUM"," AND"," OR"," TRUE_"," FALSE_"," PLUS"," MINUS"," MUL"," DIV"," LT"," LE"," GE"," GT"," EQ"," NE"," 
//     // DEF"," ENDDEF"," DRIVERDEF"," DRIVERENDDEF"," COLON"," RANGEOP"," SEMICOL"," COMMA"," ASSIGNOP"," SQBO"," SQBC"," 
//     // BO"," BC"," COMMENT"};
//     token currToken;
//     int cnt = 0;
//     while(!finishedReading){
//     currToken = getNextToken(fp);
//     if(currToken.lexeme == NULL) {
//         continue;
//     }
//     cnt++;
//     // printf("-----\n");
//     // printf("Lexeme {%s}\n",currToken.lexeme);
//     // printf("Token {%d}\n",currToken.gItem);
//     // printf("Line Number {%d}\n",currToken.lineNum);
//     // if(currToken.gItem == 91)
//     //     printf("Float Value: %f\n",currToken.floatVal);
//     }
//     // printf("Hello\n");
//     printf("Num of tokens = %d\n",cnt);
//     return 0;
// }
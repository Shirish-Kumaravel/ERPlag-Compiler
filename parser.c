#include "parserDef.h"

/*
GROUP_NO_34
Aditya Kanthi - 2020A7PS0087P
Shirish Kumaravel - 2020A7PS0131P
Arvind Ram - 2020A7PS1210P
Aryan Kapoor - 2020A7PS1689P 
*/

token* toks;
int size;
int pp = 0;

void initializeParser(token* t,int s){
    toks = t;
    size = s;
}

grammarLHSInstance* addRuletoGrammar(int num, grammarLHSInstance* Grammar[],grammarItem lhsTerm, grammarItem rhsTerms[],int noRHSTerms){
    int i=0;
    grammarLHSInstance* newRuleHeadPtr = (grammarLHSInstance*)malloc(sizeof(struct lhsInstance));
    grammarRHSTerm* currGrammarRHSTerm = (grammarRHSTerm*)malloc(sizeof(struct rhsTerm));
    newRuleHeadPtr->nextLHSInstance=NULL;
    newRuleHeadPtr->lhsSymbol=lhsTerm;
    newRuleHeadPtr->firstRHSTerm = currGrammarRHSTerm;
    newRuleHeadPtr->grammarRuleNo = num;
    currGrammarRHSTerm->rhsSymbol = rhsTerms[i];
    i++;

    while(i<noRHSTerms){
        grammarRHSTerm* nextGrammarRHSTerm = (grammarRHSTerm*)malloc(sizeof(struct rhsTerm));
        nextGrammarRHSTerm->rhsSymbol=rhsTerms[i];
        currGrammarRHSTerm->nextTerm=nextGrammarRHSTerm;
        currGrammarRHSTerm= currGrammarRHSTerm->nextTerm;
        i++;
    }

    currGrammarRHSTerm->nextTerm=NULL;
    grammarLHSInstance* currRuleHeadPtr = Grammar[lhsTerm];
    while(currRuleHeadPtr!=NULL&&currRuleHeadPtr->nextLHSInstance){
        currRuleHeadPtr=currRuleHeadPtr->nextLHSInstance;
    }
    if(Grammar[lhsTerm]==NULL){
        Grammar[lhsTerm]=newRuleHeadPtr;
        return Grammar[lhsTerm];
    }
    else{
        currRuleHeadPtr->nextLHSInstance=newRuleHeadPtr;
    }
    return newRuleHeadPtr;   
}

void initializeGrammar(grammarLHSInstance* Grammar[]){
    for(int i=0;i<NO_OF_NONTERMINALS;i++){
        Grammar[i]=NULL;
    }
}

void printGrammar(grammarLHSInstance* grammar[]){
    for(int i=iterativeStmt;i<=for_range;i++){
         grammarLHSInstance* currRuleHead =  grammar[i];
         int ruleNo=1;
         printf("Grammar[%d]\n",i);
         printf("\t\\\n");
         printf("\t|\n");
         while(currRuleHead!=NULL){
            printf("\t|");
            printf("%d ) %s --->",ruleNo,grammarArr[currRuleHead->lhsSymbol]);            
            grammarRHSTerm* currRHSTerm = currRuleHead->firstRHSTerm;
            while(currRHSTerm){
                printf(" %s ",grammarArr[currRHSTerm->rhsSymbol]);
                currRHSTerm=currRHSTerm->nextTerm;
            }
            printf("\n");

            currRuleHead=currRuleHead->nextLHSInstance;
            ruleNo++;
         }
    }
}

void findFirst(grammarItem currElement);
void markSameExceptEpsilonFollowtoFollow(grammarItem source, grammarItem target);
void markSameExceptEpsilonFollowtoFollow(grammarItem source, grammarItem target);
void markSameExceptEpsilonFollow(grammarItem source,grammarItem target);
void printFirstSetForItem(grammarItem g);


void printFollowForThisTerm(grammarItem g);
// void *memset(void *ptr, int x, size_t n);

bool isTerminal(grammarItem g) {
    return (g >= INTEGER && g <= DUMMY);
}

void initializeFollowCalculated() {
    memset(followCalculated,false,NO_OF_NON_TERMINALS);
}

void initializeFirstCalculated(){
    memset(firstCalculated,false,
    NO_OF_NON_TERMINALS +NO_OF_NON_TERMINALS);

}
void initializeFirstForTerminals() {
    for(int i = INTEGER;i <= DUMMY; i++) {
        FirstSets[i-iterativeStmt][i-INTEGER] = true;
        firstCalculated[i-iterativeStmt] = true;
    }
}

void findFirstForNonTerminals(){
    for(int i = iterativeStmt;i <= for_range; i++) {
        if(!firstCalculated[i-iterativeStmt]) {
            findFirst(i);
        }
    }
}

void markSameExceptEpsilonFirst(grammarItem source,grammarItem target) {

    for(int i = INTEGER;i <= DUMMY; i++) {
        if(i == EPS) continue;
        if(FirstSets[source-iterativeStmt][i-INTEGER]) {
            FirstSets[target-iterativeStmt][i-INTEGER] = true;
        }
    }
}
void findFirst(grammarItem currElement) {
    //printf("In findFirst");
        grammarLHSInstance* currRule = grammar[currElement-iterativeStmt];
        while(currRule != NULL) {
            //printf("In currRule");
            grammarRHSTerm* currRhsElement = currRule->firstRHSTerm;
            bool canYieldEpsilon = true;

            while(currRhsElement != NULL) {
                int currIndex = (currRhsElement->rhsSymbol-iterativeStmt);
                if(!firstCalculated[currIndex]) {
                    findFirst(currRhsElement->rhsSymbol);
                }
                markSameExceptEpsilonFirst(currRhsElement->rhsSymbol,currElement);
                if(FirstSets[currIndex][EPS-INTEGER]) {
                    currRhsElement = currRhsElement->nextTerm;
                }
                else {
                    canYieldEpsilon = false;
                    break;
                }
            }
            if(canYieldEpsilon) {
                FirstSets[currElement-iterativeStmt][EPS-INTEGER] = true;
            }
            currRule = currRule->nextLHSInstance;
        }

        firstCalculated[currElement-iterativeStmt] = true;
}

void printFirst(){
    for(int i = iterativeStmt; i <= DUMMY; i++) {
        printFirstSetForItem(i);
        printf("\n");
    }
}

void printFirstSetForItem(grammarItem g) {
    printf("First set for %s \n",grammarArr[g-iterativeStmt]);
    for(int i = INTEGER;i <= DUMMY; i++) {
        if(FirstSets[g-iterativeStmt][i-INTEGER]) {
            //printf() Here we print the enum name using the mapping array
            printf("%s      ",grammarArr[i-iterativeStmt]);
        }
    }
}


void followForThisGrammarTerm(grammarItem g) {

    for(int i = iterativeStmt; i <= for_range; i++) { 
           
            grammarLHSInstance* ruleHead = grammar[i-iterativeStmt];
            while(ruleHead != NULL) {
                //printf("... \n");
                grammarRHSTerm* currElement = ruleHead->firstRHSTerm;
               // printf("Before entering loop \n");
                //fflush(stdout);

                while(currElement != NULL){
                   // printf("In loop \n");
                    //fflush(stdout);
                     if(((grammarItem)currElement->rhsSymbol) == g) {
                        // printf(" Printing G %s \n",grammarArr[g]);
                        // printf("Dealing with rules for lhs %s \n",grammarArr[i]);

                       // grammarRHSTerm* printer = currElement;
                      //  while(printer != NULL) {
                            //printf("%s \n",grammarArr[printer->rhsSymbol]);
                           // printer = printer->nextTerm;
                      //  }
                         
                            //printf("In mathcing element \n");
                            //fflush(stdout);
                          //  printf("In here");
                            grammarRHSTerm* pointer = currElement->nextTerm;
                            while(pointer != NULL && pointer->rhsSymbol != EPS) {
                               // printf("In pointer");
                               // fflush(stdout);
                                if(isTerminal(pointer->rhsSymbol)) {
                                   //printf("Adding %s to the follow set of %s",grammarArr[pointer->rhsSymbol],grammarArr[g]);
                                    FollowSets[g-iterativeStmt][(pointer->rhsSymbol)-INTEGER] = true;
                                    break;
                                }
                                else if(!FirstSets[pointer->rhsSymbol-iterativeStmt][EPS-INTEGER]) {

                                    markSameExceptEpsilonFollow(pointer->rhsSymbol,g);
                                    break;
                                }
                                else {
                                    markSameExceptEpsilonFollow(pointer->rhsSymbol,g);
                                    pointer = pointer->nextTerm;
                                }
                            }


                            if(pointer == NULL && (g != i)) {
                               // printf("Calling the follow Rule for %s as rhs and %s as lhs",grammarArr[g],grammarArr[i]);
                                if(!followCalculated[i-iterativeStmt]) {
                                    followForThisGrammarTerm(i);
                                }

                                markSameExceptEpsilonFollowtoFollow(i, g);

                            }

                     }

                     currElement = currElement->nextTerm;
                    }


                    ruleHead = ruleHead->nextLHSInstance;

            }


            }

            followCalculated[g-iterativeStmt] = true;

             //printFollowForThisTerm(iterativeStmt);
    }

    






void fillFollowForNonTerminals() {
   // printf("In Here \n");
    //fflush(stdout);
    for(int i = 0; i < NO_OF_NON_TERMINALS; i++) {
        for(int j = 0;j < NO_OF_TERMINALS; j++) {
            FollowSets[i][j] = false;
        }
    }
    FollowSets[program-iterativeStmt][DUMMY-INTEGER] = true;
    followCalculated[program-iterativeStmt] = true;
    
    for(int i = iterativeStmt; i<= for_range;i++) {
            if(!followCalculated[i-iterativeStmt]) {
              //  printf("FF %s \n",grammarArr[i]);
   // fflush(stdout);
                followForThisGrammarTerm(i);
                //printf("FG \n");
    //fflush(stdout);
            }
       
    }

}

void markSameExceptEpsilonFollow(grammarItem source,grammarItem target) {

    for(int i = INTEGER;i <= DUMMY; i++) {
        if(i == EPS) continue;
        if(FirstSets[source-iterativeStmt][i-INTEGER]) {
            //printf("Marking %s in the follow set of %s because of next element being %s \n",grammarArr[i],grammarArr[target],grammarArr[source]);
            FollowSets[target-iterativeStmt][i-INTEGER] = true;
        }
    }
}

void markSameExceptEpsilonFollowtoFollow(grammarItem source, grammarItem target){
    for(int i = INTEGER;i <= DUMMY; i++) {
        if(i == EPS) continue;
        if(FollowSets[source-iterativeStmt][i-INTEGER]) {
           //printf("Putting %s in the follow set of %s beacause of the LHS being %s \n",grammarArr[i],grammarArr[target],grammarArr[source]);
            FollowSets[target-iterativeStmt][i-INTEGER] = true;
        }
    }

}


void useThisFunctionForFirst(grammarLHSInstance* gram[]) {
    grammar = gram;
    initializeFirstCalculated();
    initializeFirstForTerminals();
    
    findFirstForNonTerminals();
    //findFollow

  //  printf("Here");

     //printFirst();
}
void printFollowForThisTerm(grammarItem g) {
    //printf("Printing all terminals");
    for(int i = INTEGER;i <= DUMMY; i++) {
        //printf()
        if(FollowSets[g-iterativeStmt][i-INTEGER] == true) {
           // printf("Here");
            printf("%s   ",grammarArr[i]);
        }
    }
}
void printFollow() {
    printf("Here");
    for(int i = iterativeStmt; i <= for_range; i++) {
            //printf("Follow set for %s     ",grammarArr[i]);
            printFollowForThisTerm(i);
            printf("\n");
    }
}


void useThisFunctionForFollow(grammarLHSInstance* gram[]) {
    //printf("Here in this fxn");
   // fflush(stdout);
    grammar = gram;
    initializeFollowCalculated();
    fillFollowForNonTerminals() ;
}

void fillGivenRule(grammarLHSInstance* lhs,grammarRHSTerm* currRule);
void fillParseTable(grammarLHSInstance* gram[]) {
    for(int i = iterativeStmt; i <= for_range; i++) {
            grammarLHSInstance* currHead = gram[i-iterativeStmt];
            while(currHead != NULL){
                fillGivenRule(currHead,currHead->firstRHSTerm);
                currHead = currHead->nextLHSInstance;
            }
    }
}

void fillGivenRule(grammarLHSInstance* lhs,grammarRHSTerm* currRule) {

    while(currRule != NULL) {

        for(int i = INTEGER; i <= DUMMY; i++) {
            if(i == EPS) continue;
            if(FirstSets[currRule->rhsSymbol-iterativeStmt][i-INTEGER])
                parseTable[lhs->lhsSymbol][i-INTEGER] = lhs;
        }

        if(!FirstSets[currRule->rhsSymbol][EPS-INTEGER]) return;
        currRule = currRule->nextTerm;
    }

    for(int i = INTEGER;i <= DUMMY; i++) {
        if(i == EPS) continue;
        if(FollowSets[lhs->lhsSymbol - iterativeStmt][i-INTEGER])
            parseTable[lhs->lhsSymbol][i-INTEGER] = lhs;
        
    }
}


void printParseTable(){
    int rows = NO_OF_NONTERMINALS;
    int cols = NO_OF_TERMINALS;
    printf("\nNON_TERMINAL\t\t\t\t\tTERMINAL\t\t\t\t\tPRODUCTION RULE");
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            printf("%s\t\t\t\t\t%s\t\t\t\t\t",grammarArr[i-iterativeStmt],grammarArr[j+INTEGER]);
            if(parseTable[i][j]==NULL) {
                printf("NO_RULE\n");
                continue;
            }
            printf("%s ---> ",grammarArr[(parseTable[i][j])->lhsSymbol]);
            grammarRHSTerm* currRHSTerm = (parseTable[i][j])->firstRHSTerm;
            while(currRHSTerm){
                printf(" %s ",grammarArr[currRHSTerm->rhsSymbol]);
                currRHSTerm=currRHSTerm->nextTerm;
            }
            printf("\n");
        }
    }
}

void useThisFunctionForParseTable(grammarLHSInstance* gram[]){
    grammar=gram;
    fillParseTable(grammar);
    //printParseTable();
}

void initialiseParseHelpers(){
    grammarLHSInstance* grammar[NO_OF_NONTERMINALS];
    initializeGrammar(grammar);

    //printf("Grammar initialized \n");
    //fflush(stdout);
    
    addRuletoGrammar(1,grammar, program, (grammarItem []){moduleDeclarations, otherModules, driverModule, otherModules}, 4);
    addRuletoGrammar(2,grammar, moduleDeclarations, (grammarItem []){moduleDeclaration, moduleDeclarations}, 2);
    addRuletoGrammar(3,grammar, moduleDeclarations, (grammarItem []){EPS}, 1);
    addRuletoGrammar(4,grammar, moduleDeclaration, (grammarItem []){DECLARE, MODULE, ID, SEMICOL}, 4);
    addRuletoGrammar(5,grammar, otherModules, (grammarItem []){module, otherModules}, 2);
    addRuletoGrammar(6,grammar, otherModules, (grammarItem []){EPS}, 1);
    addRuletoGrammar(7,grammar, driverModule, (grammarItem []){DRIVERDEF, DRIVER, PROGRAM, DRIVERENDDEF, moduleDef}, 5);
    addRuletoGrammar(8,grammar, module, (grammarItem []){DEF, MODULE, ID, ENDDEF, TAKES, INPUT, SQBO, input_plist, SQBC, SEMICOL, ret, moduleDef}, 12);
    addRuletoGrammar(9,grammar, ret, (grammarItem []){RETURNS, SQBO, output_plist, SQBC, SEMICOL}, 5);
    addRuletoGrammar(10,grammar, ret, (grammarItem []){EPS}, 1);
    addRuletoGrammar(11,grammar, input_plist, (grammarItem []){ID, COLON, dataType, input_plist2}, 4);
    addRuletoGrammar(12,grammar, input_plist2, (grammarItem []){COMMA, ID, COLON, dataType, input_plist2}, 5);
    addRuletoGrammar(13,grammar, input_plist2, (grammarItem []){EPS}, 1);
    addRuletoGrammar(14,grammar, output_plist, (grammarItem []){ID, COLON, type, output_plist2}, 4);
    addRuletoGrammar(15,grammar, output_plist2, (grammarItem []){COMMA, ID, COLON, type, output_plist2}, 5);
    addRuletoGrammar(16,grammar, output_plist2, (grammarItem []){EPS}, 1);
    addRuletoGrammar(17,grammar, dataType, (grammarItem []){INTEGER}, 1);
    addRuletoGrammar(18,grammar, dataType, (grammarItem []){REAL}, 1);
    addRuletoGrammar(19,grammar, dataType, (grammarItem []){BOOLEAN}, 1);
    addRuletoGrammar(20,grammar, dataType, (grammarItem []){ARRAY, SQBO, arr_range, SQBC, OF, type}, 6);
    addRuletoGrammar(21,grammar, arr_range, (grammarItem []){arr_index,RANGEOP,arr_index}, 3);
    addRuletoGrammar(22,grammar, type, (grammarItem []){INTEGER},1);
    addRuletoGrammar(23,grammar, type, (grammarItem []){REAL},1);
    addRuletoGrammar(24,grammar, type, (grammarItem []){BOOLEAN},1);
    addRuletoGrammar(25,grammar, moduleDef, (grammarItem []){START, statements, END}, 3);
    addRuletoGrammar(26,grammar, statements, (grammarItem []){statement, statements}, 2);
    addRuletoGrammar(27,grammar, statements, (grammarItem []){EPS}, 1);
    addRuletoGrammar(28,grammar, statement, (grammarItem []){ioStmt}, 1);
    addRuletoGrammar(29,grammar, statement, (grammarItem []){simpleStmt}, 1);
    addRuletoGrammar(30,grammar, statement, (grammarItem []){declareStmt}, 1);
    addRuletoGrammar(31,grammar, statement, (grammarItem []){conditionalStmt}, 1);
    addRuletoGrammar(32,grammar, statement, (grammarItem []){iterativeStmt}, 1);
    addRuletoGrammar(33,grammar, ioStmt, (grammarItem []){PRINT, BO, print_var, BC, SEMICOL}, 5);
    addRuletoGrammar(34,grammar, ioStmt, (grammarItem []){GET_VALUE, BO, ID, BC, SEMICOL}, 5);
    addRuletoGrammar(35,grammar, boolConst, (grammarItem []){TRUE_}, 1);
    addRuletoGrammar(36,grammar, boolConst, (grammarItem []){FALSE_}, 1);
    addRuletoGrammar(37,grammar, print_var, (grammarItem []){ID, N1}, 2);
    addRuletoGrammar(38,grammar, print_var, (grammarItem []){NUM}, 1);
    addRuletoGrammar(39,grammar, print_var, (grammarItem []){RNUM}, 1);
    addRuletoGrammar(40,grammar, print_var, (grammarItem []){boolConst}, 1);
    //addRuletoGrammar(grammar, all_Vars, (grammarItem []){ID}, 1);
    //addRuletoGrammar(grammar, all_Vars, (grammarItem []){NUM}, 1);
    //addRuletoGrammar(grammar, all_Vars, (grammarItem []){RNUM}, 1);
    //addRuletoGrammar(grammar, print_arr, (grammarItem []){ID,SQBO,arr_index,SQBC}, 4);
    addRuletoGrammar(41,grammar, N1, (grammarItem []){SQBO,sign,index2,SQBC}, 4);
    addRuletoGrammar(42,grammar, N1, (grammarItem []){EPS}, 1);
    addRuletoGrammar(43,grammar, simpleStmt, (grammarItem []){assignmentStmt}, 1);
    addRuletoGrammar(44,grammar, simpleStmt, (grammarItem []){moduleReuseStmt}, 1);
    addRuletoGrammar(45,grammar, assignmentStmt, (grammarItem []){ID, whichStmt}, 2);
    addRuletoGrammar(46,grammar, whichStmt, (grammarItem []){lvalueIDStmt}, 1);
    addRuletoGrammar(47,grammar, whichStmt, (grammarItem []){lvalueARRStmt}, 1);
    addRuletoGrammar(48,grammar, lvalueIDStmt, (grammarItem []){ASSIGNOP, expression, SEMICOL}, 3);
    addRuletoGrammar(49,grammar, lvalueARRStmt, (grammarItem []){SQBO, exprIndex, SQBC, ASSIGNOP, expression, SEMICOL},6);
    addRuletoGrammar(50,grammar, arr_index, (grammarItem []){sign,index2}, 2);
    addRuletoGrammar(51,grammar, index2, (grammarItem []){ID}, 1);
    addRuletoGrammar(52,grammar, index2, (grammarItem []){NUM}, 1);
    addRuletoGrammar(53,grammar, sign, (grammarItem []){EPS}, 1);
    addRuletoGrammar(54,grammar, sign, (grammarItem []){MINUS}, 1);
    addRuletoGrammar(55,grammar, sign, (grammarItem []){PLUS}, 1);
    addRuletoGrammar(56,grammar, moduleReuseStmt, (grammarItem []){optional, USE, MODULE, ID, WITH, PARAMETERS, actual_para_list, SEMICOL}, 8);
    // addRuletoGrammar(grammar, actual_para_list, (grammarItem []){NUM}, 1);
    // addRuletoGrammar(grammar, actual_para_list, (grammarItem []){RNUM}, 1);
    // addRuletoGrammar(grammar, actual_para_list, (grammarItem []){boolConst}, 1);
    // addRuletoGrammar(grammar, actual_para_list, (grammarItem []){ID,actual_para_list2}, 2);
    // addRuletoGrammar(grammar, actual_para_list2, (grammarItem []){SQBO,exprIndex,SQBC}, 3);
    // addRuletoGrammar(grammar, actual_para_list2, (grammarItem []){EPS}, 1);
    
    addRuletoGrammar(57,grammar, actual_para_list, (grammarItem []){sign,K, N9}, 3);
    addRuletoGrammar(58,grammar, N9, (grammarItem []){COMMA,sign, K, N9}, 4);
    addRuletoGrammar(59,grammar, N9, (grammarItem []){EPS}, 1);
    addRuletoGrammar(60,grammar, K, (grammarItem []){NUM}, 1);
    addRuletoGrammar(61,grammar, K, (grammarItem []){RNUM}, 1);
    addRuletoGrammar(62,grammar, K, (grammarItem []){boolConst}, 1);
    addRuletoGrammar(63,grammar, K, (grammarItem []){ID, actual_para_list2}, 2);
    addRuletoGrammar(64,grammar, actual_para_list2, (grammarItem []){SQBO,exprIndex,SQBC}, 3);
    addRuletoGrammar(65,grammar, actual_para_list2, (grammarItem []){EPS}, 1);

    addRuletoGrammar(66,grammar, optional, (grammarItem []){SQBO,idList,SQBC,ASSIGNOP}, 4);
    addRuletoGrammar(67,grammar, optional, (grammarItem []){EPS}, 1);
    addRuletoGrammar(68,grammar, idList, (grammarItem []){ID, idList2}, 2);
    addRuletoGrammar(69,grammar, idList2, (grammarItem []){COMMA, ID, idList2}, 3);
    addRuletoGrammar(70,grammar, idList2, (grammarItem []){EPS}, 1);
    addRuletoGrammar(71,grammar, expression, (grammarItem []){abExpr}, 1);
    addRuletoGrammar(72,grammar, expression, (grammarItem []){U}, 1);
    addRuletoGrammar(73,grammar, U, (grammarItem []){unary_op, non_term}, 2);
    addRuletoGrammar(74,grammar, non_term, (grammarItem []){BO, arithmeticExpr, BC}, 3);
    addRuletoGrammar(75,grammar, non_term, (grammarItem []){var_const}, 1);
    addRuletoGrammar(76,grammar, unary_op, (grammarItem []){PLUS}, 1);
    addRuletoGrammar(77,grammar, unary_op, (grammarItem []){MINUS}, 1);
    addRuletoGrammar(78,grammar, abExpr, (grammarItem []){AnyTerm, N7}, 2);
    addRuletoGrammar(79,grammar, N7,(grammarItem []){logicalOp, AnyTerm, N7}, 3);
    addRuletoGrammar(80,grammar, N7,(grammarItem []){EPS}, 1);
    addRuletoGrammar(81,grammar, AnyTerm,(grammarItem []){arithmeticExpr, N8}, 2);
    addRuletoGrammar(82,grammar, AnyTerm,(grammarItem []){boolConst}, 1);
    addRuletoGrammar(83,grammar, N8,(grammarItem []){relationalOp, arithmeticExpr}, 2);
    addRuletoGrammar(84,grammar, N8,(grammarItem []){EPS}, 1);
    addRuletoGrammar(85,grammar, var_const, (grammarItem []){ID}, 1);
    addRuletoGrammar(86,grammar, var_const, (grammarItem []){NUM}, 1);
    addRuletoGrammar(87,grammar, var_const, (grammarItem []){RNUM}, 1);
    addRuletoGrammar(88,grammar, arithmeticExpr, (grammarItem []){term, arithmeticExpr2}, 2);
    addRuletoGrammar(89,grammar, arithmeticExpr2, (grammarItem []){low_op, term, arithmeticExpr2}, 3);
    addRuletoGrammar(90,grammar, arithmeticExpr2, (grammarItem []){EPS}, 1);
    addRuletoGrammar(91,grammar, term, (grammarItem []){factor, term2}, 2);
    addRuletoGrammar(92,grammar, term2, (grammarItem []){EPS}, 1);
    addRuletoGrammar(93,grammar, term2, (grammarItem []){high_op, factor, term2}, 3);
    addRuletoGrammar(94,grammar, factor, (grammarItem []){BO, abExpr, BC}, 3);
    addRuletoGrammar(95,grammar, factor, (grammarItem []){NUM}, 1);
    addRuletoGrammar(96,grammar, factor, (grammarItem []){RNUM}, 1);
    addRuletoGrammar(97,grammar, factor, (grammarItem []){boolConst}, 1);
    addRuletoGrammar(98,grammar, factor, (grammarItem []){ID, factor2}, 2);
    addRuletoGrammar(99,grammar, factor2, (grammarItem []){EPS}, 1);
    addRuletoGrammar(100,grammar, factor2, (grammarItem []){SQBO, exprIndex, SQBC}, 3);
    addRuletoGrammar(101,grammar, exprIndex, (grammarItem []){sign, exprIndex2}, 2);
    addRuletoGrammar(102,grammar, exprIndex, (grammarItem []){arrExpr}, 1);
    addRuletoGrammar(103,grammar, exprIndex2, (grammarItem []){index2}, 1);
    addRuletoGrammar(104,grammar, exprIndex2, (grammarItem []){BO,arrExpr,BC}, 3);
    addRuletoGrammar(105,grammar, arrExpr, (grammarItem []){arrTerm, arrExpr2}, 2);
    addRuletoGrammar(106,grammar, arrExpr2, (grammarItem []){EPS}, 1);
    addRuletoGrammar(107,grammar, arrExpr2, (grammarItem []){low_op, arrTerm, arrExpr2}, 3);
    addRuletoGrammar(108,grammar, arrTerm, (grammarItem []){arrFactor, arrTerm2}, 2);
    addRuletoGrammar(109,grammar, arrTerm2, (grammarItem []){high_op, arrFactor, arrTerm2}, 3);
    addRuletoGrammar(110,grammar, arrTerm2, (grammarItem []){EPS}, 1);
    addRuletoGrammar(111,grammar, arrFactor, (grammarItem []){ID}, 1);
    addRuletoGrammar(112,grammar, arrFactor, (grammarItem []){NUM}, 1);
    addRuletoGrammar(113,grammar, arrFactor, (grammarItem []){boolConst}, 1);
    addRuletoGrammar(114,grammar, arrFactor, (grammarItem []){BO, arrExpr, BC}, 3);
    addRuletoGrammar(115,grammar, low_op, (grammarItem []){PLUS}, 1);
    addRuletoGrammar(116,grammar, low_op, (grammarItem []){MINUS}, 1);
    addRuletoGrammar(117,grammar, high_op, (grammarItem []){MUL}, 1);
    addRuletoGrammar(118,grammar, high_op, (grammarItem []){DIV}, 1);
    addRuletoGrammar(119,grammar, logicalOp, (grammarItem []){OR}, 1);
    addRuletoGrammar(120,grammar, logicalOp, (grammarItem []){AND}, 1);
    addRuletoGrammar(121,grammar, relationalOp, (grammarItem []){LT}, 1);
    addRuletoGrammar(122,grammar, relationalOp, (grammarItem []){LE}, 1);
    addRuletoGrammar(123,grammar, relationalOp, (grammarItem []){GT}, 1);
    addRuletoGrammar(124,grammar, relationalOp, (grammarItem []){GE}, 1);
    addRuletoGrammar(125,grammar, relationalOp, (grammarItem []){EQ}, 1);
    addRuletoGrammar(126,grammar, relationalOp, (grammarItem []){NE}, 1);
    addRuletoGrammar(127,grammar, declareStmt, (grammarItem []){DECLARE, idList, COLON, dataType, SEMICOL}, 5);
    addRuletoGrammar(128,grammar, conditionalStmt, (grammarItem []){SWITCH, BO, ID, BC, START, caseStmts,default_stmt, END}, 8);
    addRuletoGrammar(129,grammar,caseStmts, (grammarItem []){CASE, value, COLON, statements, BREAK, SEMICOL,caseStmts2},7);
    addRuletoGrammar(130,grammar,caseStmts2, (grammarItem []){CASE, value, COLON, statements, BREAK, SEMICOL,caseStmts2},7);
    addRuletoGrammar(131,grammar,caseStmts2, (grammarItem []){EPS}, 1);
    addRuletoGrammar(132,grammar, value, (grammarItem []){NUM}, 1);
    addRuletoGrammar(133,grammar, value, (grammarItem []){TRUE_}, 1);
    addRuletoGrammar(134,grammar, value, (grammarItem []){FALSE_}, 1);
    addRuletoGrammar(135,grammar, default_stmt, (grammarItem []){DEFAULT, COLON, statements, BREAK, SEMICOL}, 5);
    addRuletoGrammar(136,grammar, default_stmt, (grammarItem []){EPS}, 1);
    addRuletoGrammar(137,grammar, iterativeStmt, (grammarItem []){FOR, BO, ID, IN, for_range, BC, START, statements, END}, 9);
    addRuletoGrammar(138,grammar, iterativeStmt,(grammarItem []) {WHILE, BO, abExpr, BC, START, statements, END}, 7);
    addRuletoGrammar(139,grammar, for_range, (grammarItem []){for_index, RANGEOP, for_index},3);
    addRuletoGrammar(140,grammar, for_index, (grammarItem []){for_sign, for_index2},2);
    addRuletoGrammar(141,grammar, for_index2, (grammarItem []){NUM},1);
    addRuletoGrammar(142,grammar, for_sign, (grammarItem []){EPS},1);
    addRuletoGrammar(143,grammar, for_sign, (grammarItem []){MINUS},1);
    addRuletoGrammar(144,grammar, for_sign, (grammarItem []){PLUS},1);
    addRuletoGrammar(145,grammar, statement, (grammarItem []){ifStmt}, 1);
    addRuletoGrammar(146,grammar, ifStmt, (grammarItem []){IF,BO, abExpr,BC,THEN,insideIfStmts,elseStmt,ENDIF}, 8);
    addRuletoGrammar(147,grammar, insideIfStmts, (grammarItem []){START,statements,END}, 3);
    addRuletoGrammar(148,grammar, insideIfStmts, (grammarItem []){statement}, 1);
    addRuletoGrammar(149,grammar, elseStmt, (grammarItem []){ELSE,insideIfStmts}, 2);
    addRuletoGrammar(150,grammar, elseStmt, (grammarItem []){EPS}, 1);

    
    //printGrammar(grammar);
    //printf("All rules added \n");
    //fflush(stdout);
    useThisFunctionForFirst(grammar);
    //printf("First done \n");
    //fflush(stdout);
    useThisFunctionForFollow(grammar);
    //printf("FollowDone \n");
    //fflush(stdout);
    useThisFunctionForParseTable(grammar);      
}



treeNodePtr nextElement(treeNodePtr inp){
    treeNodePtr temp = inp;

    while(temp!=NULL){
        if(temp->nextSibling != NULL)
            return temp->nextSibling;
        temp = temp->parent;
    }
    return NULL;
}


token nextTokenFromInput(){
    token curr = toks[pp];
    if(pp<size-1)
        pp++;
    return curr;
}


void fillInfoAboutNode(treeNodePtr node,grammarItem inputSymbol,int lineNo,char* lexeme,int intVal,float floatVal,treeNodePtr leftmostChild,treeNodePtr nextSibling,treeNodePtr parent) {

    node->inputSymbol = inputSymbol;
    node->lineNo = lineNo;
    node->lexeme = lexeme;
    node->intVal = intVal;
    node->floatVal = floatVal;
    node->leftmostChild = leftmostChild;
    node->nextSibling = nextSibling;
    node->parent = parent;

}


treeNodePtr addChildrenForCurrRule(treeNodePtr parent,grammarRHSTerm* rhsRule) {
    if(rhsRule == NULL) return (treeNodePtr)NULL;
    treeNodePtr leftMostChild = (treeNodePtr)malloc(sizeof(treeNode));
    parent->leftmostChild = leftMostChild;
    fillInfoAboutNode(leftMostChild,rhsRule->rhsSymbol,-1,NULL,0,0.0,NULL,NULL,parent);
    treeNodePtr p = leftMostChild;
    rhsRule = rhsRule->nextTerm;
    while(rhsRule != NULL) {
        treeNodePtr curr = (treeNodePtr)malloc(sizeof(treeNode));
        p->nextSibling = curr;
        fillInfoAboutNode(curr,rhsRule->rhsSymbol,-1,NULL,0,0.0,NULL,NULL,parent);
        p = curr;
        rhsRule = rhsRule->nextTerm;
    }

    return leftMostChild;
}
treeNodePtr createParseTree(char* inputFile) {
    initialiselexer(inputFile,BUFFER_SIZE);

    treeNodePtr currNode = (treeNodePtr)malloc(sizeof(struct tree));
    treeNodePtr result = currNode;

    StackHead st = createStack();
    st = push(DUMMY,st);
    st = push(program,st);

    token a = nextTokenFromInput();
    grammarItem X = top(st);
    bool error = false;

    while(X != DUMMY) {
        if(isTerminal(X)) {
            if(X == a.gItem) {
                currNode->lineNo = a.lineNum;
                currNode->lexeme = a.lexeme;
                if(currNode->inputSymbol == NUM) {
                    currNode->intVal = a.intVal;
                }
                if(currNode->inputSymbol == RNUM) {
                    currNode->floatVal = a.floatVal;
                }

                st = pop(st);
                X = top(st);
                a = nextTokenFromInput();
                currNode = nextElement(currNode);
                error=false;
            }
            else {
               if(a.gItem == DUMMY) {
                   printf("Out of input characters to parse grammar");
                   errFlag=true;
                   return NULL;
               }
                if(!error){ 
                printf("\tSYNTAX ERROR AT LINE %d AND RECOVERY FROM ERROR SUCCESSFUL: MISSING TERMINAL %s\n" ,a.lineNum,grammarArr[X]);
                errFlag=true;
                }
                st= pop(st);
                X = top(st);
                currNode = nextElement(currNode);
                error=true;
            }
        }
        else {
            if(a.gItem == DUMMY) {
                if(FirstSets[X-iterativeStmt][EPS-INTEGER] == true) {
                    treeNodePtr endBranch = (treeNodePtr)malloc(sizeof(treeNode));
                    currNode->leftmostChild = endBranch;
                    endBranch->parent = currNode;
                    endBranch->inputSymbol = EPS;
                    currNode = nextElement(currNode);
                    st = pop(st);
                    X = top(st);
                    error=false;
                }
                else {
                    printf("Syntax error - Out of input chracters to parse grammar");
                    errFlag=true;
                    return NULL;
                }
            }
            else{
                    grammarLHSInstance* currRule = parseTable[X-iterativeStmt][a.gItem-INTEGER];
                    currNode->inputSymbol = X;       
                    st = pop(st);
                    if(currRule != NULL) {
                        currNode->grammarRuleNo = currRule->grammarRuleNo;
                        if(currRule->firstRHSTerm->rhsSymbol == EPS) {
                            treeNodePtr endBranch = (treeNodePtr)malloc(sizeof(treeNode));
                            currNode->leftmostChild = endBranch;
                            endBranch->parent = currNode;
                            endBranch->inputSymbol = EPS;
                            currNode = nextElement(currNode);
                            X = top(st);
                            continue;
                        }
                        addChildrenForCurrRule(currNode,currRule->firstRHSTerm);
                        StackHead temp = createStack();
                        int count = 0;
                        grammarRHSTerm* currElement = currRule->firstRHSTerm;
                        while(currElement != NULL) {
                        temp =  push(currElement->rhsSymbol,temp);
                            currElement = currElement->nextTerm;
                            count++;
                        }
                        for(int i = 0;i < count; i++) {
                            st = push(top(temp),st);
                            temp = pop(temp);
                        }
                        X = top(st);
                        currNode = currNode->leftmostChild;
                        error=false;
                    }
                    else {  
                        if(FollowSets[X-iterativeStmt][a.gItem-INTEGER]){
                            X = top(st);
                            currNode = nextElement(currNode);
                            if(!error)
                            printf("\tSYNTAX ERROR AT LINE %d AND RECOVERY FROM ERROR SUCCESSFUL: INVALID POSITION OF %s\n",a.lineNum,a.lexeme);
                            errFlag=true;
                            error=true;
                        }
                        else{
                            st = push(X,st);
                            
                            if(!error)
                            printf("\tSYNTAX ERROR AT LINE %d AND RECOVERY FROM ERROR SUCCESSFUL: INVALID POSITION OF %s\n",a.lineNum,a.lexeme);
                            a = nextTokenFromInput();
                            errFlag=true;
                            error=true;
                        }
                    }    
                    
            }
        }
  }

   if(a.gItem != DUMMY) {
       printf("SYNTAX ERROR! UNNECESSARY INPUT %s AT LINE %d\n",a.lexeme,a.lineNum);
       errFlag=true;
       return NULL;
   }

    return result;
}

bool hasNextSibling(treeNodePtr currNodePtr){
    if(currNodePtr->nextSibling) return true;
    else return false;
}

bool hasParent(treeNodePtr currNodePtr){
    if(currNodePtr->parent) return true;
    else return false;
}

bool hasChild(treeNodePtr currNodePtr){
    if(currNodePtr->leftmostChild) return true;
    else return false;
}

// void inOrderTraversal(treeNodePtr currNodePtr, int* countNodes, FILE* fp){
//     treeNodePtr tempNodePtr = (treeNodePtr)malloc(sizeof(struct tree));

    
//     if(currNodePtr!=NULL){
//         tempNodePtr = currNodePtr->leftmostChild;
//     }
//     else{return;}

//     if(hasChild(currNodePtr) && (currNodePtr->leftmostChild) -> nextSibling == NULL) {
//         inOrderTraversal(currNodePtr->leftmostChild,countNodes,fp);
//         // print details of currNodePtr node
//         if(hasChild(currNodePtr)){
//         printf("----");
//         fprintf(fp,"----");
//     }
//     else{
//         printf("%s",currNodePtr->lexeme);
//         fprintf(fp,"%s",currNodePtr->lexeme);
//     }
//     printf("\t%d\t%s",currNodePtr->lineNo,grammarArr[currNodePtr->inputSymbol]);
//     fprintf(fp,"\t%d\t%s",currNodePtr->lineNo,grammarArr[currNodePtr->inputSymbol]);
//     if(currNodePtr->inputSymbol == RNUM){
//         printf("\t%f",currNodePtr->floatVal);
//         fprintf(fp,"\t%f",currNodePtr->floatVal);
//     }
//     else if(currNodePtr->inputSymbol == NUM){
//         printf("\t%d",currNodePtr->intVal);
//         fprintf(fp,"\t%d",currNodePtr->intVal);
//     }
//     else{
//         printf("\tNOT_NUMBER");
//         fprintf(fp,"\tNOT_NUMBER");
//     }
//     if(hasParent(currNodePtr)){
//         printf("\t%s",grammarArr[(currNodePtr->parent)->inputSymbol]);
//         fprintf(fp, "\t%s",grammarArr[(currNodePtr->parent)->inputSymbol]);
//     }
//     else{
//        printf("\tROOT");
//        fprintf(fp,"\tROOT"); 
//     }
//     if(hasChild(currNodePtr)){
//         printf("\tNO\t%s\n",grammarArr[currNodePtr->inputSymbol]);
//         fprintf(fp,"\tNO\t%s\n",grammarArr[currNodePtr->inputSymbol]);
//     }
//     else{
//         printf("\tYES\tLEAF_NODE\n");
//         fprintf(fp,"\tYES\tLEAF_NODE\n");
//     }
//     *countNodes = *countNodes + 1;
//         return;
//     }
//     if(tempNodePtr!=NULL){
//         while(hasNextSibling(tempNodePtr)){
//             inOrderTraversal(tempNodePtr, countNodes, fp);
//             tempNodePtr=tempNodePtr->nextSibling;
//         }
//     }

//     if(hasChild(currNodePtr)){
//         printf("---- \t");
//         fprintf(fp,"---- \t");
//     }
//     else{
//         printf("%s \t",currNodePtr->lexeme);
//         fprintf(fp,"%s \t",currNodePtr->lexeme);
//     }
//     printf("%d %s \t",currNodePtr->lineNo,grammarArr[currNodePtr->inputSymbol]);
//     fprintf(fp,"%d %s \t",currNodePtr->lineNo,grammarArr[currNodePtr->inputSymbol]);
//     if(currNodePtr->inputSymbol == RNUM){
//         printf("%f \t",currNodePtr->floatVal);
//         fprintf(fp,"%f \t",currNodePtr->floatVal);
//     }
//     else if(currNodePtr->inputSymbol == NUM){
//         printf("%d \t",currNodePtr->intVal);
//         fprintf(fp,"%d \t",currNodePtr->intVal);
//     }
//     else{
//         printf("NOT_NUMBER");
//         fprintf(fp,"NOT_NUMBER");
//     }
//     if(hasParent(currNodePtr)){
//         printf("%s \t",grammarArr[(currNodePtr->parent)->inputSymbol]);
//         fprintf(fp, "%s \t",grammarArr[(currNodePtr->parent)->inputSymbol]);
//     }
//     else{
//        printf("ROOT \t");
//        fprintf(fp,"ROOT \t"); 
//     }
//     if(hasChild(currNodePtr)){
//         printf("NO %s \n",grammarArr[currNodePtr->inputSymbol]);
//         fprintf(fp,"NO %s \n",grammarArr[currNodePtr->inputSymbol]);
//     }
//     else{
//         printf("YES LEAF_NODE\n");
//         fprintf(fp,"YES LEAF_NODE\n");
//     }
//     *countNodes = *countNodes + 1;
//     inOrderTraversal(tempNodePtr, countNodes,fp);
// }

void inOrderTraversal2(treeNodePtr currNodePtr, int* countNodes, FILE* fp){
    if(currNodePtr == NULL) return;
    if(hasChild(currNodePtr)){
    treeNodePtr tempNodePtr = currNodePtr->leftmostChild;
    inOrderTraversal2(tempNodePtr,countNodes,fp);
    }
    if(hasChild(currNodePtr)){
        printf("----\t");
        fprintf(fp,"----\t");
    }
    else{
        printf("%s\t",currNodePtr->lexeme);
        fprintf(fp,"%s\t",currNodePtr->lexeme);
    }
    printf("%d\t %s\t",currNodePtr->lineNo,grammarArr[currNodePtr->inputSymbol]);
    fprintf(fp,"%d\t %s\t",currNodePtr->lineNo,grammarArr[currNodePtr->inputSymbol]);
    if(currNodePtr->inputSymbol == RNUM){
        printf("%f\t",currNodePtr->floatVal);
        fprintf(fp,"%f\t",currNodePtr->floatVal);
    }
    else if(currNodePtr->inputSymbol == NUM){
        printf("%d\t",currNodePtr->intVal);
        fprintf(fp,"%d\t",currNodePtr->intVal);
    }
    else{
        printf("NOT_NUMBER\t");
        fprintf(fp,"NOT_NUMBER\t");
    }
    if(hasParent(currNodePtr)){
        printf("%s\t",grammarArr[(currNodePtr->parent)->inputSymbol]);
        fprintf(fp, "%s\t",grammarArr[(currNodePtr->parent)->inputSymbol]);
    }
    else{
       printf("ROOT\t");
       fprintf(fp,"ROOT\t"); 
    }
    if(hasChild(currNodePtr)){
        printf("NO\t%s\n",grammarArr[currNodePtr->inputSymbol]);
        fprintf(fp,"NO\t%s\n",grammarArr[currNodePtr->inputSymbol]);
    }
    else{
        printf("YES\tLEAF_NODE\n");
        fprintf(fp,"YES\tLEAF_NODE\n");
    }
    // printf("Rule is %d\t",currNodePtr->ruleNo);
    // fprintf(fp,"Rule is %d\t",currNodePtr->ruleNo);
    *countNodes = *countNodes + 1;
    if(hasChild(currNodePtr)){
        treeNodePtr tempNodePtr = (currNodePtr->leftmostChild)->nextSibling;
        while(tempNodePtr!=NULL){
            inOrderTraversal2(tempNodePtr,countNodes,fp);
            tempNodePtr=tempNodePtr->nextSibling;
        }
    }


}

void inOrderTraversalForPTSize(treeNodePtr currNodePtr, int* countSpace){
    if(currNodePtr == NULL) return;
    if(hasChild(currNodePtr)){
    treeNodePtr tempNodePtr = currNodePtr->leftmostChild;
    inOrderTraversalForPTSize(tempNodePtr,countSpace);
    }
    *countSpace = *countSpace + sizeof(struct tree)/* + strlen(currNodePtr->lexeme)*sizeof(char)*/;
    if(currNodePtr->lexeme!=NULL){
        *countSpace= *countSpace + strlen(currNodePtr->lexeme)*sizeof(char);
    }
    if(hasChild(currNodePtr)){
        treeNodePtr tempNodePtr = (currNodePtr->leftmostChild)->nextSibling;
        while(tempNodePtr!=NULL){
            inOrderTraversalForPTSize(tempNodePtr,countSpace);
            tempNodePtr=tempNodePtr->nextSibling;
        }
    }


}

int printParseTree(treeNodePtr parseTree, char* outputfile){
    printf("\n\n--------------PRINTING PARSE TREE THROUGH INORDER TRAVERSAL---------------\n\n");
    FILE* outFile = fopen(outputfile,"w");
    int countNodes = 0;
    if(parseTree==NULL){
        printf("SYNTAX ERROR! PLEASE CORRECT.\n");
        fclose(outFile);
        return countNodes;
    }
    inOrderTraversal2(parseTree, &countNodes,outFile);
    fclose(outFile);
    return countNodes;
}


void printParseTreeVisualize(treeNodePtr node,int level){

    while (node != NULL)
        {
            for (int i = 0; i < level; i++){ 
                printf(" ");
            }
            printf("Level %d Node -> ", level);
            printf("%s ",grammarArr[node->inputSymbol]);
            printf("\n");
            if (node->leftmostChild != NULL)
            {
                for (int i = 0; i < level; i++) printf("  ");
                printf("Children: \n");
                printParseTreeVisualize(node->leftmostChild, level + 1);
            }
            node = node->nextSibling;
        }
}
 
void printParser(treeNodePtr tree){
    printParseTreeVisualize(tree,1);
}

int getParseTreeSize(treeNodePtr currNode){
    int countSpace=0;
    inOrderTraversalForPTSize(currNode,&countSpace);
    return countSpace;
}
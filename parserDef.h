#include "lexer.c"

#define NO_OF_NONTERMINALS 77
#define NO_OF_NON_TERMINALS 77
#define NO_OF_TERMINALS 67

/*
GROUP_NO_34
Aditya Kanthi - 2020A7PS0087P
Shirish Kumaravel - 2020A7PS0131P
Arvind Ram - 2020A7PS1210P
Aryan Kapoor - 2020A7PS1689P 
*/
bool errFlag = false;
typedef struct tree {
    grammarItem inputSymbol;
    int lineNo;
    char* lexeme;
    int intVal;
    float floatVal;
    int grammarRuleNo;
    struct tree* leftmostChild;
    struct tree* nextSibling;
    struct tree* parent;
} treeNode;

typedef struct tree* treeNodePtr;

typedef struct rhsTerm{
    grammarItem rhsSymbol;
    struct rhsTerm* nextTerm;
}grammarRHSTerm;

typedef struct rhsTerm* grammarRHSTermPtr;

typedef struct lhsInstance{
    grammarItem lhsSymbol;
    int grammarRuleNo;
    grammarRHSTerm* firstRHSTerm;
    struct lhsInstance* nextLHSInstance;
} grammarLHSInstance;

typedef struct lhsInstance* grammarLHSInstancePtr;
grammarLHSInstance** grammar;
bool firstCalculated[NO_OF_TERMINALS+NO_OF_NON_TERMINALS];
bool followCalculated[NO_OF_NON_TERMINALS];
bool FirstSets[NO_OF_TERMINALS + NO_OF_NONTERMINALS][NO_OF_TERMINALS];
bool FollowSets[NO_OF_NONTERMINALS][NO_OF_TERMINALS];
grammarLHSInstance* parseTable[NO_OF_NON_TERMINALS][NO_OF_TERMINALS];



typedef struct stack{
    grammarItem g;
    struct stack* next;
}Stack;

typedef struct head{
    Stack* stackele;
}StackHead;

StackHead createStack(){
    StackHead head ;
    head.stackele = NULL;
    // Stack* st = (Stack*)malloc(sizeof(Stack*));
    // st->g = DUMMY;
    // st->next = NULL;
    return head;
}

void printStack(StackHead head);

bool isStackEmpty(StackHead head){
    if(head.stackele == NULL)
        return true;
    return false;
}

StackHead push(grammarItem gitem,StackHead head){
    Stack* newNode = (Stack*)malloc(sizeof(Stack*));
    newNode->g = gitem;
    newNode->next = NULL;
    if(isStackEmpty(head)){
        head.stackele = newNode;
        return head;
    }
    Stack* temp = head.stackele;
    while(temp->next!=NULL)
        temp = temp->next;
    temp->next = newNode;
    return head;
}

StackHead pop(StackHead head){
    if(isStackEmpty(head)){
        printf("Stack element empty\n");
        return head;
    }
    Stack* last = head.stackele;
    if(last->next == NULL){
        head.stackele = NULL;
        return head;;
    }
    Stack* secLast = last;
    last = last->next;
    while(last->next!=NULL){
        last = last->next;
        secLast = secLast->next;
    }
    secLast->next = NULL;
    free(last);
    return head;
}


int top(StackHead head){
    Stack* temp = head.stackele;
    while(temp->next!=NULL){
        
        temp = temp->next;
    }
    return temp->g;

}

void printStack(StackHead head){
    
    if(isStackEmpty(head)){
        printf("Stack is Empty\n");
        return;
    }
    else
        printf("Printing stack elements.....\n");
     Stack* temp = head.stackele;
    while(temp->next != NULL){
        printf("Stack element is %d\n",temp->g);
        temp = temp->next;
    }
    printf("Stack element is %d\n",temp->g);
}

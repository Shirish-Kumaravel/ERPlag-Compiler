#include "ASTDef.h"

/*
GROUP_NO_34
Aditya Kanthi - 2020A7PS0087P
Shirish Kumaravel - 2020A7PS0131P
Arvind Ram - 2020A7PS1210P
Aryan Kapoor - 2020A7PS1689P 
*/

astNode* makeNode(int scopeStart,int scopeEnd, ASTItem aItem, astNode* parent, astNode* leftmostChild, astNode* nextSibling,treeNodePtr parseTreeNode){
    astNode* newNode = (astNode*) malloc(sizeof(astNode));
    newNode->scopeStart=scopeStart;
    newNode->scopeEnd=scopeEnd;
    newNode->astItem = aItem;
    newNode->parent = parent;
    newNode->leftmostChild = leftmostChild;
    newNode->nextSibling = nextSibling;
    newNode->inputSymbol = parseTreeNode->inputSymbol;
    newNode->lineNo = parseTreeNode->lineNo;
    newNode->lexeme = parseTreeNode->lexeme;
    newNode->intVal = parseTreeNode->intVal;
    newNode->floatVal = parseTreeNode->floatVal;
    return newNode;
}

void setScopes(treeNodePtr currPTNode,int* stScopePtr, int* endScopePtr){
    while(currPTNode->inputSymbol!=START){
        currPTNode = currPTNode -> nextSibling;
    }
    *stScopePtr = currPTNode->lineNo;
    currPTNode = currPTNode -> nextSibling;
    while(currPTNode->inputSymbol!=END){
        currPTNode = currPTNode -> nextSibling;
    }
    *endScopePtr = currPTNode->lineNo;
}

astNode* connectAST(astNode* childArr[],int size){
    astNode *firstChild,*curr;
    int i=0;
    while(childArr[i]==NULL && i<size){
        i++;
    }
    if(i==size) return NULL;
    firstChild=childArr[i];
    curr=firstChild;
    i++;
    while(i<size){
        if(childArr[i]==NULL) {i++;continue;}
        curr->nextSibling=childArr[i];
        curr=curr->nextSibling;
        i++;
    }
    return firstChild;
}

astNode* makeAST(int scopeStart,int scopeEnd, treeNodePtr parseTreeNode, astNode* inheritedAST, astNode* parent){
    if(parseTreeNode->grammarRuleNo==1){
        //<program> → <moduleDeclarations><otherModules(1)><driverModule><otherModules(2)>
            astNode* currNode = makeNode(scopeStart,scopeEnd,_PROGRAM,parent,NULL,NULL,parseTreeNode);

            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[4];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode=currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode=currPTNode->nextSibling;
            childArr[2] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode=currPTNode->nextSibling;
            childArr[3] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode); 
                       
            currNode->leftmostChild=connectAST(childArr,4);
            return currNode;
    }

    else if(parseTreeNode->grammarRuleNo==2){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_MODULE_DECLARATION,parent,NULL,NULL,parseTreeNode);

            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }

    else if(parseTreeNode->grammarRuleNo==3){
            return NULL;
    }

    else if(parseTreeNode->grammarRuleNo==4){
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            astNode* currNode = makeNode(scopeStart,scopeEnd,_ID,parent,NULL,NULL,currPTNode);
            return currNode;
    }

    //<otherModules(1)> →  <module><otherModules(2)>
    else if(parseTreeNode->grammarRuleNo==5){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_MODULE_NODE,parent,NULL,NULL,parseTreeNode);

            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }

    //<otherModules> →  ε	
    else if(parseTreeNode->grammarRuleNo==6){
            return NULL;
    }

    //<driverModule> → DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>	
    else if(parseTreeNode->grammarRuleNo==7){
        int scStart=0,scEnd=0;
        treeNodePtr tempNode = parseTreeNode;
        tempNode=tempNode->leftmostChild->nextSibling->nextSibling->nextSibling->nextSibling->leftmostChild;
        setScopes(tempNode,&scStart,&scEnd);
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeNode(scStart,scEnd,_DRIVER_MODULE,parent,NULL,NULL,currPTNode);
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        astNode* childArr = makeAST(scStart,scEnd,currPTNode,NULL,currNode);
        currNode->leftmostChild = childArr;
        return currNode;
    }

    //<module> → DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret><moduleDef>
    else if(parseTreeNode->grammarRuleNo==8){
        int scStart=0,scEnd=0;
        treeNodePtr tempNode = parseTreeNode;
        tempNode=tempNode->leftmostChild->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->nextSibling->leftmostChild;
        setScopes(tempNode,&scStart,&scEnd);
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeNode(scStart,scEnd,_MODULE,parent,NULL,NULL,currPTNode);
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        astNode* childArr[4];
        childArr[0] = makeNode(scStart,scEnd,_ID,currNode,NULL,NULL,currPTNode);
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        childArr[1] = makeAST(scStart,scEnd,currPTNode,NULL,currNode);
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        childArr[2] = makeAST(scStart,scEnd,currPTNode,NULL,currNode);
        currPTNode = currPTNode->nextSibling;
        childArr[3] = makeAST(scStart,scEnd,currPTNode,NULL,currNode);
        currNode->leftmostChild = connectAST(childArr,4);
        return currNode;
    }

    //<ret>  → RETURNS SQBO <output_plist> SQBC SEMICOL
    else if(parseTreeNode->grammarRuleNo==9){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    // <ret>  → ε	
    else if(parseTreeNode->grammarRuleNo==10){
        return NULL;
    }

    // <input_plist> → ID COLON <dataType> <input_plist2>	
    else if(parseTreeNode->grammarRuleNo==11){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_INPUT_LINKED_LIST_NODE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[3];
            childArr[0] = makeNode(scopeStart,scopeEnd,_ID,currNode,NULL,NULL,currPTNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode;
    }

    // 	<input_plist2(1)> → COMMA ID COLON <dataType> <input_plist2(2)> 	
    else if(parseTreeNode->grammarRuleNo==12){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_INPUT_LINKED_LIST_NODE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[3];
            currPTNode = currPTNode->nextSibling;
            childArr[0] = makeNode(scopeStart,scopeEnd,_ID,currNode,NULL,NULL,currPTNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode;
    }

    // <input_plist2>  → ε	
    else if(parseTreeNode->grammarRuleNo==13){
        return NULL;
    }

    // 	<output_plist> → ID COLON <type> <output_plist2>		
    else if(parseTreeNode->grammarRuleNo==14){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_OUTPUT_LINKED_LIST_NODE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[3];
            childArr[0] = makeNode(scopeStart,scopeEnd,_ID,currNode,NULL,NULL,currPTNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode;   
    }

    // 	<output_plist2(1)> → COMMA ID COLON <type> <output_plist2(2)>			
    else if(parseTreeNode->grammarRuleNo==15){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_OUTPUT_LINKED_LIST_NODE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[3];
            currPTNode = currPTNode->nextSibling;
            childArr[0] = makeNode(scopeStart,scopeEnd,_ID,currNode,NULL,NULL,currPTNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode; 
    }

    // <output_plist2>  → ε
    else if(parseTreeNode->grammarRuleNo==16){
        return NULL;
    }

    //<dataType> → INTEGER 	
    else if(parseTreeNode->grammarRuleNo==17){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_INTEGER,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<dataType> → REAL 	
    else if(parseTreeNode->grammarRuleNo==18){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_REAL,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<dataType> → BOOLEAN 	
    else if(parseTreeNode->grammarRuleNo==19){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_BOOLEAN,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<dataType> → ARRAY SQBO <arr_range> SQBC OF <type>	
    else if(parseTreeNode->grammarRuleNo==20){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_ARRAY_DATA_TYPE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }

    //<arr_range>→ <arr_index(1)>< RANGEOP <arr_index(2)>
    else if(parseTreeNode->grammarRuleNo==21){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_RANGEOP,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }

    //<type> → INTEGER 	
    else if(parseTreeNode->grammarRuleNo==22){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_INTEGER,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<type> → REAL 	
    else if(parseTreeNode->grammarRuleNo==23){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_REAL,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<type> → BOOLEAN 	
    else if(parseTreeNode->grammarRuleNo==24){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_BOOLEAN,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<moduleDef>  →START <statements> END	
    else if(parseTreeNode->grammarRuleNo==25){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<statements(1)>→ <statement> <statements(2)> 	
    else if(parseTreeNode->grammarRuleNo==26){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_STATEMENT_LINKED_LIST_NODE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,2);
            return currNode; 
    }

    //<statements>→  ε
    else if(parseTreeNode->grammarRuleNo==27){
        return NULL;
    }	

    // <statement> → <ioStmt>	
    else if(parseTreeNode->grammarRuleNo==28){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    // <statement> → <simpleStmt>	
    else if(parseTreeNode->grammarRuleNo==29){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    // <statement> → <declareStmt>	
    else if(parseTreeNode->grammarRuleNo==30){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    // <statement> → <conditionalStmt>	
    else if(parseTreeNode->grammarRuleNo==31){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    // <statement> → <iterativeStmt>	
    else if(parseTreeNode->grammarRuleNo==32){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    // <ioStmt> → PRINT BO <print_var> BC SEMICOL	
    else if(parseTreeNode->grammarRuleNo==33){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_PRINT_OUTPUT,parent,NULL,NULL,parseTreeNode);
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        astNode* childNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
        currNode->leftmostChild=childNode;
        return currNode;
    }

    //<ioStmt> → GET_VALUE BO ID BC SEMICOL
    else if(parseTreeNode->grammarRuleNo==34){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_GET_INPUT,parent,NULL,NULL,parseTreeNode);
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        astNode* childNode = makeNode(scopeStart,scopeEnd,_ID,currNode,NULL,NULL,currPTNode);
        currNode->leftmostChild=childNode;
        return currNode;
    }

    //<boolConst> →TRUE
    else if(parseTreeNode->grammarRuleNo==35){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_TRUE_,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<boolConst> →FALSE
    else if(parseTreeNode->grammarRuleNo==36){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_FALSE_,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    // <print_var> → ID <N1>
    else if(parseTreeNode->grammarRuleNo==37){
            /*treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
            currPTNode = currPTNode -> nextSibling;
            astNode* childArr[2];
            childArr[0] = inheritedAST;
            inheritedAST->parent = inhNode;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,inhNode);
            inhNode->leftmostChild= connectAST(childArr,2);
            currPTNode = currPTNode -> nextSibling;
            astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
            return currNode;*/
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeNode(scopeStart,scopeEnd,_ID,NULL,NULL,NULL,currPTNode);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
        
    }

    // <print_var> →  NUM	
    else if(parseTreeNode->grammarRuleNo==38){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_NUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }
    
    // <print_var> →  RNUM	
    else if(parseTreeNode->grammarRuleNo==39){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_RNUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    // <print_var> →  <boolConst>	
    else if(parseTreeNode->grammarRuleNo==40){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<N1> → SQBO<sign><index2> SQBC		
    else if(parseTreeNode->grammarRuleNo==41){
        astNode* signedNumNode = makeNode(scopeStart,scopeEnd,_SIGNED_NUM,NULL,NULL,NULL,parseTreeNode);
        astNode* childArr[2];
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode->nextSibling;
        childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,signedNumNode);
        currPTNode = currPTNode->nextSibling;
        childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,signedNumNode);
        signedNumNode->leftmostChild = connectAST(childArr,2);
        astNode* currNode = makeNode(scopeStart,scopeEnd,_ARRAY_ELEMENT,parent,NULL,NULL,parseTreeNode);
        astNode* childArr2[2];
        childArr2[0]=inheritedAST;
        inheritedAST->parent=currNode;
        childArr2[1]=signedNumNode;
        signedNumNode->parent=currNode;
        currNode->leftmostChild=connectAST(childArr2,2);
        return currNode;
    }

    //<N1> →  ε			
    else if(parseTreeNode->grammarRuleNo==42){
        return inheritedAST;
    }

    //<simpleStmt>  →<assignmentStmt> 	
    else if(parseTreeNode->grammarRuleNo==43){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<simpleStmt>  →<moduleReuseStmt> 	
    else if(parseTreeNode->grammarRuleNo==44){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<assignmentStmt> → ID <whichStmt>	 	
    else if(parseTreeNode->grammarRuleNo==45){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeNode(scopeStart,scopeEnd,_ID,NULL,NULL,NULL,currPTNode);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<whichStmt> →<lvalueIDStmt> 	
    else if(parseTreeNode->grammarRuleNo==46){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inheritedAST,parent);
        return currNode;
    }

    //<whichStmt> → <lvalueARRStmt>	 	
    else if(parseTreeNode->grammarRuleNo==47){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inheritedAST,parent);
        return currNode; 
    }

    //<lvalueIDStmt> → ASSIGNOP <expression> SEMICOL		
    else if(parseTreeNode->grammarRuleNo==48){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_LVALUE_ID_STMT,parent,NULL,NULL,parseTreeNode);
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode->nextSibling;
        astNode* childArr[2];
        childArr[0]=inheritedAST;
        inheritedAST->parent=currNode;
        childArr[1]= makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
        currNode->leftmostChild=connectAST(childArr,2);
        return currNode;
    }

    //<lvalueARRStmt> → SQBO <exprIndex> SQBC ASSIGNOP <expression> SEMICOL		
    else if(parseTreeNode->grammarRuleNo==49){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* arrayElementNode = makeNode(scopeStart,scopeEnd,_ARRAY_ELEMENT,NULL,NULL,NULL,parseTreeNode);
        astNode* childArr[2];
        childArr[0]=inheritedAST;
        inheritedAST->parent=arrayElementNode;
        currPTNode = currPTNode->nextSibling;
        childArr[1]=makeAST(scopeStart,scopeEnd,currPTNode,NULL,arrayElementNode);
        arrayElementNode->leftmostChild = connectAST(childArr,2);
        currPTNode = currPTNode->nextSibling;
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeNode(scopeStart,scopeEnd,_LVALUE_ARR_STMT,parent,NULL,NULL,parseTreeNode);
        astNode* childArr2[2];
        childArr2[0]=arrayElementNode;
        arrayElementNode->parent=currNode;
        childArr2[1]=makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
        currNode->leftmostChild = connectAST(childArr2,2);
        return currNode;
    }

    // 	<arr_index> → <sign><index2>
    else if(parseTreeNode->grammarRuleNo==50){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_SIGNED_NUM,parent,NULL,NULL,parseTreeNode);

            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }

    //<index2> →  ID	
    else if(parseTreeNode->grammarRuleNo==51){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_ID,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<index2> → NUM		 	
    else if(parseTreeNode->grammarRuleNo==52){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_NUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

     // <sign> →  ε		
    else if(parseTreeNode->grammarRuleNo==53){
        return NULL;
    }

    //<sign> → MINUS			
    else if(parseTreeNode->grammarRuleNo==54){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_MINUS,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<sign> → PLUS 			
    else if(parseTreeNode->grammarRuleNo==55){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_PLUS,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<moduleReuseStmt> → <optional> USE MODULE ID WITH PARAMETERS <actual_para_list> SEMICOL		
    else if(parseTreeNode->grammarRuleNo==56){
            astNode* inhNode = makeNode(scopeStart,scopeEnd,_MODULE_REUSE_STMT,NULL,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            treeNodePtr optNode = currPTNode;
            currPTNode = currPTNode -> nextSibling;
            currPTNode = currPTNode -> nextSibling;
            currPTNode = currPTNode -> nextSibling;
            astNode* childArr[2];
            childArr[0] = makeNode(scopeStart,scopeEnd,_ID,inhNode,NULL,NULL,currPTNode);
            currPTNode = currPTNode -> nextSibling;
            currPTNode = currPTNode -> nextSibling;
            currPTNode = currPTNode -> nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,inhNode);
            inhNode->leftmostChild = connectAST(childArr,2);
            astNode* currNode;
            currNode = makeAST(scopeStart,scopeEnd,optNode,inhNode,parent);
            return currNode;
    }

    //<actual_para_list> → <sign> <K> <N9>		
    else if(parseTreeNode->grammarRuleNo==57){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_SIGNED_VALUE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[3];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode; 
    }

    //<N9(1)> → COMMA <sign> <K> <N9(2)> 	 	
    else if(parseTreeNode->grammarRuleNo==58){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_SIGNED_VALUE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[3];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode; 
    }

    //<N9> →  ε		
    else if(parseTreeNode->grammarRuleNo==59){
        return NULL;
    }

    //<K> → NUM	 	
    else if(parseTreeNode->grammarRuleNo==60){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_NUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<K> → RNUM	 	
    else if(parseTreeNode->grammarRuleNo==61){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_RNUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<K> → <boolConst>	 	
    else if(parseTreeNode->grammarRuleNo==62){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<K> →  ID <actual_para_list2>		 	
    else if(parseTreeNode->grammarRuleNo==63){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeNode(scopeStart,scopeEnd,_ID,NULL,NULL,NULL,currPTNode);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<actual_para_list2> → SQBO <exprIndex> SQBC	 	
    else if(parseTreeNode->grammarRuleNo==64){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode -> nextSibling;      
        astNode* currNode = makeNode(scopeStart,scopeEnd,_ARRAY_ITEM,parent,NULL,NULL,parseTreeNode);
        astNode* childArr[2];
        childArr[0] = inheritedAST;
        inheritedAST->parent=currNode;
        childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
        currNode->leftmostChild = connectAST(childArr,2);
        return currNode;
    }

    //<actual_para_list2> →  ε		 	
    else if(parseTreeNode->grammarRuleNo==65){
        inheritedAST->parent = parent;
        return inheritedAST;
    }

    //<optional> → SQBO <idList> SQBC ASSIGNOP		 	
    else if(parseTreeNode->grammarRuleNo==66){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_ASSIGNOP,parent,NULL,NULL,parseTreeNode);
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode->nextSibling;
        astNode* childArr[2];
        childArr[0]=inheritedAST;
        inheritedAST->parent=currNode;
        childArr[1]= makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
        currNode->leftmostChild=connectAST(childArr,2);
        return currNode;
        
    }

    //<optional> → ε		 	
    else if(parseTreeNode->grammarRuleNo==67){
        inheritedAST->parent = parent;
        return inheritedAST;
    }

    //<idList> -> ID <idList2>		 	
    else if(parseTreeNode->grammarRuleNo==68){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_LINKED_LIST_NODE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeNode(scopeStart,scopeEnd,_ID,currNode,NULL,NULL,currPTNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,2);
            return currNode; 
    }

    //<idList2(1)> -> COMMA ID <idList2(2)> 			 	
    else if(parseTreeNode->grammarRuleNo==69){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_LINKED_LIST_NODE,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[2];
            childArr[0] = makeNode(scopeStart,scopeEnd,_ID,currNode,NULL,NULL,currPTNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,2);
            return currNode; 
    }

    //<idList2> ->  ε		 	
    else if(parseTreeNode->grammarRuleNo==70){
        return NULL;
    }

    //<expression> -> <abExpr>			 	
    else if(parseTreeNode->grammarRuleNo==71){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<expression> ->  <U>		 	
    else if(parseTreeNode->grammarRuleNo==72){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<U> -> <unary_op> <non_term>			 	
    else if(parseTreeNode->grammarRuleNo==73){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_UNARYOP,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }

    //<non_term> -> BO <arithmeticExpr> BC	 	
    else if(parseTreeNode->grammarRuleNo==74){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode -> nextSibling;      
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<non_term> ->  <var_const>		 	
    else if(parseTreeNode->grammarRuleNo==75){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<unary_op> → PLUS 		 	
    else if(parseTreeNode->grammarRuleNo==76){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_PLUS,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<unary_op> → MINUS		 	
    else if(parseTreeNode->grammarRuleNo==77){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_MINUS,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<abExpr> → <AnyTerm> <N7>	 	
    else if(parseTreeNode->grammarRuleNo==78){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<N7(1)> → <logicalOp><AnyTerm><N7(2)>		 	
    else if(parseTreeNode->grammarRuleNo==79){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode -> nextSibling;
        astNode* childArr[2];
        childArr[0] = inheritedAST;
        inheritedAST->parent = inhNode;
        childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,inhNode);
        inhNode->leftmostChild= connectAST(childArr,2);
        currPTNode = currPTNode -> nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<N7> →  ε		 	
    else if(parseTreeNode->grammarRuleNo==80){
        inheritedAST->parent = parent;
        return inheritedAST;
    }

    //<AnyTerm> → <arithmeticExpr> <N8> 			 	
    else if(parseTreeNode->grammarRuleNo==81){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<AnyTerm> →  <boolConst>		 	
    else if(parseTreeNode->grammarRuleNo==82){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<N8> → <relationalOp> <arithmeticExpr>		 	
    else if(parseTreeNode->grammarRuleNo==83){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        currPTNode = currPTNode -> nextSibling;
        astNode* childArr[2];
        childArr[0] = inheritedAST;
        inheritedAST->parent = currNode;
        childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
        currNode->leftmostChild= connectAST(childArr,2);
        return currNode;
    }

    //<N8> →  ε	 	
    else if(parseTreeNode->grammarRuleNo==84){
        inheritedAST->parent = parent;
        return inheritedAST;
    }

    //<var_const> → ID 			 	
    else if(parseTreeNode->grammarRuleNo==85){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_ID,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<var_const> → NUM 	
    else if(parseTreeNode->grammarRuleNo==86){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_NUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<var_const> → RNUM	 	
    else if(parseTreeNode->grammarRuleNo==87){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_RNUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<arithmeticExpr> → <term>  <arithmeticExpr2>		 	
    else if(parseTreeNode->grammarRuleNo==88){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<arithmeticExpr2(1)> → <low_op> <term>  <arithmeticExpr2(2)>			 	
    else if(parseTreeNode->grammarRuleNo==89){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode -> nextSibling;
        astNode* childArr[2];
        childArr[0] = inheritedAST;
        inheritedAST->parent = inhNode;
        childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,inhNode);
        inhNode->leftmostChild= connectAST(childArr,2);
        currPTNode = currPTNode -> nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;        
    }

    //<arithmeticExpr2> →  ε	 	
    else if(parseTreeNode->grammarRuleNo==90){
        inheritedAST->parent = parent;
        return inheritedAST;
    }

    //<term> → <factor> <term2>		 	
    else if(parseTreeNode->grammarRuleNo==91){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<term2> →  ε		 	
    else if(parseTreeNode->grammarRuleNo==92){
        inheritedAST->parent = parent;
        return inheritedAST;
    }

    //<term2(1)> → <high_op> <factor> <term2(2)> 		 	
    else if(parseTreeNode->grammarRuleNo==93){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode -> nextSibling;
        astNode* childArr[2];
        childArr[0] = inheritedAST;
        inheritedAST->parent = inhNode;
        childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,inhNode);
        inhNode->leftmostChild= connectAST(childArr,2);
        currPTNode = currPTNode -> nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;    
    }

    //<factor> → BO <abExpr> BC	 	
    else if(parseTreeNode->grammarRuleNo==94){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode -> nextSibling;      
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode; 
    }

    //<factor> -> NUM	 			 	
    else if(parseTreeNode->grammarRuleNo==95){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_NUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<factor> -> RNUM		
    else if(parseTreeNode->grammarRuleNo==96){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_RNUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<factor> → <boolConst>	 	
    else if(parseTreeNode->grammarRuleNo==97){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<factor> ->  ID <factor2>			 	
    else if(parseTreeNode->grammarRuleNo==98){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeNode(scopeStart,scopeEnd,_ID,NULL,NULL,NULL,currPTNode);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<factor2> -> ε				 	
    else if(parseTreeNode->grammarRuleNo==99){
        inheritedAST->parent = parent;
        return inheritedAST;
    }

    //<factor2> -> SQBO <exprIndex> SQBC 		 	
    else if(parseTreeNode->grammarRuleNo==100){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_ARRAY_ELEMENT,parent,NULL,NULL,parseTreeNode);
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode->nextSibling;
        astNode* childArr[2];
        childArr[0]=inheritedAST;
        inheritedAST->parent=currNode;
        childArr[1]= makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
        currNode->leftmostChild=connectAST(childArr,2);
        return currNode;
    }

    //<exprIndex> → <sign> <exprIndex2>		 	
    else if(parseTreeNode->grammarRuleNo==101){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_SIGNED_EXPR_INDEX,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }

    //<exprIndex> →  <arrExpr>			 	
    else if(parseTreeNode->grammarRuleNo==102){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;  
    }

    //<exprIndex2> → <index2>			 	
    else if(parseTreeNode->grammarRuleNo==103){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<exprIndex2> → BO <arrExpr> BC	 	
    else if(parseTreeNode->grammarRuleNo==104){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        currPTNode = currPTNode -> nextSibling;      
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<arrExpr> → <arrTerm> <arrExpr2>			 	
    else if(parseTreeNode->grammarRuleNo==105){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<arrExpr2> →  ε		
    else if(parseTreeNode->grammarRuleNo==106){
        inheritedAST->parent = parent;
        return inheritedAST;
    }

    //<arrExpr2(1)> → <low_op> <arrTerm> <arrExpr2(2)> 		 	
    else if(parseTreeNode->grammarRuleNo==107){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode -> nextSibling;
        astNode* childArr[2];
        childArr[0] = inheritedAST;
        inheritedAST->parent = inhNode;
        childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,inhNode);
        inhNode->leftmostChild= connectAST(childArr,2);
        currPTNode = currPTNode -> nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;    
    }

    //<arrTerm> → <arrFactor> <arrTerm2>			 	
    else if(parseTreeNode->grammarRuleNo==108){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode->nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
    }

    //<arrTerm2(1)> → <high_op> <arrFactor> <arrTerm2(2)> 				 	
    else if(parseTreeNode->grammarRuleNo==109){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* inhNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,NULL);
        currPTNode = currPTNode -> nextSibling;
        astNode* childArr[2];
        childArr[0] = inheritedAST;
        inheritedAST->parent = inhNode;
        childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,inhNode);
        inhNode->leftmostChild= connectAST(childArr,2);
        currPTNode = currPTNode -> nextSibling;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,inhNode,parent);
        return currNode;
                
    }

    //<arrTerm2> →  ε		 	
    else if(parseTreeNode->grammarRuleNo==110){
        inheritedAST->parent = parent;
        return inheritedAST;
    }

    //<arrFactor> → ID 		 	
    else if(parseTreeNode->grammarRuleNo==111){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_ID,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<arrFactor> → NUM			 	
    else if(parseTreeNode->grammarRuleNo==112){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_NUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<arrFactor> →  <boolConst>			 	
    else if(parseTreeNode->grammarRuleNo==113){
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
            return currNode;
    }

    //<arrFactor> → BO <arrExpr> BC		 	
    else if(parseTreeNode->grammarRuleNo==114){
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
            return currNode;
    }

    //<low_op> → PLUS	 			 	
    else if(parseTreeNode->grammarRuleNo==115){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_PLUS,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<low_op> → MINUS	 	
    else if(parseTreeNode->grammarRuleNo==116){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_MINUS,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<high_op> → MUL		 	
    else if(parseTreeNode->grammarRuleNo==117){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_MUL,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<high_op> → DIV		 	
    else if(parseTreeNode->grammarRuleNo==118){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_DIV,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<logicalOp>→ OR			 	
    else if(parseTreeNode->grammarRuleNo==119){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_OR,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<logicalOp>→ AND		 	
    else if(parseTreeNode->grammarRuleNo==120){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_AND,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<relationalOp>→  LT		 	
    else if(parseTreeNode->grammarRuleNo==121){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_LT,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<relationalOp>→  LE		 	
    else if(parseTreeNode->grammarRuleNo==122){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_LE,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<relationalOp>→  GT		 	
    else if(parseTreeNode->grammarRuleNo==123){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_GT,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<relationalOp>→  GE 	
    else if(parseTreeNode->grammarRuleNo==124){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_GE,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<relationalOp>→  EQ			 	
    else if(parseTreeNode->grammarRuleNo==125){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_EQ,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<relationalOp>→  NE	
    else if(parseTreeNode->grammarRuleNo==126){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_NE,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

        //<declareStmt>→  DECLARE <idList> COLON <dataType> SEMICOL		 	
    else if(parseTreeNode->grammarRuleNo==127){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_DECLARE_STMT,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }

    //<conditionalStmt>→ SWITCH BO ID BC START <caseStmts><default_stmt> END			 	
    else if(parseTreeNode->grammarRuleNo==128){
            int scStart=0,scEnd=0;
            setScopes(parseTreeNode->leftmostChild,&scStart,&scEnd);
            astNode* currNode = makeNode(scStart,scEnd,_CONDITIONAL_STMT,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[3];
            childArr[0] = makeNode(scopeStart,scopeEnd,_ID,currNode,NULL,NULL,currPTNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scStart,scEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scStart,scEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode;
    }

    //<caseStmts>→ CASE <value> COLON <statements> BREAK SEMICOL <caseStmts2>				 	
    else if(parseTreeNode->grammarRuleNo==129){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_PARENT_CASE_VALUE_STMT,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[3];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode;
    }

    //<caseStmts2(1)>→ CASE <value> COLON <statements> BREAK SEMICOL <caseStmts2(2)>		 	
    else if(parseTreeNode->grammarRuleNo==130){
            astNode* currNode = makeNode(scopeStart,scopeEnd,_CASE_VALUE_STMTS,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[3];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode;
    }

    //<caseStmts2>→ ε		 	
    else if(parseTreeNode->grammarRuleNo==131){
        return NULL;
    }

    //<value> → NUM			 	
    else if(parseTreeNode->grammarRuleNo==132){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_NUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<value> → TRUE			 	
    else if(parseTreeNode->grammarRuleNo==133){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_TRUE_,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<value> → FALSE			 	
    else if(parseTreeNode->grammarRuleNo==134){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_FALSE_,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }

    //<default_stmt> →DEFAULT COLON <statements> BREAK SEMICOL			 	
    else if(parseTreeNode->grammarRuleNo==135){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_DEFAULT_STMT,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = childArr;
            return currNode;
    }
    
    //<default_stmt> → ε				 	
    else if(parseTreeNode->grammarRuleNo==136){
        return NULL;
    }
    
    //<iterativeStmt>→ FOR BO ID IN <for_range> BC START <statements> END 				 	
    else if(parseTreeNode->grammarRuleNo==137){
            int scStart=0,scEnd=0;
            setScopes(parseTreeNode->leftmostChild,&scStart,&scEnd);
            astNode* currNode = makeNode(scStart,scEnd,_FOR_LOOP,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[3];

            childArr[0] = makeNode(scopeStart,scopeEnd,_ID,currNode,NULL,NULL,currPTNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scStart,scEnd,currPTNode,NULL,currNode);
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode;
    }
    
    //<iterativeStmt>→  WHILE BO <abExpr> BC START <statements> END				 	
    else if(parseTreeNode->grammarRuleNo==138){
            int scStart=0,scEnd=0;
            setScopes(parseTreeNode->leftmostChild,&scStart,&scEnd);
            astNode* currNode = makeNode(scStart,scEnd,_WHILE_LOOP,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scStart,scEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }
    
    //<for_range> → <for_index(1)> RANGEOP <for_index(2)>				 	
    else if(parseTreeNode->grammarRuleNo==139){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_RANGEOP,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }
    
    //<for_index> → <for_sign> <for_index2>				 	
    else if(parseTreeNode->grammarRuleNo==140){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_SIGNED_NUM,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* childArr[2];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);

            currNode->leftmostChild = connectAST(childArr,2);
            return currNode;
    }
    
    //<for_index2> → NUM				 	
    else if(parseTreeNode->grammarRuleNo==141){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_NUM,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }
    
    //<for_sign> →  ε				 	
    else if(parseTreeNode->grammarRuleNo==142){
        return NULL;
    }
    
    //<for_sign> → MINUS 				 	
    else if(parseTreeNode->grammarRuleNo==143){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_MINUS,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }
    
    //<for_sign> → PLUS 				 	
    else if(parseTreeNode->grammarRuleNo==144){
        astNode* currNode = makeNode(scopeStart,scopeEnd,_PLUS,parent,NULL,NULL,parseTreeNode->leftmostChild);
        return currNode;
    }
    
    //<statement> -> <ifStmt>
    else if(parseTreeNode->grammarRuleNo==145){
        treeNodePtr currPTNode = parseTreeNode->leftmostChild;
        astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
        return currNode;
    }

    //<ifStmt> -> if BO ... 
    else if(parseTreeNode->grammarRuleNo==146){
            int scStart=0,scEnd=0;
            setScopes(parseTreeNode,&scStart,&scEnd);
            astNode* currNode = makeNode(scStart,scEnd,_IF_STMT,parent,NULL,NULL,parseTreeNode);
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            astNode* childArr[3];
            childArr[0] = makeAST(scopeStart,scopeEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            currPTNode = currPTNode->nextSibling;
            childArr[1] = makeAST(scStart,scEnd,currPTNode,NULL,currNode);
            currPTNode = currPTNode->nextSibling;
            childArr[2] = makeAST(scStart,scEnd,currPTNode,NULL,currNode);
            
            currNode->leftmostChild = connectAST(childArr,3);
            return currNode;
    }

    else if(parseTreeNode->grammarRuleNo==147){
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
            return currNode;
    }

    else if(parseTreeNode->grammarRuleNo==148){
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
            return currNode;
    }

    else if(parseTreeNode->grammarRuleNo==149){
            treeNodePtr currPTNode = parseTreeNode->leftmostChild;
            currPTNode = currPTNode->nextSibling;
            astNode* currNode = makeAST(scopeStart,scopeEnd,currPTNode,NULL,parent);
            return currNode;
    }

    else if(parseTreeNode->grammarRuleNo==150){
            return NULL;
    }

}

void printASTNodes(astNode* node,int level){
    while (node != NULL)
        {
            for (int i = 0; i < level; i++) printf("  ");
            printf("Level %d Node -> ", level);
            printf("%s ",ASTArr[node->astItem]);
            printf(" Scope Start:%d, Scope End:%d ",node->scopeStart,node->scopeEnd);
            if(node->astItem == _ID || node->astItem == _NUM || node->astItem == _RNUM) printf(" %s ", node->lexeme);
            
            printf("\n");

            if (node->leftmostChild != NULL)
            {
                for (int i = 0; i < level; i++) printf("  ");
                printf("Children: \n");
                printASTNodes(node->leftmostChild, level + 1);
            }
            node = node->nextSibling;
        }
}

void inOrderTraversalAST(astNode* currNodePtr, int* countNodes){
    if(currNodePtr == NULL) return;
    if(currNodePtr->leftmostChild!=NULL){
    astNode* tempNodePtr = currNodePtr->leftmostChild;
    inOrderTraversalAST(tempNodePtr,countNodes);
    }
    printf("Node: %s\t\tLineNo:%d\t\tLexeme:%s\t\tINT_VAL:%d\t\tFLOAT_VAL:%f\n",ASTArr[currNodePtr->astItem],currNodePtr->lineNo,currNodePtr->lexeme,currNodePtr->intVal,currNodePtr->floatVal);
    *countNodes = *countNodes + 1;
    if(currNodePtr->leftmostChild!=NULL){
        astNode* tempNodePtr = (currNodePtr->leftmostChild);
        tempNodePtr = tempNodePtr->nextSibling;
        while(tempNodePtr!=NULL){
            inOrderTraversalAST(tempNodePtr,countNodes);
            tempNodePtr=tempNodePtr->nextSibling;
        }
    }
}

void inOrderTraversalASTSize(astNode* currNodePtr, int* countSpace){
    if(currNodePtr == NULL) return;
    if(currNodePtr->leftmostChild!=NULL){
    astNode* tempNodePtr = currNodePtr->leftmostChild;
    inOrderTraversalASTSize(tempNodePtr,countSpace);
    }
    *countSpace = *countSpace + sizeof(struct ast)/* + strlen(currNodePtr->lexeme)*sizeof(char)*/;
    if(currNodePtr->lexeme!=NULL){
        *countSpace= *countSpace + strlen(currNodePtr->lexeme)*sizeof(char);
    }
    if(currNodePtr->leftmostChild!=NULL){
        astNode* tempNodePtr = (currNodePtr->leftmostChild);
        tempNodePtr = tempNodePtr->nextSibling;
        while(tempNodePtr!=NULL){
            inOrderTraversalASTSize(tempNodePtr,countSpace);
            tempNodePtr=tempNodePtr->nextSibling;
        }
    }
}
 
// Wrapper over print2DUtil()
void printAST(astNode* ast)
{   

    // Pass initial space count as 0
    int countNodes=0;
    inOrderTraversalAST(ast,&countNodes);
    printf("Total Nodes in AST:%d\n",countNodes);
}

int getASTSize(astNode* ast){
    int countSpace = 0;
    inOrderTraversalASTSize(ast,&countSpace);
    return countSpace;
}
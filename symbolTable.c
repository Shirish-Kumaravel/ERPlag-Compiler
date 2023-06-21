#include "symbolTableDef.h"

/*
GROUP_NO_34
Aditya Kanthi - 2020A7PS0087P
Shirish Kumaravel - 2020A7PS0131P
Arvind Ram - 2020A7PS1210P
Aryan Kapoor - 2020A7PS1689P 
*/

global_table* GTABLE;
void global_first_pass(astNode* node,int scopeStart,int scopeEnd);
void global_second_pass(astNode* node,int scopeStart,int scopeEnd);
void insert_idtable(local_table* table,identifier_entry* id);

void initializeGlobalTable(global_table* g) {
    GTABLE = (global_table*)malloc(sizeof(global_table));
    GTABLE->functionTable = (function_hash*)(malloc(sizeof(function_hash)));
    GTABLE->driver_module = (function_entry*)(malloc(sizeof(function_entry)));
    for(int i = 0;i < gsize; i++){
        GTABLE->functionTable->functionHash[i] = NULL;
    }

}

void printList(para_list* list){
    if(list!=NULL){
        printf("%s\t%d\t",list->lex,list->datatype);
        if(list->isArr == true){
            printf("%d - %d\t",list->arrSt,list->arrEnd);
        }
        printList(list->next);
    }
}


void printGlobalTable(){
    fflush(stdout);
    for(int i = 0;i<gsize;i++){
        function_entry* curr = GTABLE->functionTable->functionHash[i]; 
        while(curr!=NULL){
            printf("%s\t",curr->func_name);
            printList(curr->ip_list);
            printf("\t");
            printList(curr->op_list);
            printf("\t");
            printf("%d\t%d\t",curr->redeclaration_allowed_firstpass,curr->redeclaration_allowed_secondpass);
            printf("\n");
            printf("\n");
            curr = curr->next;
        }
    }
    function_entry* curr = GTABLE->driver_module;
        printf("%s\t",curr->func_name);
            printList(curr->ip_list);
            printf("\t");
            printList(curr->op_list);
            printf("\t");
            printf("%d\t%d\t",curr->redeclaration_allowed_firstpass,curr->redeclaration_allowed_secondpass);
            printf("\n");
}

int getLevel(local_table* curr_level){
    int level = -1;
    while(curr_level!=NULL){
        curr_level = curr_level->parentTable;
        level++;
    }
    return level;
}

void printLocalArray(local_table* table){
    if(table == NULL)return;
    int lev = getLevel(table);
    for(int j = 0;j<lsize;j++){
            identifier_entry* temp = table->idTable->ID_HASH[j];
            
            while(temp != NULL){
            char* arr = "NO";
            char* stat = "..";
            if(temp->isArray == false)
                arr = "NO";
            else{
                if(temp->isStatic == true)
                    stat = "static";
            }
            if(temp->isDynamic == true)
                stat = "dynamic";
            
            if(temp->isArray == true)
                printf("%-10s [%d-%d%-10s %-15s %-15s %-10s %-15s[%d-%d%-10s %-10d %-10d %-10d \n ",temp->scopMod,temp->scopeSt,temp->scopeEnd,"]",temp->varName,ASTArr[temp->datatype],"YES",stat,temp->arrRanSt,temp->arrRanEnd,"]",temp->width,temp->offset,lev);
                temp = temp->next;
        }
        }
        printLocalArray(table->nextSiblingTable);
        printLocalArray(table->firstChildTable);
}


void printLocalTable(local_table* table){
    if(table == NULL)return;
    int lev = getLevel(table);
    for(int j = 0;j<lsize;j++){
            identifier_entry* temp = table->idTable->ID_HASH[j];
            
            while(temp != NULL){
            char* arr = "NO";
            char* stat = "..";
            if(temp->isArray == false)
                arr = "NO";
            else{
                if(temp->isStatic == true)
                    stat = "static";
            }
            if(temp->isDynamic == true)
                stat = "dynamic";
            
            if(temp->isArray == true){
                printf("%-10s %-15s[%d-%d%-10s %-15s %-10s %-15s[%d-%d%-10s %-10d %-10d %-10d \n ",temp->varName,temp->scopMod,temp->scopeSt,temp->scopeEnd,"]",ASTArr[temp->datatype],"YES",stat,temp->arrRanSt,temp->arrRanEnd,"]",temp->width,temp->offset,lev);
                // printf("Has been reassigned - %d\n",temp->has_been_reassigned);
            }
            else{
                printf("%-10s %-15s[%d-%d%-10s %-15s %-10s %-15s %-15s %-10d %-10d %-10d \n ",temp->varName,temp->scopMod,temp->scopeSt,temp->scopeEnd,"]",ASTArr[temp->datatype],arr,stat,"..",temp->width,temp->offset,lev);
                // printf("Has been reassigned - %d\n",temp->has_been_reassigned);
            }
                temp = temp->next;
        }
        }
        printLocalTable(table->nextSiblingTable);
        printLocalTable(table->firstChildTable);
}

int calcSize(local_table* table){
    int size = 0;
    if(table ==NULL)
        return 0;
    for(int i = 0;i<lsize;i++){
        if(table->idTable->ID_HASH[i]!=NULL){
            identifier_entry* temp = table->idTable->ID_HASH[i];
            while(temp!=NULL){
                size = size+temp->width;
                temp = temp->next;
            }
        }
    }
    return size+calcSize(table->nextSiblingTable)+calcSize(table->firstChildTable);
}

int listSize(para_list* list){
    if(list == NULL)
        return 0;
    return list->width+listSize(list->next);
}

void printModuleSizes(){
    printf("%-15s %-15s \n ", "Module Name","Module size");
    for(int i = 0;i<gsize;i++){
        int size = 0;
        function_entry* curr = GTABLE->functionTable->functionHash[i];
        if(curr == NULL) continue;
        size = listSize(curr->ip_list)+listSize(curr->op_list);
        for(int j = 0;j<lsize;j++){
            size = size+calcSize(curr->local_table);
        }
        printf("%-15s %-15d \n",curr->func_name,size);
    }
}

void printArrays(){
    // printGlobalTable();
    // fflush(stdout);

// switch_var_demo1           36- 56          B         static array      [10, 30]          integer 

    printf("%-15s %-15s %-15s %-15s %-15s %-15s\n ","Scope_Module","Scope", "var name","Static/Dynamic", "Array_Range","element_type");

    for(int i = 0;i<gsize;i++){
        function_entry* curr = GTABLE->functionTable->functionHash[i];

        if(curr == NULL) continue;
        // printf("In global table entry");
        // fflush(stdout);

        para_list* inp = curr->ip_list;
        while(inp!=NULL){

            char* arr = "NO";
            char* stat = "..";
            if(inp->isArr == false)
                arr = "NO";
            else{
                if(inp->isStatic == false)
                    stat = "static array";
                else
                    stat = "dynamic array";
            }
            int scSt = 0;
            int scEn = 0;
            for(int i = 0;i<lsize;i++){
                if(curr->local_table->idTable->ID_HASH[i] != NULL){
                    scSt = curr->local_table->idTable->ID_HASH[i]->scopeSt;
                    scEn = curr->local_table->idTable->ID_HASH[i]->scopeEnd;
                }
            }
            
            if(inp->isArr == true)
                printf("%-10s [%d-%d%-10s %-15s %-15s [%d-%d%-10s %-15s\n ",curr->func_name,scSt,scEn,"]",inp->lex,stat,inp->arrSt,inp->arrEnd,"]",ASTArr[inp->datatype]);
            inp = inp->next;
        }

        inp = curr->op_list;
        while(inp!=NULL){

            char* arr = "NO";
            char* stat = "..";
            if(inp->isArr == false)
                arr = "NO";
            else{
                if(inp->isStatic == false)
                    stat = "static";
                else
                    stat = "dynamic";
            }
            int scSt = 0;
            int scEn = 0;
            for(int i = 0;i<lsize;i++){
                if(curr->local_table->idTable->ID_HASH[i] != NULL){
                    scSt = curr->local_table->idTable->ID_HASH[i]->scopeSt;
                    scEn = curr->local_table->idTable->ID_HASH[i]->scopeEnd;
                }
            }
            
            if(inp->isArr == true)
                printf("%-10s [%d-%d%-10s %-15s %-15s [%d-%d%-10s %-15s\n ",curr->func_name,scSt,scEn,"]",inp->lex,stat,inp->arrSt,inp->arrEnd,"]",ASTArr[inp->datatype]);

            inp = inp->next;

        }

        local_table* table = curr->local_table;
        printLocalArray(table);
    }

    function_entry* func_table = GTABLE->driver_module;
    local_table* table = func_table->local_table;
    printLocalArray(table);
}

void printSymbolTable(){
    // printGlobalTable();
    // fflush(stdout);
    printf("%-10s %-15s %-10s %-15s %-10s %-15s %-15s %-10s %-10s %-10s \n ", "var name","scope_Module","Scope","element_type","is_array","Static/Dynamic","Array_Range","width","offset","nesting level");

    for(int i = 0;i<gsize;i++){
        function_entry* curr = GTABLE->functionTable->functionHash[i];

        if(curr == NULL) continue;
        // printf("In global table entry");
        // fflush(stdout);

        para_list* inp = curr->ip_list;
        while(inp!=NULL){

            char* arr = "NO";
            char* stat = "..";
            if(inp->isArr == false)
                arr = "NO";
            else{
                if(inp->isStatic == false)
                    stat = "static";
                else
                    stat = "dynamic";
            }
            int scSt = 0;
            int scEn = 0;
            for(int i = 0;i<lsize;i++){
                if(curr->local_table->idTable->ID_HASH[i] != NULL){
                    scSt = curr->local_table->idTable->ID_HASH[i]->scopeSt;
                    scEn = curr->local_table->idTable->ID_HASH[i]->scopeEnd;
                }
            }

            if(scSt == 0 && scEn == 0) {
                scSt = curr->scopeSt;
                scEn = curr->scopeEnd;
            }
            
            if(inp->isArr == true){
                printf("%-10s %-15s[%d-%d%-10s %-15s %-10s %-15s[%d-%d%-10s %-10d %-10d %-10d \n ",inp->lex,curr->func_name,scSt,scEn,"]",ASTArr[inp->datatype],"YES",stat,inp->arrSt,inp->arrEnd,"]",inp->width,inp->offset,0);
                // printf("Has been reassigned - %d\n",inp->has_been_reassigned);
            }
            else{
                printf("%-10s %-15s[%d-%d%-10s %-15s %-10s %-15s %-15s %-10d %-10d %-10d \n ",inp->lex,curr->func_name,scSt,scEn,"]",ASTArr[inp->datatype],arr,stat,"..",inp->width,inp->offset,0);
                // printf("Has been reassigned - %d\n",inp->has_been_reassigned);
            }
                inp = inp->next;

        }

        inp = curr->op_list;
        while(inp!=NULL){

            char* arr = "NO";
            char* stat = "..";
            if(inp->isArr == false)
                arr = "NO";
            else{
                if(inp->isStatic == false)
                    stat = "static";
                else
                    stat = "dynamic";
            }
            int scSt = 0;
            int scEn = 0;
            for(int i = 0;i<lsize;i++){
                if(curr->local_table->idTable->ID_HASH[i] != NULL){
                    scSt = curr->local_table->idTable->ID_HASH[i]->scopeSt;
                    scEn = curr->local_table->idTable->ID_HASH[i]->scopeEnd;
                }
            }

             if(scSt == 0 && scEn == 0) {
                scSt = curr->scopeSt;
                scEn = curr->scopeEnd;
            }
            
            if(inp->isArr == true){
                printf("%-10s %-15s[%d-%d%-10s %-15s %-10s %-15s[%d-%d%-10s %-10d %-10d %-10d \n ",inp->lex,curr->func_name,scSt,scEn,"]",ASTArr[inp->datatype],arr,stat,inp->arrSt,inp->arrEnd,"]",inp->width,inp->offset,0);
                // printf("Has been reassigned - %d\n",inp->has_been_reassigned);
            
            }
            else{
                printf("%-10s %-15s[%d-%d%-10s %-15s %-10s %-15s %-15s %-10d %-10d %-10d \n ",inp->lex,curr->func_name,scSt,scEn,"]",ASTArr[inp->datatype],arr,stat,"..",inp->width,inp->offset,0);
                // printf("Has been reassigned - %d\n",inp->has_been_reassigned);
            }
                inp = inp->next;

        }

        local_table* table = curr->local_table;
        printLocalTable(table);
    }

    function_entry* func_table = GTABLE->driver_module;
    local_table* table = func_table->local_table;
    printLocalTable(table);
}

void populateSymbolTable(astNode* node){
    // dealing with first pass
    initializeGlobalTable(GTABLE);
    // printf("LINE 21");
    // fflush(stdout);
    if(node->leftmostChild->astItem == _MODULE_DECLARATION) {
        global_first_pass(node->leftmostChild,-1,-1);
        astNode* temp = node->leftmostChild->nextSibling;
        while(temp!= NULL) {
            // printf("%s\n",ASTArr[temp->astItem]);
            // fflush(stdout);
            if(temp->astItem != _DRIVER_MODULE){
                global_first_pass(temp,-1,-1);
            }
            temp = temp->nextSibling;
        }
    }
    // printf("Done with first pass\n");
    // printSymbolTable();
    // fflush(stdout);
    astNode* temp = node->leftmostChild;
        while(temp!= NULL) {
                   if(temp->astItem != _MODULE_DECLARATION)
                        global_second_pass(temp,-1,-1);
                    temp = temp->nextSibling;
         }

// printf("Done with second pass\n");
// fflush(stdout);
}

void variable_declaration(local_table* table,char* varName,char* scopMod,int scopeSt,int scopeEnd,ASTItem datatype,bool isArray, bool isStatic,bool isDynamic,int arrRanSt, int arrRanEnd,int width,int offset,int level) {
    
    identifier_entry* id = (identifier_entry* )malloc(sizeof(identifier_entry));
    id->varName = varName;
    id->scopMod = scopMod;
    id->scopeSt = scopeSt;
    id->scopeEnd = scopeEnd;
    id->datatype = datatype;
    id->isArray = isArray;
    id->isStatic = isStatic;
    id->isDynamic = isDynamic;
    id->arrRanSt = arrRanSt;
    id->arrRanEnd = arrRanEnd;
    id->width = width;
    id->offset = offset;
    id->level = level;
    id->has_been_reassigned = false;
    insert_idtable(table,id);
            
}

int hashVal1(char* s,int size) {
        long long p = 31, m = 1e9 + 7;
        long long hash = 0;
        long long p_pow = 1;
        int n = strlen(s);
        for(int i = 0; i < n; i++) {
            hash = (hash*31 + s[i])%m;
        }    //return hash;
        return hash%size;
}

function_entry* function_search(global_table* table,char* name){
    int hashval = hashVal1(name,gsize);
    function_entry* entry = GTABLE->functionTable->functionHash[hashval];
    
    while(entry!=NULL){
        // printf("Entry recieved\n");
        // fflush(stdout);
        if(strcmp(entry->func_name,name) == 0)
            return entry;
        entry = entry->next;
    }
    return NULL;
}

identifier_entry* id_search(local_table* table,char* name){
    int hashval = hashVal1(name,lsize);
    identifier_entry* entry = table->idTable->ID_HASH[hashval];
    while(entry!=NULL){
        if(strcmp(entry->varName,name) == 0)
            return entry;
        entry = entry->next;
    }
    if(table->parentTable!=NULL)
        return id_search(table->parentTable,name);
    return NULL;
}

para_list* search_list(char* id, para_list* list){
    if(list == NULL) return NULL;
   // printf("Entered search list\n");
     //fflush(stdout);
    if(list->lex == NULL) return NULL;
    if(strcmp(list->lex,id) == 0){
        // printf("Exiting list1\n");
        // fflush(stdout);
        return list;
    }
    if(list->next!=NULL){
        // printf("In search list first if %s\n",list->next->lex);
        // fflush(stdout);
        return search_list(id,list->next);
    }
    // printf("Exiting list2\n");
    //     fflush(stdout);
    return NULL;
}

    void insert_idtable(local_table* table,identifier_entry* id){
        int hashval = hashVal1(id->varName,lsize);
        if(!table->idTable->ID_HASH[hashval]){
            table->idTable->ID_HASH[hashval] = id;
        }
        else{
            identifier_entry* curr = table->idTable->ID_HASH[hashval];
            while(curr->next!=NULL){
                curr = curr->next;
            }
            curr->next = id;
        }
    }

identifier_entry* local_search(char* name,local_table* table){
    int hashval = hashVal1(name,lsize);
    identifier_entry* entry = table->idTable->ID_HASH[hashval];
    while(entry!=NULL){
        if(strcmp(entry->varName,name) == 0)
            return entry;
        entry = entry->next;
    }
    return NULL;
}

identifier_entry* full_search(local_table* curr_level,char* lex) {
        identifier_entry* result = id_search(curr_level,lex);
        // printf("After id_search in full_search \n");
        // fflush(stdout);
        if(result != NULL) return result;
        local_table* temp = curr_level;
        while(temp->parentTable != NULL) {
            temp = temp->parentTable;
        }
        // printf("After while loop in full_search \n");
        // fflush(stdout);
        para_list* p = search_list(lex,temp->function_head->ip_list);
        // printf("After search_list1 in full_search \n");
        // fflush(stdout);
        if(p != NULL && p->lex != NULL) {
            // printf("In here \n");
            // fflush(stdout);
            identifier_entry* r = (identifier_entry*)malloc(sizeof(identifier_entry));
            r->varName = p->lex;
            r->scopMod = NULL;
            r->scopeSt = -1;
            r->scopeEnd = -1;
            r->datatype = p->datatype;
            r->isArray = p->isArr;
            // printf("BBBB \n");
            // fflush(stdout);
            r->isStatic = p->isStatic;
            // printf("AAA \n");
            // fflush(stdout);
            r->isDynamic = !(p->isStatic);
            // printf("aBBB \n");
            // fflush(stdout);
            r->arrRanSt = p->arrSt;
            // printf("accc\n");
            // fflush(stdout);
            r->arrRanEnd = p->arrEnd;
            // printf("a \n");
            // fflush(stdout);
            r->width = 0;
            r->offset = 0;
            r->has_been_reassigned = p->has_been_reassigned;
            // printf("kkk \n");
            // fflush(stdout);
            return r;

        }
        if(temp->function_head != NULL && temp->function_head->op_list != NULL)
        p = search_list(lex,temp->function_head->op_list);
         //printf("After search_list 2 in full_search \n");

        if(p != NULL) {
            identifier_entry* r = (identifier_entry*)malloc(sizeof(identifier_entry));
            r->varName = p->lex;
            r->scopMod = NULL;
            r->scopeSt = -1;
            r->scopeEnd = -1;
            r->datatype = p->datatype;
            r->isArray = p->isArr;
            r->isStatic = p->isStatic;
            r->isDynamic = !(p->isStatic);
            r->arrRanSt = p->arrSt;
            r->arrRanEnd = p->arrEnd;
            r->width = 0;
            r->offset = 0;
            r->has_been_reassigned = p->has_been_reassigned;
            return r;

        }

        return NULL;
}

identifier_entry* identifier_search(local_table* curr_level,char* lex) {
        identifier_entry* result = local_search(lex,curr_level);
        if(result != NULL) return result;
        while(curr_level->parentTable != NULL) curr_level = curr_level->parentTable;
        para_list* op = curr_level->function_head->op_list;
        while(op != NULL) {
            if(strcmp(op->lex,lex) == 0) {
                identifier_entry * r = (identifier_entry*)malloc(sizeof(identifier_entry));
                r->varName = lex;
                r->has_been_reassigned = op->has_been_reassigned;
                return r;
            }
            op = op->next;
        }
        return NULL;
}

    void insert_functiontable(global_table* table, function_entry* func){
        int hashval = hashVal1(func->func_name,gsize);
        if(!table->functionTable->functionHash[hashval]){
            table->functionTable->functionHash[hashval] = func;
        }
        else{
            function_entry* curr = table->functionTable->functionHash[hashval];
            while(curr->next!=NULL){
                curr = curr->next;
            }
            curr->next = func;
        }

    }



int getWidth(ASTItem datatype){
    if(datatype == _INTEGER){
        return 2;
    }
    if(datatype == _REAL){
        return 4;
    }
    else
        return 1;
}

void checkData(ASTItem dt, astNode* ast,local_table* table){
    if(ast == NULL)
        return;
    if(ast->astItem == _NUM){
        if(dt != _INTEGER)
            printf("Semantic error: Incompatible datatypes in line no for %s %d\n",ast->lexeme,ast->lineNo);
    }
    if(ast->astItem == _RNUM){
        if(dt != _REAL)
            printf("Semantic error: Incompatible datatypes in line no for %s %d\n",ast->lexeme,ast->lineNo);
    }
    if(ast->astItem == _TRUE_ || ast->astItem == _FALSE_){
        if(dt != _BOOLEAN)
            printf("Semantic error: Incompatible datatypes in line no for %s %d\n",ast->lexeme,ast->lineNo);
    }

    if(ast->astItem == _ID){
        identifier_entry* curr = full_search(table,ast->lexeme);
        if(curr == NULL){
            printf("Semantic error: %s Variable not decalared %d\n",ast->lexeme,ast->lineNo);
        }
        else{
            if(curr->datatype!=dt){
                printf("Semantic error: Incompatible datatypes in line no for %s %d\n",ast->lexeme,ast->lineNo);
            }
        }
    }
    if(ast->astItem == _ARRAY_ELEMENT){
        //need to check for rray element
    }

    if(ast->nextSibling!=NULL){
        checkData(dt,ast->nextSibling,table);
    }
    if(ast->leftmostChild!=NULL){
        checkData(dt,ast->leftmostChild,table);
    }
    

}

void function_declaration(global_table* parent_table,char* func_name,para_list* ip_list,para_list* op_list,bool redeclaration_allowed_firstpass,bool redeclaration_allowed_secondpass,int scopeSt,int scopeEnd) {
    function_entry* global_table_entry = (function_entry*)malloc(sizeof(function_entry));
    global_table_entry->func_name = func_name;
    
    global_table_entry->ip_list = ip_list;
    global_table_entry->op_list = op_list;
    global_table_entry->redeclaration_allowed_firstpass = redeclaration_allowed_firstpass;
    global_table_entry->redeclaration_allowed_secondpass = redeclaration_allowed_secondpass;
    global_table_entry->scopeSt = scopeSt;
    global_table_entry->scopeEnd = scopeEnd;
    // printf("LINE 122");
    //     fflush(stdout);
    local_table* newTable = (local_table*)malloc(sizeof(local_table));
    newTable->idTable = (identifier_hash*)malloc(sizeof(identifier_hash));
    newTable->function_head = global_table_entry;
    for(int i = 0;i < lsize; i++) {
        newTable->idTable->ID_HASH[i] = (identifier_entry*) NULL;
    }
    global_table_entry->local_table = newTable;
    insert_functiontable(parent_table,global_table_entry);
}

void local_scope_declaration(local_table* parent_table) {
    local_table* newTable = (local_table*)malloc(sizeof(local_table));
    newTable->idTable = (identifier_hash*)malloc(sizeof(identifier_hash));
    for(int i = 0;i < lsize; i++) {
        newTable->idTable->ID_HASH[i] = NULL;
    } 
    newTable->parentTable = parent_table;
    if(parent_table->firstChildTable == NULL) {
        parent_table->firstChildTable = newTable;
    }
    else {
        local_table* curr = parent_table->firstChildTable;
        while(curr->nextSiblingTable != NULL) {
            curr = curr->nextSiblingTable;
        }
        curr->nextSiblingTable = newTable;
    }
}

void markReassigned(char* lexeme,local_table* curr_level) {
    identifier_entry* local_s = local_search(lexeme,curr_level);
    if(local_s != NULL) {
        local_s->has_been_reassigned = true;
        return;
    }
    while(curr_level->parentTable != NULL) {
        curr_level = curr_level->parentTable;
    }
    function_entry* fx = curr_level->function_head;
        para_list* ip = fx->ip_list;
        while(ip != NULL) {
            if(strcmp(ip->lex,lexeme) == 0) {
                ip->has_been_reassigned = true;
                return;
            }
            ip = ip->next;
        }

        para_list* op = fx->op_list;
        while(op != NULL) {
            if(strcmp(op->lex,lexeme) == 0) {
                op->has_been_reassigned = true;
                return;
            }
            op = op->next;
        }
        return;
}



bool matchType(para_list* ip_list,astNode* act_param,local_table* curr) {

    if(ip_list == NULL) return false;
    act_param = act_param->leftmostChild;
    if(act_param->astItem == _PLUS || act_param->astItem == _MINUS) {
        act_param = act_param->nextSibling;
    }
    if(act_param->astItem == _ID) {
        //printf("ID case consideration \n");
        identifier_entry* en = full_search(curr,act_param->lexeme);
       if(en == NULL) {
           //printf("Semantic error - Undeclared variable usage \n");
           return false;
       }
       //printf("formal para is array ? %d     actual para is array ? %d \n",ip_list->isArr,en->isArray);
       if(!en->isArray && en->datatype == _INTEGER && !ip_list->isArr && ip_list->datatype == _INTEGER) return true;
       if(!en->isArray && en->datatype == _REAL && !ip_list->isArr && ip_list->datatype == _REAL) return true;
       if(!en->isArray && en->datatype == _BOOLEAN && !ip_list->isArr && ip_list->datatype == _BOOLEAN) return true;
        if(en->isArray && ip_list->isArr && ip_list->datatype == en->datatype) {
            //printf("Array case consideration \n");
            int actualSize = (en->arrRanEnd - en->arrRanSt + 1);
            int formalSize = (ip_list->arrEnd - ip_list->arrSt+1);
            return (actualSize <= formalSize);
        }

       return false;

    }
    else {
        if(act_param->astItem != _ARRAY_ITEM)
            return ip_list->datatype == act_param->astItem;
        else {
             identifier_entry* en = full_search(curr,act_param->leftmostChild->lexeme);
            if(en == NULL) {
                 printf("Semantic error - Undeclared variable usage \n");
                    return false;
            }
            return en->datatype == ip_list->datatype;
        }
           
    }
}


bool matchType2(para_list* op_list,astNode* llnode,local_table* curr) {

    if(op_list == NULL) return false;
    
    llnode = llnode->leftmostChild;
    if(llnode->astItem == _ID) {
       // printf("ID case consideration \n");
        identifier_entry* en = full_search(curr,llnode->lexeme);
       if(en == NULL) {
           //printf("Semantic error - Undeclared variable usage \n");
           return false;
       }
       //printf("formal para is array ? %d     actual para is array ? %d \n",ip_list->isArr,en->isArray);
       if(!en->isArray && en->datatype == _INTEGER && !op_list->isArr && op_list->datatype == _INTEGER) return true;
       if(!en->isArray && en->datatype == _REAL && !op_list->isArr && op_list->datatype == _REAL) return true;
       if(!en->isArray && en->datatype == _BOOLEAN && !op_list->isArr && op_list->datatype == _BOOLEAN) return true;
        if(en->isArray && op_list->isArr && op_list->datatype == en->datatype) {
            //printf("Array case consideration \n");
           int actualSize = (en->arrRanEnd - en->arrRanSt+ 1);
            int formalSize = (op_list->arrEnd - op_list->arrSt+1);
            return (actualSize <= formalSize);
        }

       return false;

    }
    else {
        if(llnode->astItem != _ARRAY_ITEM)
            return op_list->datatype == llnode->astItem;
        else {
             identifier_entry* en = full_search(curr,llnode->leftmostChild->lexeme);
            if(en == NULL) {
                 printf("Semantic error - Undeclared variable usage \n");
                    return false;
            }
            return en->datatype == op_list->datatype;
        }
           
    }
}

void local_pass(astNode* node,local_table* curr_table,int offset,int scopeStart,int scopeEnd) {


    if(node == NULL) return;
    // printf("In local pass");
    // fflush(stdout);
    if(node->astItem == _STATEMENT_LINKED_LIST_NODE) {
//         function_entry* module = curr_table->function_head;
//  printf("%s\n",module->func_name);
//  fflush(stdout);
        //printf("In _STATEMENT_LINKED_LIST_NODE \n");
         //fflush(stdout);
        astNode* left = node->leftmostChild;
        // printf("%s\n",ASTArr[left->astItem]);
        // fflush(stdout);
        ASTItem elem;
        if(left != NULL)
            elem = left->astItem;
        else 
            elem = -1;
        if(elem == _GET_INPUT){
            //do type checking on the leftmost child of left(GET INPUT NODE)
        //  printf("In GET INPUT \n");
        //     fflush(stdout);
            identifier_entry* curr = full_search(curr_table,left->leftmostChild->lexeme); 
            // printf("In GET INPUT search done \n");
            // fflush(stdout);
            if(curr == NULL){
                printf("Semantic Error: Variable not declared in line no. %d\n",left->leftmostChild->lineNo);
            }
            // printf("GET INPUT OVER \n");
            // fflush(stdout);
        }

        else if(elem == _FOR_LOOP){
            //do type checking

            // printf("In FOR \n");
            // fflush(stdout);
            local_table* newTable = (local_table*)malloc(sizeof(local_table));
            newTable->parentTable = curr_table;
            newTable->function_head = curr_table->function_head;
            if(curr_table->firstChildTable == NULL)
                curr_table->firstChildTable = newTable;
            else{
                local_table* temp = curr_table->firstChildTable;
                while(temp->nextSiblingTable!=NULL){
                    temp = temp->nextSiblingTable;
                }
                temp->nextSiblingTable = newTable;
            }
    newTable->idTable = (identifier_hash*)malloc(sizeof(identifier_hash));
            
            if(left->leftmostChild->nextSibling->nextSibling == NULL) {
                // printf("SCREWED \n");
                // fflush(stdout);
            }
            // printf("Gonna parse FOR child \n");
            // fflush(stdout);
            char* loop_var_name = left->leftmostChild->lexeme;
            identifier_entry* en = full_search(curr_table,loop_var_name);
            bool save_r = en->has_been_reassigned;
            local_pass(left->leftmostChild->nextSibling->nextSibling,newTable,offset,left->scopeStart,left->scopeEnd);
            if(en->has_been_reassigned == true) {
                printf("Semantic Error - FOR loop variable reassigned in the loop body  beginning at line %d\n",left->leftmostChild->lineNo);
            }
            en->has_been_reassigned = save_r;
            identifier_entry* jj = local_search(loop_var_name,newTable);
            if(jj != NULL) {
                  printf("Semantic Error - FOR loop variable reinitialized in the loop body beginning at line %d\n",left->leftmostChild->lineNo);
            }
            
            // printf("FOR DONE \n");
            // fflush(stdout);
        }

    else if(elem == _CONDITIONAL_STMT){
        // do type checking

        char* lex = left->leftmostChild->lexeme;
        identifier_entry* iden = full_search(curr_table,lex);
        if(iden == NULL){
            printf("Semantics error: Switch variable not declared in line no %d\n",left->leftmostChild->lineNo);
        }
        ASTItem dt = iden->datatype;
        if(dt != _INTEGER && dt != _BOOLEAN){
             printf("Semantics error: Switch variable of incompatible datatype %d\n",left->leftmostChild->lineNo);
        }
        if(dt == _INTEGER && left->leftmostChild->nextSibling->nextSibling == NULL){
            printf("Semantics error: Switch on INTEGER should have default statement in line no %d\n",left->leftmostChild->lineNo);
        }
        if(dt == _BOOLEAN && left->leftmostChild->nextSibling->nextSibling != NULL){
            printf("Semantics error: Switch on BOOLEAN should not have default statement in line no %d\n",left->leftmostChild->lineNo);
        }

        // printf("In conditional stmt(switch) \n");
        // fflush(stdout);

        local_table* newTable = (local_table*)malloc(sizeof(local_table));
            newTable->parentTable = curr_table;
            newTable->function_head = curr_table->function_head;
            if(curr_table->firstChildTable == NULL)
                curr_table->firstChildTable = newTable;
            else{
                local_table* temp = curr_table->firstChildTable;
                while(temp->nextSiblingTable!=NULL){
                    temp = temp->nextSiblingTable;
                }
                temp->nextSiblingTable = newTable;
            }
    newTable->idTable = (identifier_hash*)malloc(sizeof(identifier_hash));
            astNode* kk = left->leftmostChild->nextSibling;
            astNode* ss = left->leftmostChild->nextSibling;
            while(kk!= NULL){
                // printf("Switch while print start\n");
                // fflush(stdout);
            local_pass(kk->leftmostChild->nextSibling,newTable,offset,left->scopeStart,left->scopeEnd);
            kk = kk->leftmostChild->nextSibling->nextSibling;
            // printf("Switch while print end\n");
            //     fflush(stdout);
            }
            // printf("out of while loop\n");
            // fflush(stdout);
            if(ss->nextSibling != NULL) {
                // we have a default
                local_pass(ss->nextSibling->leftmostChild,newTable,offset,left->scopeStart,left->scopeEnd);
            }
            // printf("Done with default");
            // fflush(stdout);

    }

        else if(elem == _WHILE_LOOP){
            //do type checking

            // printf("IN WHILE LOOP \n");
            // fflush(stdout);

            local_table* newTable = (local_table*)malloc(sizeof(local_table));
            newTable->parentTable = curr_table;
            newTable->function_head = curr_table->function_head;
            if(curr_table->firstChildTable == NULL)
                curr_table->firstChildTable = newTable;
            else{
                local_table* temp = curr_table->firstChildTable;
                while(temp->nextSiblingTable!=NULL){
                    temp = temp->nextSiblingTable;
                }
                temp->nextSiblingTable = newTable;
            }
    newTable->idTable = (identifier_hash*)malloc(sizeof(identifier_hash));
            // printf("LOCAL PASS OF WHILE LOOP \n");
            // fflush(stdout);
            local_pass(left->leftmostChild->nextSibling,newTable,offset,left->scopeStart,left->scopeEnd);
            // printf("Done with local pass of WHILE \n");
            // fflush(stdout);
        }

        else if(elem == _PRINT_OUTPUT){

            // do complicated type checking
            // printf(" in _PRINT_OUTPUT \n");
            // fflush(stdout);
            // printf("%s",ASTArr[left->leftmostChild->astItem]);
            // fflush(stdout);
            if(left->leftmostChild->astItem == _ID){
                // printf("In ID case of print \n");
                // fflush(stdout);
                identifier_entry* curr = full_search(curr_table,left->leftmostChild->lexeme);
                // printf("After full search in _PRINT_OUTPUT \n");
                // fflush(stdout);
                if(curr == NULL){
                    printf("Semantic Error: Variable not declared in line no. %d\n",left->leftmostChild->lineNo);
                }
                // printf("No semantic error in _PRINT_OUTPUT id element\n");
                // fflush(stdout);
            }
            else if(left->leftmostChild->astItem == _ARRAY_ELEMENT){
                // printf("ARRAY PRINTING \n");
                // fflush(stdout);
                astNode* temp = left->leftmostChild;
                identifier_entry* curr = full_search(curr_table,temp->leftmostChild->lexeme);
                if(curr == NULL){
                    printf("Semantic Error: Variable not declared in line no. %d\n",left->leftmostChild->lineNo);
                }
                // printf("No semantic error in _PRINT_OUTPUT array element\n");
                // fflush(stdout);
                // printf("%s\n",ASTArr[temp->astItem]);
                // fflush(stdout);
                temp = temp->leftmostChild->nextSibling;
                // printf("here");
                // printf("%s\n",ASTArr[temp->leftmostChild->astItem]);
                // fflush(stdout);
                int sign = 1;
                // printf("Hi\n");
                // printf("%s\n",ASTArr[temp->astItem]);
                // fflush(stdout);
                if(temp->leftmostChild->astItem == _MINUS){
                                   sign = -1;
                    temp = temp->leftmostChild->nextSibling;
                }
                else if(temp->leftmostChild->astItem == _PLUS){
                    temp = temp->leftmostChild->nextSibling;
                }
                else
                    temp = temp->leftmostChild;
                if(temp->astItem == _NUM && temp->intVal<curr->arrRanSt && temp->intVal>curr->arrRanEnd){
                    printf("Semantic Error: Array index out of bounds in line no. %d\n",left->leftmostChild->lineNo);
                }
            }
        }
        else if(elem == _DECLARE_STMT){
            // printf("In declare statement");
            // fflush(stdout);
            if(left->leftmostChild->nextSibling->astItem != _ARRAY_DATA_TYPE && left->leftmostChild->nextSibling->astItem != _ARRAY_ELEMENT) {
                astNode* currNode = left->leftmostChild;

                while(currNode != NULL) {
                    astNode* id = currNode->leftmostChild;
                    identifier_entry* result = identifier_search(curr_table,id->lexeme);
                    if(result != NULL) {
                        printf("Semantic Error - Variable redeclaration at line number %d\n",left->leftmostChild->leftmostChild->lineNo);
                    }
                    else {
                        local_table* temp = curr_table;
                        while(temp->parentTable != NULL) temp = temp->parentTable;
                        function_entry* module = temp->function_head;
                        int width = getWidth((left->leftmostChild->nextSibling->astItem));
                        int level = getLevel(curr_table);
                        variable_declaration(curr_table,id->lexeme,module->func_name,left->scopeStart,left->scopeEnd,left->leftmostChild->nextSibling->astItem,false,true,false,-1,-1,width,offset,level);
                        offset= offset + width;
                    }
                    currNode = currNode->leftmostChild->nextSibling;

                    
                }
                
            
            }
            else {
                astNode* dt = left->leftmostChild->nextSibling;
                int rangeSt = 0;
                bool isDynamic = false;
                astNode* signNode = dt->leftmostChild->leftmostChild->leftmostChild;
                ASTItem tt= dt->leftmostChild->nextSibling->astItem;
                if(signNode->astItem == _MINUS){
                    if(dt->leftmostChild->leftmostChild->leftmostChild->nextSibling->astItem == _NUM)
                        rangeSt = dt->leftmostChild->leftmostChild->leftmostChild->nextSibling->intVal*-1;
                    else
                        isDynamic = true;
                }
                else if(signNode->astItem == _PLUS){
                    if(dt->leftmostChild->leftmostChild->leftmostChild->nextSibling->astItem == _NUM)
                        rangeSt = dt->leftmostChild->leftmostChild->leftmostChild->nextSibling->intVal;
                    else
                        isDynamic = true;
                }
                else{
                    if(signNode->astItem == _NUM)
                        rangeSt = dt->leftmostChild->leftmostChild->leftmostChild->intVal;
                    else
                        isDynamic = true;
                }
                int rangeEnd = 0;
                signNode = dt->leftmostChild->leftmostChild->nextSibling->leftmostChild;
                if(signNode->astItem == _MINUS){
                    if(signNode->astItem == _NUM)
                        rangeEnd = signNode->nextSibling->intVal*-1;
                    else
                        isDynamic = true;
                }
                else if(signNode->astItem == _PLUS){
                    if(signNode->astItem == _NUM)
                        rangeEnd = signNode->nextSibling->intVal*-1;
                    else
                        isDynamic = true;
                }
                else{
                    if(signNode->astItem == _NUM)
                        rangeEnd = signNode->intVal;
                    else
                        isDynamic = true;
                }
                astNode* currNode = left->leftmostChild;

                while(currNode != NULL) {
                    astNode* id = currNode->leftmostChild;
                    identifier_entry* result = local_search(id->lexeme,curr_table);
                    if(result != NULL) {
                        printf("Semantic Error - Variable redeclaration at line number %d\n",left->leftmostChild->leftmostChild->lineNo);
                    }
                    else {
                        local_table* temp = curr_table;
                        while(temp->parentTable != NULL) temp = temp->parentTable;
                        function_entry* module = temp->function_head;
                        
                        int width = getWidth(tt);
                        int actWidth = 0;
                        if(!isDynamic) actWidth = ((rangeEnd- rangeSt+1)*width)+1;
                        
                        int level = getLevel(curr_table);
                        variable_declaration(curr_table,id->lexeme,module->func_name,left->scopeStart,left->scopeEnd,tt,true,!isDynamic,isDynamic,rangeSt,rangeEnd,actWidth,offset,level);
                        offset= offset + actWidth;
                    }
                    currNode = currNode->leftmostChild->nextSibling;

                    
                }


            }
        }
         else if(elem == _MODULE_REUSE_STMT) {
            // left holds the module reuse statement
            char* module_name;
            local_table* ee = curr_table;
            while(curr_table->parentTable != NULL) {
                ee = ee->parentTable;
            }
            module_name  = ee->function_head->func_name;
            char* f_name = left->leftmostChild->lexeme;

            if(strcmp(f_name,module_name) == 0) {
                printf("Semantic Error - Recursive call of function %s",f_name);
            }
            bool nofxn = false;
            function_entry* fxn = function_search(GTABLE,f_name);
            fflush(stdout);
            if(fxn == NULL) {
                nofxn = true;
                printf("Semantic Error - Undeclared function used at line no %d \n",left->leftmostChild->lineNo);
            }
            
            para_list* ip_list = NULL;

            if(!nofxn) ip_list = fxn->ip_list;

            astNode* act_param = left->leftmostChild->nextSibling;
            //act_param is _LINKED_LIST_NODE;
            bool mis = false;
            while(ip_list != NULL && act_param != NULL) {
                if(!matchType(ip_list,act_param,curr_table)) {
                    printf("Type Checking Error - Actual and Formal parameter type mismatch at line no %d for formal_para %s\n",left->leftmostChild->lineNo,ip_list->lex);

                    mis = true;
                    
                }

                fflush(stdout);
                ip_list = ip_list->next;
                act_param = act_param->leftmostChild->nextSibling;
                if(act_param != NULL && act_param->astItem != _SIGNED_VALUE) act_param = act_param->nextSibling;

                fflush(stdout);
            }

            if(ip_list == NULL && act_param != NULL && !nofxn) {
                printf("Type Checking Error - Actual and Formal parameter length mismatch at line no %d \n",left->leftmostChild->lineNo);
            }

            if(ip_list != NULL && act_param == NULL && !nofxn) {
                printf("Type Checking Error - Actual and Formal parameter length mismatch at line no %d \n",left->leftmostChild->lineNo);
            }


        } 
        else if(elem == _ASSIGNOP) {
            //printf("IN ASSIGN OP \n");
                left = left->leftmostChild;
                char* f_name = left->leftmostChild->lexeme;
            bool nofxn = false;
            function_entry* fxn = function_search(GTABLE,f_name);
            fflush(stdout);
            if(fxn == NULL) {
                nofxn = true;
                printf("Semantic Error - Undeclared function used at line no %d \n",left->leftmostChild->lineNo);
            }
            
            para_list* ip_list = NULL;

            if(!nofxn) ip_list = fxn->ip_list;

            astNode* act_param = left->leftmostChild->nextSibling;
            //act_param is _LINKED_LIST_NODE;
            bool mis = false;
            while(ip_list != NULL && act_param != NULL) {
                if(!matchType(ip_list,act_param,curr_table)) {
                   printf("Type Checking Error - Actual and Formal parameter type mismatch at line no %d for formal_para %s\n",left->leftmostChild->lineNo,ip_list->lex);

                    mis = true;
                    
                }

                fflush(stdout);
                ip_list = ip_list->next;
                act_param = act_param->leftmostChild->nextSibling;
                if(act_param != NULL && act_param->astItem != _SIGNED_VALUE) act_param = act_param->nextSibling;

                fflush(stdout);
            }

            if(ip_list == NULL && act_param != NULL && !nofxn) {
                printf("Type Checking Error - Actual and Formal parameter length mismatch at line no %d \n",left->leftmostChild->lineNo);
            }

            if(ip_list != NULL && act_param == NULL && !nofxn) {
                printf("Type Checking Error - Actual and Formal parameter length mismatch at line no %d \n",left->leftmostChild->lineNo);
            }

            

          






        }

         else if(elem == _LVALUE_ID_STMT) {
            char* lexeme = left->leftmostChild->lexeme;
             identifier_entry* entry = full_search(curr_table,lexeme);
                if(entry == NULL)
                 printf("Semantic error: Variable no declared earlier in line no %d",left->lineNo);
            else{
                  markReassigned(lexeme,curr_table);
            }
         }

    }
    // printf("HHHH %s \n",ASTArr[node->leftmostChild->astItem]);
    // fflush(stdout);
     if(node->leftmostChild != NULL){
        // printf("Starting local pass from %s \n",ASTArr[node->leftmostChild->astItem]);
    local_pass(node->leftmostChild->nextSibling,curr_table,offset,scopeStart,scopeEnd);
    
    // printf("Ending local pass from %s \n",ASTArr[node->leftmostChild->astItem]);
     }
    else {
        // printf("WT \n");
        // fflush(stdout);
    }
}

void global_second_pass(astNode* node,int scopeStart,int scopeEnd) {
    
    // printf("In second pass \n");
    //  fflush(stdout);
    if(node == NULL) return; 

    if(node->astItem == _MODULE_NODE) {
        // printf("Hi");
        // fflush(stdout);
        node = node->leftmostChild;
        char* name = node->leftmostChild->lexeme; 
        
        function_entry* is_present = function_search(GTABLE,name);

        // printf("In MODULE NODE\n");
        // fflush(stdout);

        if(is_present != NULL) {
            // printf("Module already declared yet to know if there is an error for ID %s\n",node->leftmostChild->lexeme);
            // fflush(stdout);
            if(is_present->redeclaration_allowed_secondpass == true) {
                is_present->redeclaration_allowed_secondpass = false;
                global_second_pass(node->nextSibling,scopeStart,scopeEnd);
                
            }
            else {
                printf("Semantic Error - Module Redeclaration Error at line number %d and ID %s \n",node->leftmostChild->lineNo,node->leftmostChild->lexeme);
                global_second_pass(node->nextSibling,scopeStart,scopeEnd);
            }
        }
        else{
            // printf("In the else block \n");
            // fflush(stdout);
            int offset = 0;
        para_list* ip_list = (para_list*)malloc(sizeof(para_list));
        para_list* op_list = (para_list*)malloc(sizeof(para_list));
        para_list* save_ip = ip_list;
        para_list* save_op = op_list;
        save_ip->lex = NULL;
        save_op->lex = NULL;
        // node->leftmostChild->nextSibling
        astNode* input_ll_node = node->leftmostChild->nextSibling;
         while(input_ll_node != NULL) {
             ip_list->lex = input_ll_node->leftmostChild->lexeme;
            ip_list->has_been_reassigned = false;
            ASTItem datatype = input_ll_node->leftmostChild->nextSibling->astItem;
            if(datatype == _INTEGER || datatype == _BOOLEAN || datatype == _REAL){
                ip_list->datatype = input_ll_node->leftmostChild->nextSibling->astItem;
                ip_list->arrSt = 0;
                ip_list->arrEnd = 0;
                ip_list->isArr = false;
                ip_list->width = getWidth(datatype);
                ip_list->offset = offset;
                offset += ip_list->width;
            }
            else{
                astNode* curr = input_ll_node->leftmostChild->nextSibling;
                // printf("%s\n",ASTArr[curr->astItem]);
                // fflush(stdout);
                ip_list->datatype = curr->leftmostChild->nextSibling->astItem;
                ip_list->isArr = true;
                curr = curr->leftmostChild->leftmostChild;
                bool sign = 0;
                astNode* temp = curr;
                if(curr->leftmostChild->astItem == _MINUS){
                    sign = 1;
                    curr = curr->leftmostChild->nextSibling;
                }
                else if(curr->leftmostChild->astItem == _PLUS)
                    curr = curr->leftmostChild->nextSibling;
                else
                    curr = curr->leftmostChild;
                if(curr->astItem == _ID)
                    ip_list->isStatic = false;
                else{
                    int st = curr->intVal;
                    if(sign == 1)
                    st = st*-1;
                    ip_list->arrSt = st;
                }
                curr = temp->nextSibling;
                sign = 0;
                if(curr->leftmostChild->astItem == _MINUS){
                    sign = 1;
                    curr = curr->leftmostChild->nextSibling;
                }
                else if(curr->leftmostChild->astItem == _PLUS)
                    curr = curr->leftmostChild->nextSibling;
                else
                    curr = curr->leftmostChild;
                if(curr->astItem == _ID){
                    ip_list->isStatic = false;
                }
                else{
                    int end = curr->intVal;
                    if(sign == 1)
                    end = end*-1;
                    ip_list->arrEnd = end;
                }

                int actWidth = 0;
                    if(!ip_list->isStatic)
                        actWidth = ((ip_list->arrEnd - ip_list->arrSt+1)* getWidth(ip_list->datatype))+1;
                    else
                        actWidth = 0;
                     ip_list->offset = offset;
                     offset += actWidth;
                     ip_list->width = actWidth;

                    //  printf("Actual WD ip : %d \n",actWidth);
                    // fflush(stdout);

                     
            }
             input_ll_node = input_ll_node->leftmostChild->nextSibling->nextSibling        ;
             if(input_ll_node != NULL)
                ip_list->next = (para_list*)malloc(sizeof(para_list));
             ip_list = ip_list->next;
         }

        //  printf("Input done\n");
        //  fflush(stdout);

        astNode* output_ll_node = node->leftmostChild->nextSibling->nextSibling;
        // printf("%s\n",ASTArr[output_ll_node->astItem]);
        //         fflush(stdout);
         while(output_ll_node != NULL && output_ll_node->astItem!=_STATEMENT_LINKED_LIST_NODE){
            //  printf("dealing with output list \n");
            //  fflush(stdout);
             op_list->lex = output_ll_node->leftmostChild->lexeme;
             op_list->has_been_reassigned = false;
            ASTItem datatype = output_ll_node->leftmostChild->nextSibling->astItem;
            if(datatype == _INTEGER || datatype == _BOOLEAN || datatype == _REAL){
                op_list->datatype = output_ll_node->leftmostChild->nextSibling->astItem;
                op_list->arrSt = 0;
                op_list->arrEnd = 0;
                op_list->isArr = false;
                op_list->width = getWidth(datatype);
                op_list->offset = offset;
                offset += op_list->width;
            //     printf("primitive type \n");
            //  fflush(stdout);
            }
            else{
                astNode* curr = output_ll_node->leftmostChild->nextSibling;
                // printf("%s\n",ASTArr[curr->astItem]);
                // fflush(stdout);
                op_list->datatype = curr->leftmostChild->nextSibling->astItem;
                op_list->isArr = true;
                curr = curr->leftmostChild->leftmostChild;
                bool sign = 0;
                astNode* temp = curr;
                if(curr->leftmostChild->astItem == _MINUS){
                    sign = 1;
                    curr = curr->leftmostChild->nextSibling;
                }
                else if(curr->leftmostChild->astItem == _PLUS)
                    curr = curr->leftmostChild->nextSibling;
                else
                    curr = curr->leftmostChild;
                if(curr->astItem == _ID)
                    op_list->isStatic = false;
                else{
                    int st = curr->intVal;
                    if(sign == 1)
                    st = st*-1;
                    op_list->arrSt = st;
                }
                curr = temp->nextSibling;
                sign = 0;
                if(curr->leftmostChild->astItem == _MINUS){
                    sign = 1;
                    curr = curr->leftmostChild->nextSibling;
                }
                else if(curr->leftmostChild->astItem == _PLUS)
                    curr = curr->leftmostChild->nextSibling;
                else
                    curr = curr->leftmostChild;
                if(curr->astItem == _ID){
                    op_list->isStatic = false;
                }
                else{
                    int end = curr->intVal;
                    if(sign == 1)
                    end = end*-1;
                    op_list->arrEnd = end;
                }

                int actWidth = 0;
                    if(op_list->isStatic)
                        actWidth = ((op_list->arrEnd - op_list->arrSt+1)* getWidth(op_list->datatype))+1;
                    else
                        actWidth = 0;
                    // printf("Actual WD op : %d \n",actWidth);
                    // fflush(stdout);
                     op_list->offset = offset;
                     offset += actWidth;
                     op_list->width = actWidth;
            }
            output_ll_node = output_ll_node->leftmostChild->nextSibling->nextSibling;
             if(output_ll_node != NULL)
                op_list->next = (para_list*)malloc(sizeof(para_list));
             op_list = op_list->next;

            //  printf("ouput item dealt \n");
            //  fflush(stdout);
         }
        //  printf("OP DONE \n");
        //  fflush(stdout);
         if(save_op->lex != NULL)
            function_declaration(GTABLE,node->leftmostChild->lexeme,save_ip,save_op,false,false,node->leftmostChild->scopeStart,node->leftmostChild->scopeEnd);
        else
        function_declaration(GTABLE,node->leftmostChild->lexeme,save_ip,NULL,false,false,node->leftmostChild->scopeStart,node->leftmostChild->scopeEnd);
        function_entry* curr_entry = function_search(GTABLE,name);
        if(node->leftmostChild->nextSibling->nextSibling->astItem == _STATEMENT_LINKED_LIST_NODE)
            local_pass(node->leftmostChild->nextSibling->nextSibling,curr_entry->local_table,offset,node->scopeStart,node->scopeEnd);
        else
            local_pass(node->leftmostChild->nextSibling->nextSibling->nextSibling,curr_entry->local_table,offset,node->scopeStart,node->scopeEnd);


         if(node->nextSibling != NULL)
                global_second_pass(node->nextSibling,-1,-1);
    }

    
    }
    else if(node->astItem == _DRIVER_MODULE){
        // printf("IN driver");
        // fflush(stdout);
            function_entry* driver_func_entry = (function_entry*)malloc(sizeof(function_entry));
            driver_func_entry->scopeSt = node->scopeStart;
            driver_func_entry->scopeEnd = node->scopeEnd;
            //    printf("LINE 242");
            //     fflush(stdout);
            driver_func_entry->func_name = "DRIVER_MODULE";
            driver_func_entry->local_table = (local_table*)malloc(sizeof(local_table));
            driver_func_entry->local_table->idTable = (identifier_hash*)malloc(sizeof(identifier_hash));
            driver_func_entry->local_table->function_head = driver_func_entry;
            for(int i = 0;i < lsize; i++) {
                driver_func_entry->local_table->idTable->ID_HASH[i] = NULL;
            }
            driver_func_entry->ip_list = NULL;
            driver_func_entry->op_list = NULL;
            driver_func_entry->redeclaration_allowed_secondpass = false;
            driver_func_entry->redeclaration_allowed_firstpass = false;
            driver_func_entry->next = NULL;
            // fflush(stdout);
            GTABLE->driver_module = driver_func_entry;
            local_pass(node->leftmostChild,driver_func_entry->local_table,0,node->scopeStart,node->scopeEnd);
    }


}

void global_first_pass(astNode* node,int scopeStart,int scopeEnd) {
    if(node == NULL) return;
    //  printf("LINE 244");
    //     fflush(stdout);
    if(node->astItem == _MODULE_DECLARATION) {
        function_declaration(GTABLE,node->leftmostChild->lexeme,NULL,NULL,true,true,0,0);
        // printf("LINE 248");
        // fflush(stdout);
        if(node->leftmostChild != NULL) {
            global_first_pass(node->leftmostChild->nextSibling,node->scopeStart,node->scopeEnd);
        }
    }
    else if(node->astItem == _MODULE_NODE) {
        // printf("MODULE NODE IN FIRT PASS\n");
        // fflush(stdout);
        node = node->leftmostChild;
        function_entry* fxn = function_search(GTABLE,node->leftmostChild->lexeme);
        // printf("MODULE NODE IN FIRT PASS after function search\n");
        // fflush(stdout);
        if(fxn == NULL) {
            // printf("BITVH \n");
            // fflush(stdout);
            global_first_pass(node->nextSibling,-1,-1);
        }
        else {
            int offset = 0;
            if(fxn->redeclaration_allowed_firstpass == true){
                // printf("IN redclaration allowed \n");
                // fflush(stdout);
            fxn->redeclaration_allowed_firstpass = false;

            para_list* ip_list = (para_list*)malloc(sizeof(para_list));
            para_list* op_list = (para_list*)malloc(sizeof(para_list));
            para_list* save_ip = ip_list;
            para_list* save_op = op_list;
            save_ip->lex = NULL;
            save_op->lex = NULL;
            // printf("Done");
            // fflush(stdout);
            astNode* input_ll_node = node->leftmostChild->nextSibling;
         while(input_ll_node != NULL) {
             ip_list->lex = input_ll_node->leftmostChild->lexeme;
             ip_list->has_been_reassigned = false;
            ASTItem datatype = input_ll_node->leftmostChild->nextSibling->astItem;
            if(datatype == _INTEGER || datatype == _BOOLEAN || datatype == _REAL){
                ip_list->datatype = input_ll_node->leftmostChild->nextSibling->astItem;
                ip_list->arrSt = 0;
                ip_list->arrEnd = 0;
                ip_list->isArr = false;
                ip_list->offset = offset;
                ip_list->width = getWidth(datatype);
                offset += getWidth(datatype);
            }
            else{
                astNode* curr = input_ll_node->leftmostChild->nextSibling;
                // printf("%s\n",ASTArr[curr->astItem]);
                // fflush(stdout);
                ip_list->datatype = curr->leftmostChild->nextSibling->astItem;
                ip_list->isArr = true;
                curr = curr->leftmostChild->leftmostChild;
                bool sign = 0;
                astNode* temp = curr;
                if(curr->leftmostChild->astItem == _MINUS){
                    sign = 1;
                    curr = curr->leftmostChild->nextSibling;
                }
                else if(curr->leftmostChild->astItem == _PLUS)
                    curr = curr->leftmostChild->nextSibling;
                else
                    curr = curr->leftmostChild;
                if(curr->astItem == _ID)
                    ip_list->isStatic = false;
                else{
                    int st = curr->intVal;
                    if(sign == 1)
                    st = st*-1;
                    ip_list->arrSt = st;
                }
                curr = temp->nextSibling;
                sign = 0;
                if(curr->leftmostChild->astItem == _MINUS){
                    sign = 1;
                    curr = curr->leftmostChild->nextSibling;
                }
                else if(curr->leftmostChild->astItem == _PLUS)
                    curr = curr->leftmostChild->nextSibling;
                else
                    curr = curr->leftmostChild;
                if(curr->astItem == _ID){
                    ip_list->isStatic = false;
                }
                else{
                    int end = curr->intVal;
                    if(sign == 1)
                    end = end*-1;
                    ip_list->arrEnd = end;
                }
                    int actWidth = 0;
                    if(!ip_list->isStatic)
                        actWidth = ((ip_list->arrEnd - ip_list->arrSt+1)* getWidth(ip_list->datatype))+1;
                    else
                        actWidth = 0;
                     ip_list->offset = offset;
                     offset += actWidth;
                     ip_list->width = actWidth;

                    //  printf("Actual WD ip : %d \n",actWidth);
                    // fflush(stdout);
            }
             input_ll_node = input_ll_node->leftmostChild->nextSibling->nextSibling;
             
             
            if(input_ll_node != NULL)
             ip_list->next = (para_list*)malloc(sizeof(para_list));
            
             
             
             ip_list = ip_list->next;
         }
        // printf("Input done\n");
        // fflush(stdout);
         astNode* output_ll_node = node->leftmostChild->nextSibling->nextSibling;
        //   printf("%s\n",ASTArr[output_ll_node->astItem]);
        //     fflush(stdout);
         while(output_ll_node != NULL && output_ll_node->astItem!=_STATEMENT_LINKED_LIST_NODE){
             op_list->lex = output_ll_node->leftmostChild->lexeme;
             op_list->has_been_reassigned = false;
            ASTItem datatype = output_ll_node->leftmostChild->nextSibling->astItem;
            if(datatype == _INTEGER || datatype == _BOOLEAN || datatype == _REAL){
                op_list->datatype = output_ll_node->leftmostChild->nextSibling->astItem;
                op_list->arrSt = 0;
                op_list->arrEnd = 0;
                op_list->isArr = false;
                op_list->width = getWidth(datatype);
                op_list->offset = offset;
                offset += op_list->width;
            }
            else{
                astNode* curr = output_ll_node->leftmostChild->nextSibling;
                op_list->datatype = curr->leftmostChild->nextSibling->astItem;
                op_list->isArr = true;
                curr = curr->leftmostChild->leftmostChild;
                bool sign = 0;
                astNode* temp = curr;
                if(curr->leftmostChild->astItem == _MINUS){
                    sign = 1;
                    curr = curr->leftmostChild->nextSibling;
                }
                else if(curr->leftmostChild->astItem == _PLUS)
                    curr = curr->leftmostChild->nextSibling;
                else
                    curr = curr->leftmostChild;
                if(curr->astItem == _ID)
                    op_list->isStatic = false;
                else{
                    int st = curr->intVal;
                    if(sign == 1)
                    st = st*-1;
                    op_list->arrSt = st;
                }
                curr = temp->nextSibling;
                sign = 0;
                if(curr->leftmostChild->astItem == _MINUS){
                    sign = 1;
                    curr = curr->leftmostChild->nextSibling;
                }
                else if(curr->leftmostChild->astItem == _PLUS)
                    curr = curr->leftmostChild->nextSibling;
                else
                    curr = curr->leftmostChild;
                if(curr->astItem == _ID){
                    op_list->isStatic = false;
                }
                else{
                    int end = curr->intVal;
                    if(sign == 1)
                    end = end*-1;
                    op_list->arrEnd = end;
                }


                int actWidth = 0;
                    if(op_list->isStatic)
                        actWidth = ((op_list->arrEnd - op_list->arrSt+1)* getWidth(op_list->datatype))+1;
                    else
                        actWidth = 0;
                     op_list->offset = offset;
                     offset += actWidth;
                     op_list->width = actWidth;

                    //  printf("Actual WD op : %d \n",actWidth);
                    // fflush(stdout);
            }

             output_ll_node = output_ll_node->leftmostChild->nextSibling->nextSibling;
            if(output_ll_node != NULL)
             op_list->next = (para_list*)malloc(sizeof(para_list));
             op_list = op_list->next;
    }

           fxn->func_name = node->leftmostChild->lexeme;
           fxn->ip_list = save_ip;
           if(save_op->lex != NULL)
            fxn->op_list = save_op;
           else
            fxn->op_list = NULL; 

            fxn->scopeSt = node->leftmostChild->scopeStart;
            fxn->scopeEnd = node->leftmostChild->scopeEnd;
        // printf("Sending to local pass\n");
        if(node->leftmostChild->nextSibling->nextSibling->astItem == _STATEMENT_LINKED_LIST_NODE)
            local_pass(node->leftmostChild->nextSibling->nextSibling,fxn->local_table,offset,node->scopeStart,node->scopeEnd);
        else
            local_pass(node->leftmostChild->nextSibling->nextSibling->nextSibling,fxn->local_table,offset,node->scopeStart,node->scopeEnd);
            }
            else {
                printf("SEMANTIC ERROR - Module redeclaration at line %d\n",node->leftmostChild->lineNo);
            }
            if(node->nextSibling != NULL)
                global_first_pass(node->nextSibling,-1,-1);
        }
    }

}




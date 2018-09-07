//
//  CMOS.cpp
//  Microelectronics
//
//  Created by Ahmad Refaat on 12/4/17.
//  Copyright © 2017 Ahmad Refaat. All rights reserved.
//

#include "CMOS.hpp"



CMOS::CMOS(string x){
    
    
    // Don't forget to validate
    valid = true;
    
    char Output = x[0];
    if(x[1]!= '='){
        valid = false;
    }
    
    for(int i = 2 ; i < x.length(); i++){
        if(x[i] == Output)
            valid = false;
        
        if(!isOperand(x[i]) && !isOperator(x[i]) && x[i] != '\'' && x[i] != '(' && x[i] != ')')
            valid = false;
        
        if(isOperator(x[i])){
            
            if(i == 0 || i == x.length() - 1)
                valid = false;
            
            else if((!isOperand(x[i-1])&&x[i-1] != '(' && x[i-1] != ')' ) ||
                    (!isOperand(x[i+1])&&x[i+1] != '(' && x[i+1] != ')' ))
                valid = false;
        }
        
    }
    
    if(!valid){
        cout << "Sorry the entered boolean function is invalid." << endl;
        return;
    }
    
    finalOutput = Output;
    infix = x.substr(2);
    

    // Validate here
    this->generateCircuit();
    sizing_bool = false;
}

CMOS::CMOS(string x,int ll,int nn,int pp){
    
    // Don't forget to validate
    valid = true;
    
    char Output = x[0];
    if(x[1]!= '='){
        valid = false;
    }
    
    for(int i = 2 ; i < x.length(); i++){
        if(x[i] == Output)
            valid = false;
        
        if(!isOperand(x[i]) && !isOperator(x[i]) && x[i] != '\'' && x[i] != '(' && x[i] != ')')
            valid = false;
        
        if(isOperator(x[i])){
            
            if(i == 0 || i == x.length() - 1)
                valid = false;
            
            else if((!isOperand(x[i-1])&&x[i-1] != '(' && x[i-1] != ')' ) ||
                    (!isOperand(x[i+1])&&x[i+1] != '(' && x[i+1] != ')' ))
                valid = false;
        }
        
    }
    
    if(!valid){
        cout << "Sorry the entered boolean function is invalid." << endl;
        return;
    }
    
    finalOutput = Output;
    infix = x.substr(2);
    
    N = nn;
    L = ll;
    P = pp;
    
    // Validate here
    this->generateCircuit();    
    sizing_bool = true;
}


void CMOS::print(){
    
    if(!valid)
        return;
    
    if(sizing_bool){
        
        
        cout << "PUN: " << endl;
        print_transistors_sizing(PUN,false);
        
        cout << "PDN: " << endl;
        print_transistors_sizing(PDN,false);
        
        cout << "Inverters: " << endl;
        print_transistors_sizing(inverted_inputs,true);
        
        cout << endl << endl;
        
    }else{
        
        cout << "PUN: " << endl;
        print_transistors_normal(PUN,false);
        
        cout << "PDN: " << endl;
        print_transistors_normal(PDN,false);

        
        cout << "Inverters: " << endl;
        print_transistors_normal(inverted_inputs,true);
        
        cout << endl << endl;


        
    }
    
}

void CMOS::print_transistors_normal(vector<transistor>& PUN, bool inverter){
    
    cout << endl;
    for(int i = 0 ; i < PUN.size(); i++){
        cout << PUN[i].name << " " << PUN[i].drain << " " << PUN[i].gate << " " << PUN[i].src << "  " << PUN[i].src << " " << PUN[i].type  << endl;
    }
    cout << endl;
}

void CMOS::print_transistors_sizing(vector<transistor>& PUN, bool inverter){
    
    cout << endl ;
    for(int i = 0 ; i < PUN.size(); i++){
        cout << PUN[i].name << " " << PUN[i].drain << " " << PUN[i].gate << "  " << PUN[i].src<< "  " << PUN[i].src << "  " << PUN[i].type << ", W = " << ((PUN[i].type == "PMOS")? (to_string(L*P*PUN[i].sizing)) : (to_string(L*N*PUN[i].sizing))) << ", L = " << L << endl;
    }
    cout << endl;
}



void CMOS::generateCircuit(){
    
    string temp1 = infix, temp2 = "(" + infix + ")'";
    
    apply_demorgan_to_string(temp1, 0, temp1.length(), false);
    apply_demorgan_to_string(temp2, 0, temp2.length(), false);
    
    string postfix_PUN, postfix_PDN;
    
    postfix_PUN = this->infix_postfix_converter(temp1);
    postfix_PDN = this->infix_postfix_converter(temp2);
    
    
    int num_operands1 = this->create_expression_tree(postfix_PUN, arr1);
    
    top = -1;
    int num_operands2 = this->create_expression_tree(postfix_PDN, arr2);
    
    if(num_operands1 == 1 || num_operands2 == 1){
        to_be_inverted.push_back(arr1[0]->data);
    }else if(num_operands1 > 1 || num_operands2 > 1) {
        
        this->apply_sizing(arr1[0], 1);
        this->apply_sizing(arr2[0], 1);
        
        this->create_transistors_from_tree(arr1[0], finalOutput, "VDD", PUN, "PMOS");
        this->create_transistors_from_tree(arr2[0], finalOutput, "GND", PDN, "NMOS");
    }
    
    create_inverters(inverted_inputs, to_be_inverted);
    
    
    /*postfix = this->infix_postfix_converter(infix);
    int num_operands = this->create_expression_tree(postfix);
    
    if(num_operands > 1){
        
        this->apply_demorgan(arr[0], false); // HAS TO BE APPLIED TO GENERATE THE SIMPLEST FUNCTION
        this->apply_sizing(arr[0], 1);
        this->create_transistors_from_tree(arr[0], finalOutput, "VDD", PUN, "PMOS");
        
        
        apply_demorgan(arr[0], true); // HAS TO BE APPLIED TO GENERATE THE SIMPLEST FUNCTION
        apply_sizing(arr[0], 1);
        this->create_transistors_from_tree(arr[0], finalOutput, "GND", PDN, "NMOS");
        
    }else if(num_operands == 1)
        to_be_inverted.push_back(arr[0]->data);
    
    create_inverters(inverted_inputs, to_be_inverted);
     */
    
    
}



//Generating PDN & PUN
string CMOS::create_transistors_from_tree(node* tree, string drain, string src, vector<transistor>& all_trans, string type){
    
    if(tree == NULL)
        return NULL;
    
    if(tree->operand){
        
        temp_trans.sizing = tree->ratio;
        temp_trans.name = "M" + to_string(++P_Trans);
        temp_trans.src = (src == "")? "Wire_" + to_string(++P_Wires) :src;
        temp_trans.drain = drain;
        
        if((!tree->inverted && type == "PMOS") || (tree->inverted && type == "NMOS")){
            temp_trans.gate  = tree->data;
            temp_trans.gate += "\'";
            
            if(!search_vector(to_be_inverted,tree->data))
                to_be_inverted.push_back(tree->data);
            
        }else{
            temp_trans.gate = tree->data ;
        }
        temp_trans.type = type;
        
        all_trans.push_back(temp_trans);
        return temp_trans.src;
        
    }else{
        
        if(tree->data == '&'){
            string temp_drain = create_transistors_from_tree(tree->left, drain, "", all_trans,type); // return src for left is drain for right
            string temp_src_and = (src == "")? "Wire_" + to_string(++P_Wires) :src;
            create_transistors_from_tree(tree->right, temp_drain, temp_src_and,all_trans,type);
            
            return temp_src_and;
            
        }else if(tree->data == '|'){
            
            string temp_src_or = (src == "")? "Wire_" + to_string(++P_Wires) :src;
            create_transistors_from_tree(tree->left, drain, temp_src_or,all_trans,type);
            create_transistors_from_tree(tree->right, drain, temp_src_or,all_trans,type);
            
            return temp_src_or;
        }
        
    }
    
    return NULL;
}

void CMOS::create_inverters(vector<transistor>& transistors, vector<char>& inputs){
    
    for(int i = 0 ; i < inputs.size(); i++){
        
        temp_trans.name   = "M" + to_string(++P_Trans);
        temp_trans.drain  = inputs[i];
        temp_trans.drain += "\'";
        temp_trans.gate   = inputs[i];
        temp_trans.src    = "VDD";
        temp_trans.type   = "PMOS";
        temp_trans.sizing = 1;
        
        inverted_inputs.push_back(temp_trans);
        
        temp_trans.src    = "GND";
        temp_trans.type   = "NMOS";
        temp_trans.name = "M" + to_string(++P_Trans);
        
        transistors.push_back(temp_trans);
        
    }
    
    
}

bool CMOS::search_vector(vector<char>& x, char z){
    for(int i = 0 ; i < x.size(); i++) if(x[i] == z) return true;
    return false;
}


bool CMOS::isOperator(char c) {
    if (c == '&' || c == '|') {
        return true;
    }
    return false;
}


bool CMOS::isOperand(char c) {
    if (!isOperator(c) && c != '(' && c != ')') {
        return true;
    }
    return false;
}

int CMOS::compareOperators(char operator1, char operator2) {
    if ((operator1 == '&') && (operator2 == '|')) { return -1; }
    else if ((operator1 == '|' ) && (operator2 == '&')) { return 1; }
    return 0;
}



int  CMOS::identifySymbol(char inputchar)
{ //for checking symbol is operand or operator
    if (inputchar == '&' || inputchar == '|' )
        return (-1);
    else if (inputchar >= 'a' || inputchar <= 'z')
        return (1);
    else if (inputchar >= 'A' || inputchar <= 'Z')
        return (1);
    
    return -1;
}

string CMOS::infix_postfix_converter(string infix_expression){
    
    string postfix_expression = "";
    stack<char> operatorsStack;
    
    for(int i = 0 ; i < infix_expression.size(); i++){
        
        if (isOperator(infix_expression[i])) {
            while (!operatorsStack.empty() && operatorsStack.top() != '(' && compareOperators(operatorsStack.top(),infix_expression[i]) <= 0) {
                postfix_expression += operatorsStack.top();
                operatorsStack.pop();
            }
            operatorsStack.push(infix_expression[i]);
        }

        else if (isOperand(infix_expression[i])) {
            
            if(i < infix_expression.size() -1 && infix_expression[i + 1] == '\''){
                postfix_expression +=  infix_expression[i] ;
                postfix_expression +=  "\'";
                i++;
            }else
                postfix_expression += infix_expression[i];
            
        }
        
        else if (infix_expression[i] == '(') { operatorsStack.push(infix_expression[i]); }
        else if (infix_expression[i] == ')') {
            while (!operatorsStack.empty()) {
                if (operatorsStack.top() == '(') { operatorsStack.pop(); break; }
                postfix_expression += operatorsStack.top();
                operatorsStack.pop();
            }
            if(i < infix_expression.size() -1 && infix_expression[i + 1] == '\''){
                postfix_expression +=  "\'";
                i++;
            }
        }
    }
    
    while (!operatorsStack.empty()) {
        postfix_expression += operatorsStack.top();
        operatorsStack.pop();
    }

    
    return postfix_expression;
    
}

void CMOS::push(node *tree, node* arr[])
{
    top++;
    arr[top] = tree;
}

CMOS::node* CMOS::pop(node* arr[])
{
    top--;
    return (arr[top + 1]);
}


int CMOS::create_expression_tree(string postfix_expression, node* arr[])
{
    char symbol;
    node *newl, *ptr1, *ptr2;
    int isOperand;
    int counter = 0;
    
    for (int i = 0; i < postfix_expression.size(); i++)
    { //continue till end of the expr.
        symbol = postfix_expression[i];
        isOperand = identifySymbol(symbol); //check symbol is operand or operator.
        if (isOperand == 1)//if symbol is operand.
        {
            newl = new node;
            newl->data = symbol;
            newl->left = NULL;
            newl->right = NULL;
            newl->operand = true;
            newl->inverted = (i < postfix_expression.size() - 1 && postfix_expression[i+1] == '\'');
            if(newl->inverted) i++;
            push(newl,arr);
            counter++;
        }
        else
        { //If the symbol is operator//pop two top elements.
            ptr1 = pop(arr);
            ptr2 = pop(arr);
            newl = new node;
            newl->data = symbol;
            newl->left = ptr2;
            newl->right = ptr1;
            newl->operand = false;
            newl->inverted = (i < postfix_expression.size() - 1 && postfix_expression[i+1] == '\'');
            if(newl->inverted) i++;
            push(newl,arr);
            
        }
    }
    
    return counter;
    
}


void CMOS::apply_demorgan(node *tree,bool inv){
    if(tree->operand){
        tree->inverted = (inv)? !tree->inverted : tree->inverted;
    }else {
        if(tree->inverted && inv) {
            tree->inverted = false; inv = false;
        }else if(tree->inverted || inv){
            inv = true;
            if(tree->data == '|') tree->data = '&';
            else tree->data = '|';
            tree->inverted = false;
        }
        apply_demorgan(tree->left, inv);
        apply_demorgan(tree->right, inv);
    }
}


int CMOS::get_max_height(node* tree){
    
    if(tree->operand) return 1;
    
    if(tree->data == '|') return max(get_max_height(tree->left), get_max_height(tree->right));
    
    if(tree->data == '&') return (get_max_height(tree->left) + get_max_height(tree->right));
    
    return 0;
}


void CMOS::apply_sizing(node*tree, float sizing){
    
    if(tree->operand) {
        tree->ratio = sizing;
        return;
    }
    
    if(tree->data == '|'){
        apply_sizing(tree->left, sizing);
        apply_sizing(tree->right, sizing);
        return;
    }else if(tree->data == '&'){
        
        float left = get_max_height(tree->left);
        float right = get_max_height(tree->right);
        float sum = left+right;
        apply_sizing(tree->left, sum*sizing/left);
        apply_sizing(tree->right, sum*sizing/right);
    }
    
    
}

int CMOS::find_closing_bracket(string& x, int y){
    
    int z = -1, counter = 1;
    
    for(int i = y +1 ; i < x.length() ; i++){
        
        if(x[i] == '(') counter++;
        
        if(x[i] == ')') counter --;
        
        if(counter == 0) return i;
    }
    
    
    
    return z;
    
    
}

void CMOS::apply_demorgan_to_string(string& x, int y, int end, bool inv){
    
    for(int i = y ; i < end; i++){
        
        
        if(x[i] == '&' && inv) x[i] = '|';
        else if(x[i] == '|' && inv) x[i] = '&';
        else if(((x[i] >= 'a' && x[i] <= 'z') || (x[i] >= 'A' && x[i] <= 'Z')) && inv){
            
            if(i < end - 1 && x[i+1] == '\''){
                x.erase(i+1,1);
                end--;
            }else {
                x.insert(i+1, "'");
                end++;
                i++;
            }
            
        }
        else if(x[i] == '('){
            int index = find_closing_bracket(x, i);
            if(inv){
                if(index < end - 1 && x[index+1] == '\''){
                    x.erase(index+1,1);
                    i = index;
                }else{
                    apply_demorgan_to_string(x, i+1, index, true);
                    i = index;
                }
                
            }else {
                
                if(index < end - 1 && x[index+1] == '\''){
                    x.erase(index+1,1);
                    apply_demorgan_to_string(x, i+1, index, true);
                    i = index;
                }
                
            }
            
        }
        
    }
    
    
}







//
//  CMOS.hpp
//  Microelectronics
//
//  Created by Ahmad Refaat on 12/4/17.
//  Copyright © 2017 Ahmad Refaat. All rights reserved.
//

#ifndef CMOS_hpp
#define CMOS_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

class CMOS{
    
public:

    CMOS(string);
    CMOS(string,int,int,int);
    
    void print();
    
private:
    
    struct node{
        char data;
        bool inverted = false;
        bool operand;
        node *left;
        node *right;
        float ratio;
    };
    
    struct transistor{
        string name;
        string src;
        string drain;
        string gate;
        string type;
        float sizing;
    };
    
    
    const int PMOS = 0, NMOS = 1;
    bool valid,sizing_bool;
    
    string finalOutput;
    // Expression Tree Variables
    int top = -1;
    node *arr1[35];
    node *arr2[35];
    
    // INFIX-POSTFIX Variables
    string infix;
    string postfix;
    
    // Transistor Variables
    vector<transistor> PUN, PDN, inverted_inputs;
    vector<char> to_be_inverted;
    int P_Wires = 0, P_Trans = 0;
    transistor temp_trans;
    int N,L,P;
    
    
    //HELPER FUNCTIONS
    string infix_postfix_converter(string);
    bool isOperator(char);
    bool isOperand(char);
    int compareOperators(char, char);
    int identifySymbol(char);

    
    void push(node*,node* arr[]);
    node *pop(node* arr[]);
    
    
    
    
    //Creating Expression Tree
    int create_expression_tree(string,node* arr[]);
    
    //Simplification according to the NOT Operator
    void apply_demorgan(node*,bool);
    
    //Size Application
    int get_max_height(node* tree);
    void apply_sizing(node*tree, float sizing);
    
    
    void create_inverters(vector<transistor>&, vector<char>&);
    
    
    //Generating PDN & PUN
    string create_transistors_from_tree(node* tree, string drain, string src, vector<transistor>& all_trans, string type);
    bool search_vector(vector<char>& x, char z);
    void generateCircuit();

    
    int find_closing_bracket(string& x, int y);
    void apply_demorgan_to_string(string& x, int y, int end, bool inv);
    
    //printing
    
    void print_transistors_normal(vector<transistor>&, bool);
    void print_transistors_sizing(vector<transistor>&, bool);
    
};
#endif /* CMOS_hpp */

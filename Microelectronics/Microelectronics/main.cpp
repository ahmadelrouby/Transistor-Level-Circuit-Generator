//
//  main.cpp
//  Microelectronics
//
//  Created by Ahmad Refaat on 12/4/17.
//  Copyright © 2017 Ahmad Refaat. All rights reserved.
//

#include <iostream>
#include <string>
#include "CMOS.hpp"
#include <sstream>

using namespace std;

int main(int argc, const char * argv[]) {
   
    
    cout << "Please enter the expression in one of these forms \na. Y=a' \nb. Y=a' L=2 n=2 p=4" << endl;
    
    string x;
    getline(cin, x);
    
    stringstream ss;
    ss.str(x);
    
    vector<string> equation;
    string temp;
    while(ss >> temp){
        equation.push_back(temp);
    }
    
   
    if(equation.size() == 1){
        
        CMOS circuit(equation[0]);
        circuit.print();
        
    }else if(equation.size() == 4){
        
        int p = 1 , l = 1, n = 1;
        
        for(int i = 1; i < 4; i++){
            if((equation[i][0] == 'L' || equation[i][0] == 'l') && equation[i][1] == '=')
                l = stoi(equation[i].substr(2));
            if((equation[i][0] == 'N' || equation[i][0] == 'n') && equation[i][1] == '=')
                n = stoi(equation[i].substr(2));
            if((equation[i][0] == 'P' || equation[i][0] == 'p') && equation[i][1] == '=')
                p = stoi(equation[i].substr(2));
        }
        
        
        CMOS circuit(equation[0], l, n, p);
        circuit.print();
        
        
    }else {
        
        cout << "Invalid equation" <<endl;
    }
    
    
    
    return 0;
}

//
//  test_msdscript.cpp
//  testMsdscript
//
//  Created by Ang Zhang on 2/14/22.
//

#include "test_msdscript.hpp"
#include "exec.h"
#include <time.h>


std::string random_expr_string(){
    //use clock as random generator seed
    srand(clock());

    //get 100 random numbers
    int randNum = rand() % 100;

    //20% for each of number, variable(3% for x, 3% for y, and 4% for rest), plus, mutilple and let
    if(randNum <35){
        return std::to_string(rand());
    }
    else if( randNum >=35 && randNum < 70){
        int varRand = rand() % 10;
        if(varRand <3){
            return "x";
        }
        else if(varRand >=3 && varRand < 5){
            return "y";
        }else{std::string var="";
            for(int i=rand() % 32; i-->0;){
                var+=rand() % 256;
            }
            return var;
        }
    }
    else if (randNum >= 70 && randNum < 80){
        return random_expr_string() + "+" + random_expr_string();
    }
    else if(randNum >= 80 && randNum< 90 ){
        return random_expr_string() + "*" + random_expr_string();
    }else{
        std::string word ="";
        for(int i=rand() % 32; i-->0;){
            word+=rand() % 256;
        }
        return "_let" + word + "=" +random_expr_string() + "_in" + random_expr_string();
    }
    
}

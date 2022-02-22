//
//  Val.cpp
//  msdscript
//
//  Created by Ang Zhang on 2/16/22.
//

#include "Val.hpp"
#include "expr.hpp"
#include <iostream>

 NumVal::NumVal(int val){
    this->val = val;
}

bool NumVal::equals(Val *other){
    NumVal *c = dynamic_cast<NumVal*>(other);
    if(c==nullptr){
        return false;
    }else{
        return (this->val == c->val);
    }
}


Val *NumVal::add_to(Val *other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if(other_num == NULL) {
        throw std::runtime_error("add of non-number");

    }
    return new NumVal(val +other_num->val);
}


Val *NumVal::mult_to(Val *other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if(other_val ==NULL){
        throw std::runtime_error("mult of non-number");
    }
    return new NumVal(val * other_num->val );
}


std::string NumVal::to_string(){
    std::string s = std::to_string(val);
    return s;
}

Expr *NumVal::to_expr(){
    return new NumExpr(this->val);
}

Val *BoolVal::add_to(Val *other_val){
    throw std::runtime_error("cannot add numbers");
}

Val *BoolVal::mult_to(Val *other_val){
    throw std::runtime_error("cannot mutiple numbers");
}

bool BoolVal::equals(Val *o){
    BoolVal *c = dynamic_cast<BoolVal*>(o);
    if(c == NULL){
        return false;
    }else{
        return (this->boolVal==c->boolVal);
    }
}

BoolVal::BoolVal(bool boolVal) {
    this->boolVal = boolVal;
}
    
Expr *BoolVal::to_expr(){
    return new BoolExpr(this->boolVal);
}

std::string BoolVal::to_string() {
    std::string str = std::to_string(this->boolVal);
    return str;
}








//
//  Val.hpp
//  msdscript
//
//  Created by Ang Zhang on 2/16/22.
//

#ifndef Val_hpp
#define Val_hpp

#include <iostream>
#include <stdio.h>
#include "pointer.h"
#include "env.hpp"

class Expr; //declaration class Expr to allow Val::to_expr's return to be Expr*
//class env;

CLASS(Val) {
public:
    virtual ~Val() {};
    virtual bool equals(PTR(Val) other) = 0;
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
    virtual PTR(Val) mult_to(PTR(Val) other_val) = 0;
    virtual std::string to_string() = 0;
    virtual PTR(Expr) to_expr() = 0;
};


class NumVal : public Val {
public:
    int val;
    NumVal(int val);
    bool equals(PTR(Val) other);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_to(PTR(Val) other_val);
    PTR(Val) call(PTR(Val) actual_arg);
    std::string to_string();
    PTR(Expr) to_expr();
};

class BoolVal: public Val {
public:
    bool boolVal;
    BoolVal (bool boolVal);
    bool equals(PTR(Val) o);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_to(PTR(Val) other_val);
    PTR(Val) call(PTR(Val) actual_arg);
    std::string to_string();
    PTR(Expr) to_expr();
};

class FunVal:public Val{
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;
    
    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);
    bool equals(PTR(Val) other);
    PTR(Val) add_to(PTR(Val) other_val);
    PTR(Val) mult_to(PTR(Val) other_val);
    PTR(Val) call(PTR(Val) actual_arg);
    std::string to_string();
    PTR(Expr) to_expr();
    
};
#endif /* Val_hpp */

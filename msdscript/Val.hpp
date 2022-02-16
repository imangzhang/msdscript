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
class Expr; //declaration class Expr to allow Val::to_expr's return to be Expr*

class Val {
public:
    virtual bool equals(Val *other) =0;
    virtual Val *add_to(Val *other_val) = 0;
    virtual Val *mult_to(Val *other_val)= 0;
    virtual std::string to_string() =0;
    virtual Expr * to_expr() =0;
};


class NumVal : public Val {
public:
    int val;
    NumVal(int val);
    bool equals(Val *other);
    Val *add_to(Val *other_val);
    virtual Val *mult_to(Val *other_val);
    std::string to_string();
    Expr * to_expr();
};

#endif /* Val_hpp */

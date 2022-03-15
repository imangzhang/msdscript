//
//  parse.hpp
//  msdscript
//
//  Created by Ang Zhang on 2/7/22.
//

#ifndef parse_hpp
#define parse_hpp


#include <stdio.h>
#include <iostream>
#include "pointer.h"

class Expr;

PTR(Expr) parse(std::istream &in);
void consume(std::istream &in, int expect);
PTR(Expr) parse_num(std::istream &in);
PTR(Expr) parse_expr(std::istream &in);
PTR(Expr) parse_addend(std::istream &in);
PTR(Expr) parse_multicand(std::istream &in);
PTR(Expr) parse_var(std::istream &in);
PTR(Expr) parse_let(std::istream &in);
void skip_whitespcace(std::istream &in);
PTR(Expr) parse_keyword(std::istream &in);
std::string parse_variable(std::istream &in, std::string sign);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);
PTR(Expr) parse_muli(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);
PTR(Expr) parse_inner(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);

#endif /* parse_hpp */

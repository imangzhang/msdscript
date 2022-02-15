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

class Expr;

Expr *parse(std::istream &in);
static void consume(std::istream &in, int expect);
static Expr *parse_num(std::istream &in);
static Expr *parse_expr(std::istream &in);
static Expr *parse_addend(std::istream &in);
static Expr *parse_multicand(std::istream &in);
static Expr *parse_var(std::istream &in);
static Expr *parse_let(std::istream &in);
static void skip_whitespcace(std::istream &in);
static std::string parse_keyword(std::istream &in);
static std::string parse_variable(std::istream &in, std::string sign);
Expr *parse_muli(std::istream &in);

#endif /* parse_hpp */

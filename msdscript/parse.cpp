//
//  parse.cpp
//  msdscript
//
//  Created by Ang Zhang on 2/7/22.
//

#include <iostream>
#include <sstream>
#include "parse.hpp"
#include "expr.hpp"
#include "catch.h"

//skip white space
static void skip_whitespcace(std::istream &in){
        while(1){
            int c = in.peek();
            if(!isspace(c)){
                break;
            }
            //else{
            consume(in, c);
            //}
        }
    }

//parsing string
static Expr* parse_str(std::string input){
    std::istringstream in(input);
    return parse(in);
}

Expr *parse(std::istream &in){
    skip_whitespcace(in);
    Expr *e = parse_expr(in);
    if(!in.eof()){
        throw std::runtime_error("unexpected input after expression");
    }
    return e;
}



static void consume(std::istream &in, int expect){
    int c = in.get();
    if(c != expect){
        throw std::runtime_error("consume mismatch");
    }
}



//parsing Numbers
Expr *parse_num(std::istream &in){
    int n = 0;
    bool negative = false;
    
    if(in.peek() == '-'){
        negative = true;
        consume(in, '-');
    }
    
    while(1){
        int c = in.peek();
        if(isdigit(c)){
            consume(in,c);
            n = n*10 + (c - '0');
        }else{
            break;
        }
    }
    if(negative){
        n = -n;
    }
    return new NumExpr(n);
}



//parsing Expressions
Expr *parse_expr(std::istream &in){
    Expr *e;
    e = parse_addend(in);
    skip_whitespcace(in);
    int c = in.peek();
    if (c == '+') {
      consume(in, '+');
      Expr *rhs = parse_expr(in);
      return new AddExpr(e, rhs);
    } else
      return e;
    }


Expr *parse_addend(std::istream &in){
    Expr *e;
    e = parse_multicand(in);
    skip_whitespcace(in);
    int c = in.peek();
    if (c == '*') {
      consume(in, '*');
      Expr *rhs = parse_addend(in);
      return new MultExpr(e, rhs);
    } else
      return e;
    }


//Parsing muticand
static Expr *parse_multicand(std::istream &in){
    skip_whitespcace(in);
      int c = in.peek();
      if ((c == '-') || isdigit(c))
        return parse_num(in);
      else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
          skip_whitespcace(in);
        c = in.get();
        if (c != ')')
          throw std::runtime_error("missing close parenthesis");
        return e;
      }else if(isalpha(c)){
          return parse_var(in);
      }
      else if(c == '_'){
          return parse_let(in);
      }
      else {
        consume(in, c);
        throw std::runtime_error("invalid input");
      }
}


//parse var
static Expr *parse_var(std::istream &in){
    skip_whitespcace(in);

    std::string variable;
    int c=in.peek();
    while (isalpha(c)) {
        char character = in.get();
        variable =variable+character;
        skip_whitespcace(in);
        c = in.peek();
    }
    return new VarExpr(variable);
    
}
//parse keyword
static std::string parse_keyword(std::istream &in){
      consume(in, '_');
      std::string string;;
      int c = in.peek();
      while(isalpha(c)){
         char character = in.get();
          string = string + character;
          c= in.peek();
      }
    return string;
}

//parsing let
//grammer:  _let<variable> = <expr>_in<expr>
static Expr *parse_let(std::istream &in){
        int c = in.peek();
            if(c == '_'){
                parse_keyword(in);
            }else{
                throw std::runtime_error("invalid input");
            }
       std::string s;
       Expr *var = parse_var(in);
    VarExpr* v = dynamic_cast<VarExpr *>(var);
       s+=v->variable;
        c = in.peek();
           if(c == '='){
               consume(in, c);
           }
           else{
               throw std::runtime_error("we need a = sign");
           }
        Expr* expr1;
        Expr* expr2;    
        expr1 = parse_expr(in);
        c = in.peek();
           if(c == '_'){
               parse_keyword(in);
           }
           else{
               throw std::runtime_error("invalid input");
           }
        expr2 = parse_expr(in);
        return new _letExpr(s, expr1, expr2);
        
}

TEST_CASE("TESTS"){
    SECTION("NUMBER")
        CHECK( parse_str("1")
              ->equals(new NumExpr(1)));
        CHECK( parse_str("(1)")
              ->equals(new NumExpr(1)));
        CHECK( parse_str("-1")
              ->equals(new NumExpr(-1)));
    
    SECTION("ADD")
        CHECK( parse_str("10+-20")
              ->equals(new AddExpr(new NumExpr(10), new NumExpr(-20))));
        CHECK( parse_str("10  +   -20")
          ->equals(new AddExpr(new NumExpr(10), new NumExpr(-20))));
        CHECK( parse_str("(5+1)")
              ->equals(new AddExpr(new NumExpr(5), new NumExpr(1))));
        CHECK( parse_str("(20)+10")
              ->equals(new AddExpr(new NumExpr(20), new NumExpr(10))));
        CHECK( parse_str("10+(20)")
              ->equals(new AddExpr(new NumExpr(10), new NumExpr(20))));
        CHECK( parse_str("    10+(  20   )")
              ->equals(new AddExpr(new NumExpr(10), new NumExpr(20))));
    
    SECTION("VAR")
        CHECK(parse_str("apple")->equals(new VarExpr("apple")));
        CHECK(parse_str("  apple")->equals(new VarExpr("apple")));
        CHECK(parse_str(" apple")->equals(new VarExpr("apple")));
        CHECK(parse_str("   apple  ")->equals(new VarExpr("apple")));
    
    SECTION("MUTIPLICATION")
        CHECK( parse_str ("10+20*60")
              ->equals(new AddExpr(new NumExpr(10),new MultExpr(new NumExpr(20), new NumExpr(60)))));
        CHECK( parse_str ("60*20+10")
              ->equals(new AddExpr(new MultExpr(new NumExpr(60), new NumExpr(20)),new NumExpr(10))));
        CHECK( parse_str ("60*    20  +    10")
              ->equals(new AddExpr(new MultExpr(new NumExpr(60), new NumExpr(20)),new NumExpr(10))));
        CHECK( parse_str ("60*20+10")
              ->equals(new AddExpr(new MultExpr(new NumExpr(60), new NumExpr(20)),new NumExpr(10))));
        CHECK( parse_str ("60*(20+10)")
              ->equals(new MultExpr(new NumExpr(60),new AddExpr(new NumExpr(20), new NumExpr(10)))));
        CHECK( parse_str ("(60+40)*(20+10)")
              ->equals(new MultExpr(new AddExpr(new NumExpr(60), new NumExpr(40)),new AddExpr(new NumExpr(20), new NumExpr(10)))));
        CHECK( parse_str ("(60+40)*(20+10)+100")
              ->equals(new AddExpr(new MultExpr(new AddExpr(new NumExpr(60), new NumExpr(40)),new AddExpr(new NumExpr(20), new NumExpr(10))),new NumExpr(100))));

    SECTION("_let")
        CHECK(parse_str("_let x = 10"
                    "_in x + 10")
          ->equals(new _letExpr("x",new NumExpr(10), new AddExpr(new VarExpr("x"), new NumExpr(10)))));
        CHECK(parse_str("_let x = 10"
              "_in x * 10")
            ->equals(new _letExpr("x",new NumExpr(10), new MultExpr(new VarExpr("x"), new NumExpr(10)))));
        CHECK(parse_str("_let x = 10"
              "_in xyz")->equals(new _letExpr("x",new NumExpr(10), new VarExpr("xyz"))));
     
        CHECK(parse_str("_let x = 10\n"
              "_in (_let y = 20\n"
                 "_in y + 30) + x")
            ->equals(new _letExpr("x", new NumExpr(10), new AddExpr(new _letExpr("y", new NumExpr(20), new AddExpr(new VarExpr("y"), new NumExpr(30))), new VarExpr("x")))));
        CHECK(parse_str("10 * _let x = 20\n"
                "_in x + 30")
            ->equals(new MultExpr(new NumExpr(10), new _letExpr("x", new NumExpr(20), new AddExpr(new VarExpr("x"), new NumExpr(30))))));
    
        CHECK_THROWS_WITH(parse_str("_let x = a 10"
                                    "_in x + 10"),"invalid input");
        CHECK_THROWS_WITH(parse_str("_let x = 10"
                                    "_in x+(x + 10"),"missing close parenthesis");
        CHECK_THROWS_WITH(parse_str("_let x =+ 10"
                                    "_in x + 10"),"invalid input");
        CHECK_THROWS_WITH(parse_str("_let x = 1 * a + 10)"
                                    "(_in x + 10"),"invalid input");
        CHECK_THROWS_WITH(parse_str("_let x  1 * 10)"
                                    "_in x + 10"),"we need a = sign");
        CHECK_THROWS_WITH(parse_str("_hi x = 1 * 10)"
                                    "_in x + 10"),"invalid input");
        CHECK_THROWS_WITH(parse_str("_let x = 1 * 10)"
                                    "_xxxx x + 10"),"invalid input");

}




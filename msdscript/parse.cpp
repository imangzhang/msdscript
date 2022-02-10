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
    return new Num(n);
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
      return new Add(e, rhs);
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
      return new Mult(e, rhs);
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
//      else if(c == '_'){
//          return parse_let(in);
//      }
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
    return new Var(variable);
    
}


    static std::string parse_variable(std::istream &in, std::string sign){
        std::string s =sign;
        int c=in.peek();
        while (isalpha(c)) {
            char character = in.get();
            s =s+character;
            skip_whitespcace(in);
            c = in.peek();
        }
        return s;
    }

//parse keyword
    static std::string parse_keyword(std::istream &in){
    in.get();
    return parse_variable(in, "_");
}
 
    

//parsing let
//grammer:  _let<variable> = <expr>_in<expr>

static Expr *parse_let(std::istream &in){
    
    skip_whitespcace(in);
    std::string name = parse_variable(in, "");
    char c = in.get();
    skip_whitespcace(in);
    Expr* expr = parse_expr(in);
    skip_whitespcace(in);
    std::string keyword = parse_keyword(in);
    skip_whitespcace(in);
    Expr* expr2 = parse_expr(in);

    Expr* let = new _let(name,expr,expr2);
    return let;
    
   
}

TEST_CASE("TESTS"){
    SECTION("NUMBER")
        CHECK( parse_str("1")
              ->equals(new Num(1)));
        CHECK( parse_str("(1)")
              ->equals(new Num(1)));
        CHECK( parse_str("-1")
              ->equals(new Num(-1)));
    
    SECTION("ADD")
        CHECK( parse_str("10+-20")
              ->equals(new Add(new Num(10), new Num(-20))));
        CHECK( parse_str("10  +   -20")
          ->equals(new Add(new Num(10), new Num(-20))));
        CHECK( parse_str("(5+1)")
              ->equals(new Add(new Num(5), new Num(1))));
        CHECK( parse_str("(20)+10")
              ->equals(new Add(new Num(20), new Num(10))));
        CHECK( parse_str("10+(20)")
              ->equals(new Add(new Num(10), new Num(20))));
        CHECK( parse_str("    10+(  20   )")
              ->equals(new Add(new Num(10), new Num(20))));
    
    SECTION("VAR")
        CHECK(parse_str("apple")->equals(new Var("apple")));
        CHECK(parse_str("  apple")->equals(new Var("apple")));
        CHECK(parse_str(" apple")->equals(new Var("apple")));
        CHECK(parse_str("   apple  ")->equals(new Var("apple")));
    
    SECTION("MUTIPLICATION")
        CHECK( parse_str ("10+20*60")
              ->equals(new Add(new Num(10),new Mult(new Num(20), new Num(60)))));
        CHECK( parse_str ("60*20+10")
              ->equals(new Add(new Mult(new Num(60), new Num(20)),new Num(10))));
        CHECK( parse_str ("60*    20  +    10")
              ->equals(new Add(new Mult(new Num(60), new Num(20)),new Num(10))));
        CHECK( parse_str ("60*20+10")
              ->equals(new Add(new Mult(new Num(60), new Num(20)),new Num(10))));
        CHECK( parse_str ("60*(20+10)")
              ->equals(new Mult(new Num(60),new Add(new Num(20), new Num(10)))));
        CHECK( parse_str ("(60+40)*(20+10)")
              ->equals(new Mult(new Add(new Num(60), new Num(40)),new Add(new Num(20), new Num(10)))));
        CHECK( parse_str ("(60+40)*(20+10)+100")
              ->equals(new Add(new Mult(new Add(new Num(60), new Num(40)),new Add(new Num(20), new Num(10))),new Num(100))));

    SECTION("_let")
        CHECK(parse_str("_let x = 10"
                    "_in x + 10")
          ->equals(new _let("x",new Num(10), new Add(new Var("x"), new Num(10)))));
        CHECK(parse_str("_let x = 10"
              "_in x * 10")
            ->equals(new _let("x",new Num(3), new Mult(new Var("x"), new Num(3)))));
        CHECK(parse_str("_let x = 10"
              "_in xyz")->equals(new _let("x",new Num(10), new Var("xyz"))));
     
        CHECK(parse_str("_let x = 10\n"
              "_in (_let y = 20\n"
                 "_in y + 30) + x")
            ->equals(new _let("x", new Num(10), new Add(new _let("y", new Num(20), new Add(new Var("y"), new Num(30))), new Var("x")))));
        CHECK(parse_str("10 * _let x = 20\n"
                "_in x + 30")
            ->equals(new Mult(new Num(10), new _let("x", new Num(20), new Add(new Var("x"), new Num(30))))));
}




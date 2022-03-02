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
 void skip_whitespcace(std::istream &in){
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
 Expr* parse_str(std::string input){
    std::istringstream in(input);
    return parse(in);
}

Expr *parse(std::istream &in){
    
    Expr *e = parse_expr(in);
    skip_whitespcace(in);
    
    if(!in.eof()){
        throw std::runtime_error("unexpected input after expression");
    }
    return e;
}



 void consume(std::istream &in, int expect){
    int c = in.get();
    if(c != expect){
        throw std::runtime_error("consume mismatch");
    }
     assert(c == expect);
}



//parsing Numbers
Expr *parse_num(std::istream &in){
    int n = 0;
    bool negative = false;

    if(in.peek() == '-'){
        negative = true;
        consume(in, '-');
        int n = in.peek();
        if (!isdigit(n)) {
            throw std::runtime_error("Dangling negative.");
        }
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

Expr *parse_comparg(std::istream &in){
    Expr *e;
    e = parse_addend(in);
    skip_whitespcace(in);
    int c = in.peek();
    if (c == '+') {
      consume(in, '+');
      Expr *rhs = parse_comparg(in);
      return new AddExpr(e, rhs);
    } else
      return e;
    }


Expr *parse_expr(std::istream &in){
    Expr *e;
    e= parse_comparg(in);
    skip_whitespcace(in);
    int c = in.peek();
    int d = 0;
    if (c == '=') {
        consume(in, '=');
        d = in.peek();
    }
    if(d == '='){
        consume(in, '=');
        Expr *rhs = parse_expr(in);
        return new EqualExpr(e,rhs);
    }
    else{
        return e;
    }
};


Expr *parse_addend(std::istream &in){
    Expr *e;
    e = parse_multicand(in);
    skip_whitespcace(in);
    int c = in.peek();
    if (c == '*') {
      consume(in, '*');
      Expr *rhs = parse_addend(in);
      return new MultExpr(e, rhs);
    } else{
      return e;
    }
}

//Parsing muticand
 Expr *parse_inner(std::istream &in){
    skip_whitespcace(in);
      int c = in.peek();
      if ((c == '-') || isdigit(c))
        return parse_num(in);
      else if (c == '(') {
        consume(in, '(');
          skip_whitespcace(in);
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
          return parse_keyword(in);
      }
      else {
        consume(in, c);
        throw std::runtime_error("invalid input");
      }
}

Expr *parse_multicand(std::istream &in){
    Expr *expr = parse_inner(in);
    while(in.peek() == '('){
        consume(in, '(');
        Expr *actual_arg = parse_expr(in);
        consume(in, ')');
        expr = new (CallExpr)(expr, actual_arg);
        
    }
    return expr;
}

//parse var
 Expr *parse_var(std::istream &in){

    std::string variable = "";
    
    while (1) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            char character = (char)c;
            variable += character;
        }
        else {
            break;
        }
    }
    return new VarExpr(variable);

}

Expr *parse_if(std::istream &in)
{
    Expr *_ifExpr;
    Expr *_thenVar;
    Expr *_thenExpr;
    Expr *_elseVar;
    Expr *_elseExpr;
  
    _ifExpr = parse_expr(in);
    skip_whitespcace(in);
    consume(in, '_');
    _thenVar = parse_var(in);
    if (!(_thenVar -> to_string_cmd() == "then")){
         throw std::runtime_error("invalid input of 'then'");
     }
    _thenExpr = parse_expr(in);
    skip_whitespcace(in);
    consume(in, '_');
    _elseVar = parse_var(in);
     if (!(_elseVar -> to_string_cmd() == "else")){
         throw std::runtime_error("invalid input of 'else'");
     }
    _elseExpr = parse_expr(in);
    
     return new IfExpr(_ifExpr, _thenExpr, _elseExpr);
    
}

//parse keyword
Expr* parse_keyword(std::istream &in){
    consume(in, '_');
    Expr *keyword = parse_var(in);
    skip_whitespcace(in);
    if(keyword ->to_string_cmd() == "let"){
        Expr *_let = parse_let(in);
        return _let;
    }
    else if ((keyword ->to_string_cmd()=="if")){
        Expr *ifExpr = parse_if(in);
        return ifExpr;
        
    }
    else if(keyword ->to_string_cmd() =="true"){
        return new BoolExpr("_true");
    }
    else if(keyword ->to_string_cmd() =="false"){
        return new BoolExpr("_false");
    }else if(keyword->to_string_cmd() == "fun"){
        Expr *funExpr = parse_fun(in);
        return funExpr;
    }
    else{
        throw std::runtime_error("invalid keyword");
    }
    
}

//parsing let
//grammer:  _let<variable> = <expr>_in<expr>
Expr *parse_let(std::istream &in){
    
      std::string lhsStr;
      Expr *lhsExpr;
      Expr *rhs;
      Expr *body;

      lhsExpr = parse_var(in);
      VarExpr* lhs = dynamic_cast<VarExpr *>(lhsExpr);
      lhsStr += lhs->variable;
      skip_whitespcace(in);
      consume(in, '=');
      rhs = parse_expr(in);
      skip_whitespcace(in);
      consume(in, '_');
      Expr *kw = parse_var(in);
      if (!(kw ->to_string_cmd()== "in")){
        throw std::runtime_error("invalid input of 'in'");
      }
      body = parse_expr(in);
      return new _letExpr(lhsStr, rhs, body);
    }

Expr *parse_fun(std::istream &in){
    
    std::string formal_arg_str;
    Expr *formal_arg;
    Expr *body;
        
    consume(in, '(');
    formal_arg = parse_var(in);
    VarExpr* varExpr = dynamic_cast<VarExpr *>(formal_arg);
    formal_arg_str += varExpr->variable;
    consume(in, ')');
    body = parse_expr(in);
    return new FunExpr(formal_arg_str, body);
}

//****----------------------------------------TESTS----------------------------------------

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
        CHECK( parse_str("_let x = x + 1\n"
                         "_in x + 2")
              -> equals (new _letExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1)), new AddExpr(new VarExpr("x"), new NumExpr(2)))) );
        CHECK( parse_str("(_let x = 10\n"
              " _in x + 2) * 2 * 4")
              -> equals (new MultExpr((new _letExpr("x", new NumExpr(10), new AddExpr(new VarExpr("x"), new NumExpr(2)))), new MultExpr(new NumExpr(2), new NumExpr(4)))) );

        CHECK( parse_str(" _if \n  _true \n _then a\n  _else  b\n")
              -> equals (new IfExpr(new BoolExpr("_true"), new VarExpr("a"), new VarExpr("b"))) );

        CHECK( parse_str("(_if _true _then a _else b)")
              -> equals (new IfExpr(new BoolExpr("_true"), new VarExpr("a"), new VarExpr("b"))) );

        CHECK( parse_str("(_if (_true) _then ((a)) _else (b))")
              -> equals (new IfExpr(new BoolExpr("_true"), new VarExpr("a"), new VarExpr("b"))) );

        CHECK( parse_str("_if_true_then 10  _else60 ")
              -> equals (new IfExpr(new BoolExpr("_true"), new NumExpr(10), new NumExpr(60)))
                        );
    CHECK( parse_str("_if _false _then 10  _else 60 ")
          -> equals (new IfExpr(new BoolExpr("_true"), new NumExpr(10), new NumExpr(60)))
                    );
         CHECK( parse_str("_if a _then b _else (c)")
               -> equals (new IfExpr(new VarExpr("a"), new VarExpr("b"), new VarExpr("c"))) );
    CHECK(parse_str("1 == 1")->equals(new EqualExpr(new NumExpr(1), new NumExpr(1))));
    
    CHECK(parse_str("_fun (x) (x+10)")-> equals(new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(10)))) );
       
      CHECK(parse_str("x(10)") -> equals(new CallExpr(new VarExpr("x"), new NumExpr(10))));
    
    
SECTION("ERROR TESTS")
    CHECK_THROWS_WITH(parse_str("- 1"),"Dangling negative.");
     CHECK_THROWS_WITH(parse_str("_let x = a 10"
                                "_in x + 10"),"consume mismatch");
     CHECK_THROWS_WITH(parse_str("_let x = 10"
                                "_in x+(x + 10"),"missing close parenthesis");
     CHECK_THROWS_WITH(parse_str("_let x =+ 10"
                                "_in x + 10"),"invalid input");
     CHECK_THROWS_WITH(parse_str("_let x = 1 * a + 10)"
                                "(_in x + 10"),"consume mismatch");
     CHECK_THROWS_WITH(parse_str("_let x  1 * 10)"
                                "_in x + 10"),"consume mismatch");
     CHECK_THROWS_WITH(parse_str("_hi x = 1 * 10)"
                                "_in x + 10"),"invalid keyword");
     CHECK_THROWS_WITH(parse_str("_let x = 1 * 10)"
                                "_xxxx x + 10"),"consume mismatch");
     CHECK_THROWS_WITH( parse_str("(_let x = x + 1\n"
                    "_in (x + 3) * 1"), "missing close parenthesis");
     CHECK_THROWS_WITH( parse_str("_let x = x + 1\n"
                    "_in (x + 3) * 1)"), "unexpected input after expression");
     CHECK_THROWS_WITH( parse_str("_lot x = x + 1\n"
                    "_in (x + 3) * 1)"), "invalid keyword");
     CHECK_THROWS_WITH( parse_str("_let x = x + 1\n"
                    "_on (x + 3) * 1)"), "invalid input of 'in'");
     CHECK_THROWS_WITH( parse_str("_let x = x + 1\n"
                    "(x + 3) * 1)"), "consume mismatch");
     CHECK_THROWS_WITH( parse_str("(_let x x + 1\n"
                    "_in (x + 3) * 1"), "consume mismatch");
     CHECK_THROWS_WITH( parse_str("(_let x == x + 1\n"
                    "_in (x + 3) * 1"), "invalid input");
     CHECK_THROWS_WITH( parse_str("_iffffff _true _then a _else b"),
                      "invalid keyword");
     CHECK_THROWS_WITH( parse_str("_if _true _xxxx x _else a"),
                      "invalid input of 'then'");
     CHECK_THROWS_WITH( parse_str("_if _true zz _else b"),
                      "consume mismatch");
     CHECK_THROWS_WITH( parse_str("_if _true _then x _ese y"),
                      "invalid input of 'else'");
     CHECK_THROWS_WITH( parse_str("_if _true _then abab yyyy"),
                      "consume mismatch");
     CHECK_THROWS_WITH( parse_str("_if = true then a _else b"),
                      "invalid input");
     CHECK_THROWS_WITH( parse_str("(_if _true _then a _else b"),
                      "missing close parenthesis");
     CHECK_THROWS_WITH( parse_str("_if _true _then a _else b)"),
                      "unexpected input after expression");
}




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
#include "env.hpp"
#include "Val.hpp"
#include "env.hpp"


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
PTR(Expr) parse_str(std::string input){
    std::istringstream in(input);
    return parse(in);
}

PTR(Expr) parse(std::istream &in){
    
    PTR(Expr) e = parse_expr(in);
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
PTR(Expr) parse_num(std::istream &in){
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
    return NEW(NumExpr)(n);
}

PTR(Expr) parse_comparg(std::istream &in){
    PTR(Expr) e;
    e = parse_addend(in);
    skip_whitespcace(in);
    int c = in.peek();
    if (c == '+') {
      consume(in, '+');
        PTR(Expr) rhs = parse_comparg(in);
      return NEW (AddExpr)(e, rhs);
    } else
      return e;
    }


PTR(Expr) parse_expr(std::istream &in){
    PTR(Expr) e;
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
        PTR(Expr) rhs = parse_expr(in);
        return NEW (EqualExpr)(e,rhs);
    }
    else{
        return e;
    }
};


PTR(Expr) parse_addend(std::istream &in){
    PTR(Expr) e;
    e = parse_multicand(in);
    skip_whitespcace(in);
    int c = in.peek();
    if (c == '*') {
      consume(in, '*');
        PTR(Expr) rhs = parse_addend(in);
      return NEW (MultExpr)(e, rhs);
    } else{
      return e;
    }
}

//Parsing muticand
PTR(Expr) parse_inner(std::istream &in){
    skip_whitespcace(in);
      int c = in.peek();
      if ((c == '-') || isdigit(c))
        return parse_num(in);
      else if (c == '(') {
        consume(in, '(');
          skip_whitespcace(in);
          PTR(Expr) e = parse_expr(in);
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

PTR(Expr) parse_multicand(std::istream &in){
    PTR(Expr) expr = parse_inner(in);
    skip_whitespcace(in);
    while(in.peek() == '('){
        consume(in, '(');
        PTR(Expr) actual_arg = parse_expr(in);
        consume(in, ')');
        expr = NEW (CallExpr)(expr, actual_arg);
        
    }
    return expr;
}

//parse var
PTR(Expr) parse_var(std::istream &in){

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
    return NEW (VarExpr)(variable);

}

PTR(Expr) parse_if(std::istream &in)
{
    PTR(Expr) _ifExpr;
    PTR(Expr) _thenVar;
    PTR(Expr) _thenExpr;
    PTR(Expr) _elseVar;
    PTR(Expr) _elseExpr;
  
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
    skip_whitespcace(in);
    _elseExpr = parse_expr(in);
    
     return NEW (IfExpr)(_ifExpr, _thenExpr, _elseExpr);
    
}

//parse keyword
PTR(Expr) parse_keyword(std::istream &in){
    consume(in, '_');
    PTR(Expr) keyword = parse_var(in);
    skip_whitespcace(in);
    if(keyword ->to_string_cmd() == "let"){
        PTR(Expr) _let = parse_let(in);
        return _let;
    }
    else if(keyword ->to_string_cmd() =="true"){
        return NEW (BoolExpr)(true);
    }
    else if(keyword ->to_string_cmd() =="false"){
        return NEW (BoolExpr)(false);
    }
    else if ((keyword ->to_string_cmd()=="if")){
        PTR(Expr) ifExpr = parse_if(in);
        return ifExpr;
    }else if(keyword->to_string_cmd() == "fun"){
        PTR(Expr) funExpr = parse_fun(in);
        return funExpr;
    }
    else{
        throw std::runtime_error("invalid keyword");
    }
}

//parsing let
//grammer:  _let<variable> = <expr>_in<expr>
PTR(Expr) parse_let(std::istream &in){
    
      std::string lhsStr;
    PTR(Expr) lhsExpr;
    PTR(Expr) rhs;
    PTR(Expr) body;

      lhsExpr = parse_var(in);
    PTR(VarExpr) lhs = CAST(VarExpr)(lhsExpr);
      lhsStr += lhs->variable;
    
      skip_whitespcace(in);
      consume(in, '=');
    
      rhs = parse_expr(in);
      skip_whitespcace(in);
      consume(in, '_');
    PTR(Expr) kw = parse_var(in);
      if (!(kw ->to_string_cmd()== "in")){
        throw std::runtime_error("invalid input of 'in'");
      }
    skip_whitespcace(in);
      body = parse_expr(in);
      return NEW (_letExpr)(lhsStr, rhs, body);
    }

PTR(Expr) parse_fun(std::istream &in){
    
    std::string formal_arg_str;
    PTR(Expr) formal_arg;
    PTR(Expr) body;

    consume(in, '(');
    formal_arg = parse_var(in);
    PTR(VarExpr) varExpr = CAST(VarExpr)(formal_arg);
    formal_arg_str += varExpr->variable;
    consume(in, ')');
    body = parse_expr(in);
    return NEW (FunExpr)(formal_arg_str, body);
    
//    skip_whitespcace(in);
//    consume(in, '(');
//    skip_whitespcace(in);
//    std::string formal_arg_str = parse_var(in)->to_string_cmd();
//    skip_whitespcace(in);
//    consume(in, ')');
//    skip_whitespcace(in);
//    PTR(Expr) body = parse_expr(in);
//    skip_whitespcace(in);
//    return NEW (FunExpr)(formal_arg_str, body);
    
}

//****----------------------------------------TESTS----------------------------------------

TEST_CASE("TESTS"){
    SECTION("NUMBER")
        CHECK( parse_str("1")
              ->equals(NEW (NumExpr)(1)));
        CHECK( parse_str("(1)")
              ->equals(NEW (NumExpr)(1)));
        CHECK( parse_str("-1")
              ->equals(NEW (NumExpr)(-1)));

    SECTION("ADD")
        CHECK( parse_str("10+-20")
              ->equals(NEW (AddExpr)(NEW (NumExpr)(10), NEW (NumExpr)(-20))));
        CHECK( parse_str("10  +   -20")
          ->equals(NEW (AddExpr)(NEW (NumExpr)(10), NEW (NumExpr)(-20))));
        CHECK( parse_str("(5+1)")
              ->equals(NEW (AddExpr)(NEW (NumExpr)(5), NEW (NumExpr)(1))));
        CHECK( parse_str("(20)+10")
              ->equals(NEW (AddExpr)(NEW (NumExpr)(20), NEW (NumExpr)(10))));
        CHECK( parse_str("10+(20)")
              ->equals(NEW (AddExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20))));
        CHECK( parse_str("    10+(  20   )")
              ->equals(NEW (AddExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20))));

    SECTION("VAR")
        CHECK(parse_str("apple")->equals(NEW (VarExpr)("apple")));
        CHECK(parse_str("  apple")->equals(NEW (VarExpr)("apple")));
        CHECK(parse_str(" apple")->equals(NEW (VarExpr)("apple")));
        CHECK(parse_str("   apple  ")->equals(NEW (VarExpr)("apple")));

    SECTION("MUTIPLICATION")
        CHECK( parse_str ("10+20*60")
              ->equals(NEW (AddExpr)(NEW (NumExpr)(10),NEW (MultExpr)(NEW (NumExpr)(20), NEW (NumExpr)(60)))));
        CHECK( parse_str ("60*20+10")
              ->equals(NEW (AddExpr)(NEW (MultExpr)(NEW (NumExpr)(60), NEW (NumExpr)(20)),NEW (NumExpr)(10))));
        CHECK( parse_str ("60*    20  +    10")
              ->equals(NEW (AddExpr)(NEW (MultExpr)(NEW (NumExpr)(60), NEW (NumExpr)(20)),NEW (NumExpr)(10))));
        CHECK( parse_str ("60*20+10")
              ->equals(NEW (AddExpr)(NEW (MultExpr)(NEW (NumExpr)(60), NEW (NumExpr)(20)),NEW (NumExpr)(10))));
        CHECK( parse_str ("60*(20+10)")
              ->equals(NEW (MultExpr)(NEW (NumExpr)(60),NEW (AddExpr)(NEW (NumExpr)(20), NEW (NumExpr)(10)))));
        CHECK( parse_str ("(60+40)*(20+10)")
              ->equals(NEW (MultExpr)(NEW (AddExpr)(NEW (NumExpr)(60), NEW (NumExpr)(40)),NEW (AddExpr)(NEW (NumExpr)(20), NEW (NumExpr)(10)))));
        CHECK( parse_str ("(60+40)*(20+10)+100")
              ->equals(NEW (AddExpr)(NEW (MultExpr)(NEW (AddExpr)(NEW (NumExpr)(60), NEW (NumExpr)(40)),NEW (AddExpr)(NEW (NumExpr)(20), NEW (NumExpr)(10))),NEW (NumExpr)(100))));

    SECTION("_let")
        CHECK(parse_str("_let x = 10"
                    "_in x + 10")
          ->equals(NEW (_letExpr)("x",NEW (NumExpr)(10), NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10)))));
        CHECK(parse_str("_let x = 10"
              "_in x * 10")
            ->equals(NEW (_letExpr)("x",NEW (NumExpr)(10), NEW (MultExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10)))));
        CHECK(parse_str("_let x = 10"
              "_in xyz")->equals(NEW (_letExpr)("x",NEW (NumExpr)(10), NEW (VarExpr)("xyz"))));

        CHECK(parse_str("_let x = 10\n"
              "_in (_let y = 20\n"
                 "_in y + 30) + x")
            ->equals(NEW (_letExpr)("x", NEW (NumExpr)(10), NEW (AddExpr)(NEW (_letExpr)("y", NEW (NumExpr)(20), NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(30))), NEW (VarExpr)("x")))));
        CHECK(parse_str("10 * _let x = 20\n"
                "_in x + 30")
            ->equals(NEW (MultExpr)(NEW (NumExpr)(10), NEW (_letExpr)("x", NEW (NumExpr)(20), NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(30))))));
        CHECK( parse_str("_let x = x + 1\n"
                         "_in x + 2")
              -> equals (NEW (_letExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1)), NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)))) );
        CHECK( parse_str("(_let x = 10\n"
              " _in x + 2) * 2 * 4")
              -> equals (NEW (MultExpr)((NEW (_letExpr)("x", NEW (NumExpr)(10), NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)))), NEW (MultExpr)(NEW (NumExpr)(2), NEW (NumExpr)(4)))) );

        CHECK( parse_str(" _if \n  _true \n _then a\n  _else  b\n")
              -> equals (NEW (IfExpr)(NEW (BoolExpr)("_true"), NEW (VarExpr)("a"), NEW (VarExpr)("b"))) );

        CHECK( parse_str("(_if _true _then a _else b)")
              -> equals (NEW (IfExpr)(NEW (BoolExpr)("_true"), NEW (VarExpr)("a"), NEW (VarExpr)("b"))) );

        CHECK( parse_str("(_if (_true) _then ((a)) _else (b))")
              -> equals (NEW (IfExpr)(NEW (BoolExpr)("_true"), NEW (VarExpr)("a"), NEW (VarExpr)("b"))) );

        CHECK( parse_str("_if_true_then 10  _else60 ")
              -> equals (NEW (IfExpr)(NEW (BoolExpr)("_true"), NEW (NumExpr)(10), NEW (NumExpr)(60)))
                        );
         CHECK( parse_str("_if a _then b _else (c)")
               -> equals (NEW (IfExpr)(NEW (VarExpr)("a"), NEW (VarExpr)("b"), NEW (VarExpr)("c"))) );
    
    CHECK(parse_str("1 == 1")->equals(NEW (EqualExpr)(NEW (NumExpr)(1), NEW (NumExpr)(1))));

    CHECK(parse_str("_fun (x) (x+10)")-> equals(NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10)))) );

      CHECK(parse_str("x(10)") -> equals(NEW (CallExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10))));


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

TEST_CASE("test"){
    
    CHECK( parse_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(5)")->interp(Env::empty)->equals(NEW(NumVal)(120)) );
    CHECK( parse_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(10)")->interp(Env::empty)->equals(NEW(NumVal)(3628800)) );
    CHECK( parse_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in _let factorial = factrl(factrl) _in factorial(5)")->interp(Env::empty)->equals(NEW(NumVal)(120)) );

}

//TEST_CASE("fib"){
//
//    CHECK(parse_str("_let pair = _fun (a) _fun (b)"
//    "              _fun(sel)"
//    "              _if sel _then a _else b"
//    "_in _let fst = _fun (p) p(_true)"
//    "_in _let snd = _fun (p) p(_false)"
//    "_in _let fib = _fun (fib)"
//    "                _fun (x)"
//    "                  _if x == 0"
//    "                  _then pair(1)(1)"
//    "                  _else _if x == 1"
//    "                        _then pair(1)(1)"
//    "                        _else _let p = fib(fib)(x + -1)"
//    "                              _in pair(fst(p) + snd(p))(fst(p))"
//              "_in  fst(fib(fib)(30))") ->interp(Env::empty)->equals(NEW(NumVal(1346269))));
//}
//
//TEST_CASE("fastfib") {
//        CHECK(parse_str("_let pair = _fun (a) _fun (b)"
//              "_fun(sel)"
//              "_if sel _then a _else b"
//              "_in _let fst = _fun (p) p(_true)"
//              "_in _let snd = _fun (p) p(_false)"
//              "_in _let fib = _fun (fib)"
//              "  _fun (x)"
//              "    _if x == 0"
//              "    _then pair(1)(1)"
//              "    _else _if x == 1"
//              "          _then pair(1)(1)"
//              "          _else _let p = fib(fib)(x + -1)"
//              "                _in pair(fst(p) + snd(p))(fst(p))"
//              "_in  fst(fib(fib)(30))")->interp(Env::empty)->equals(NEW(NumVal(1346269))));

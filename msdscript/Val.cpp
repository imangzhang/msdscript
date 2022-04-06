//
//  Val.cpp
//  msdscript
//
//  Created by Ang Zhang on 2/16/22.
//

#include "Val.hpp"
#include "expr.hpp"
#include <iostream>
#include "catch.h"

NumVal::NumVal(int val){
    this->val = val;
}

bool NumVal::equals(PTR(Val) other){
    PTR(NumVal) c = CAST(NumVal)(other);
    if(c==nullptr){
        return false;
    }else{
        return (this->val == c->val);
    }
}


PTR(Val) NumVal::add_to(PTR(Val) other_val){
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if(other_num == NULL) {
        throw std::runtime_error("cannot add numbers");

    }
    return NEW (NumVal)(val + other_num->val);
}

PTR(Val) NumVal::mult_to(PTR(Val) other_val){
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if(other_num == NULL){
        throw std::runtime_error("cannot mutiple numbers");
    }
    return NEW (NumVal)(this->val * other_num -> val);
}

std::string NumVal::to_string(){
    std::string s = std::to_string(val);
    return s;
}

PTR(Expr) NumVal::to_expr(){
    return NEW (NumExpr)(this->val);
    
}


PTR(Val) NumVal::call(PTR(Val) actual_arg){
    throw std::runtime_error("call number error");
}

PTR(Val) BoolVal::add_to(PTR(Val) other_val){
    throw std::runtime_error("cannot add numbers");
}

PTR(Val) BoolVal::mult_to(PTR(Val) other_val){
    throw std::runtime_error("cannot mutiple numbers");
}

bool BoolVal::equals(PTR(Val) o){
    PTR(BoolVal) c = CAST(BoolVal)(o);
    if(c == NULL){
        return false;
    }else{
        return (this->boolVal==c->boolVal);
    }
}

BoolVal::BoolVal(bool boolVal) {
    this->boolVal = boolVal;
}
    
PTR(Expr) BoolVal::to_expr(){
    return NEW (BoolExpr)(this->boolVal);
}

std::string BoolVal::to_string() {
    if (this->boolVal == true) {
        return "_true";
    }
    else {
        return "_false";
    }
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg){
    throw std::runtime_error("call number error");
}





FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env){
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}


bool FunVal::equals(PTR(Val) other){
    PTR(FunVal) c = CAST(FunVal)(other);
    if(c == NULL){
        return false;
    }else{
        return this->formal_arg==c->formal_arg && this->body->equals( c->body);
    }
}
PTR(Val) FunVal::add_to(PTR(Val) other_val){
    
    throw std::runtime_error("cannot add numbers");
    
}
PTR(Val) FunVal::mult_to(PTR(Val) other_val){
    
    throw std::runtime_error("cannot mutiple numbers");
    
}
std::string FunVal::to_string(){
    return "[function]";
}

PTR(Expr) FunVal::to_expr(){
    return NEW (FunExpr)(this->formal_arg, this->body);
}

PTR(Val) FunVal::call(PTR(Val) actual_arg){
    return this->body->interp(NEW(ExtendedEnv)(formal_arg,actual_arg,env));
}

//-------------------------------------TESTS-----------------------------------------

TEST_CASE("NumVal") {
  CHECK(!(NEW (NumVal)(10)) -> equals (NEW (NumVal)(1)));
  CHECK((NEW (NumVal)(10)) -> equals (NEW (NumVal)(10)));
    CHECK(!(NEW (NumVal)(10)) -> equals (NEW (BoolVal)(true)));
  CHECK(((NEW (NumVal)(10)) -> add_to(NEW (NumVal)(20))) -> equals(NEW (NumVal)(30)));
  CHECK(!((NEW (NumVal)(10)) -> add_to(NEW (NumVal)(20))) -> equals(NEW (NumVal)(1)));
  CHECK(((NEW (NumVal)(10)) -> mult_to(NEW (NumVal)(2))) -> equals(NEW (NumVal)(20)));
  CHECK(!((NEW (NumVal)(10)) -> mult_to(NEW (NumVal)(2))) -> equals(NEW (NumVal)(10)));
  CHECK(((NEW (NumVal)(10)) -> to_expr()) -> equals (NEW (NumExpr)(10)));
  CHECK(!((NEW (NumVal)(10)) -> to_expr()) -> equals (NEW (NumExpr)(200)));
  CHECK((NEW (NumVal)(10)) -> to_string() == "10");
  CHECK((NEW (NumVal)(10)) -> to_string() != "2");
  CHECK_THROWS_WITH(((NEW (NumVal)(1)) -> add_to(NEW (BoolVal)(true))),
                    "cannot add numbers");
    CHECK_THROWS_WITH(((NEW (NumVal)(1)) -> mult_to(NEW (BoolVal)(true))), "cannot mutiple numbers");
}

TEST_CASE("BoolVal"){
  CHECK((NEW (BoolVal)(true)) -> equals (NEW (BoolVal)(true)));
  CHECK(!(NEW (BoolVal)(true)) -> equals (NEW (BoolVal)(false)));
  CHECK(!(NEW (BoolVal)(true)) -> equals (NEW (NumVal)(777)));
  CHECK(((NEW (BoolVal)(true)) -> to_expr()) -> equals (NEW (BoolExpr)(true)));
  CHECK(!((NEW (BoolVal)(true)) -> to_expr()) -> equals (NEW (BoolExpr)(false)));
  CHECK_THROWS_WITH(((NEW (BoolVal)(true)) -> add_to(NEW (BoolVal)(true))), "cannot add numbers");
  CHECK_THROWS_WITH(((NEW (BoolVal)(true)) -> mult_to(NEW (BoolVal)(true))), "cannot mutiple numbers");
    CHECK((NEW (BoolVal)(true)) -> to_string() == "_true");
      CHECK((NEW (BoolVal)(false)) -> to_string() == "_false");
      CHECK((NEW (BoolVal)(true)) -> to_string() != "_false");
}

TEST_CASE("FunVal") {
      CHECK((NEW (FunVal)("a", NEW (NumExpr)(10), Env::empty))->equals(NEW (FunVal)("a", NEW (NumExpr)(10), Env::empty)));
      CHECK(!(NEW (FunVal)("a", NEW (NumExpr)(10), Env::empty))->equals(NEW (FunVal)("a", NEW (NumExpr)(20), Env::empty)));
      CHECK(!(NEW (FunVal)("a", NEW (NumExpr)(10), Env::empty))->equals(NEW (NumVal)(20)));
      CHECK_THROWS_WITH(((NEW (FunVal)("a", NEW (NumExpr)(10), Env::empty)) -> add_to(NEW (BoolVal)(true))), "cannot add numbers");
      CHECK_THROWS_WITH(((NEW (FunVal)("a", NEW (NumExpr)(10), Env::empty)) -> mult_to(NEW (BoolVal)(true))), "cannot mutiple numbers");
      CHECK((NEW (FunVal)("a", NEW (NumExpr)(10), Env::empty))->to_string() == "[function]");
      CHECK((NEW (FunVal)("a", NEW (NumExpr)(10), Env::empty))->to_expr()->equals(NEW (FunExpr)("a", NEW (NumExpr)(10))));
    }




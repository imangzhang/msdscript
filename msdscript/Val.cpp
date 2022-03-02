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
        throw std::runtime_error("cannot add numbers");

    }
    return new NumVal(val + other_num->val);
}

Val *NumVal::mult_to(Val *other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if(other_num == NULL){
        throw std::runtime_error("cannot mutiple numbers");
    }
    return new NumVal(this->val * other_num -> val);
}

std::string NumVal::to_string(){
    std::string s = std::to_string(val);
    return s;
}

Expr *NumVal::to_expr(){
    return new NumExpr(this->val);
    
}


Val *NumVal::call(Val *actual_arg){
    throw std::runtime_error("call number error");
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
    if (this->boolVal == true) {
        return "_true";
    }
    else {
        return "_false";
    }
}

Val *BoolVal::call(Val *actual_arg){
    throw std::runtime_error("call number error");
}





FunVal::FunVal(std::string formal_arg, Expr *body){
    this->formal_arg = formal_arg;
    this->body = body;
}


bool FunVal::equals(Val *other){
    FunVal *c = dynamic_cast<FunVal*>(other);
    if(c == NULL){
        return false;
    }else{
        return this->formal_arg==c->formal_arg && this->body->equals( c->body);
    }
}
Val *FunVal::add_to(Val *other_val){
    
    throw std::runtime_error("cannot add numbers");
    
}
Val *FunVal::mult_to(Val *other_val){
    
    throw std::runtime_error("cannot mutiple numbers");
    
}
std::string FunVal::to_string(){
    return "[function]";
}

Expr* FunVal::to_expr(){
    return new FunExpr(this->formal_arg, this->body);
}

Val* FunVal::call(Val *actual_arg){
    return (this->body->subst(this->formal_arg,actual_arg->to_expr()))->interp();
}

//-------------------------------------TESTS-----------------------------------------

TEST_CASE("NumVal") {
  CHECK(!(new NumVal(10)) -> equals (new NumVal(1)));
  CHECK((new NumVal(10)) -> equals (new NumVal(10)));
    CHECK(!(new NumVal(10)) -> equals (new BoolVal(true)));
  CHECK(((new NumVal(10)) -> add_to(new NumVal(20))) -> equals(new NumVal(30)));
  CHECK(!((new NumVal(10)) -> add_to(new NumVal(20))) -> equals(new NumVal(1)));
  CHECK(((new NumVal(10)) -> mult_to(new NumVal(2))) -> equals(new NumVal(20)));
  CHECK(!((new NumVal(10)) -> mult_to(new NumVal(2))) -> equals(new NumVal(10)));
  CHECK(((new NumVal(10)) -> to_expr()) -> equals (new NumExpr(10)));
  CHECK(!((new NumVal(10)) -> to_expr()) -> equals (new NumExpr(200)));
  CHECK((new NumVal(10)) -> to_string() == "10");
  CHECK((new NumVal(10)) -> to_string() != "2");
  CHECK_THROWS_WITH(((new NumVal(1)) -> add_to(new BoolVal(true))),
                    "cannot add numbers");
    CHECK_THROWS_WITH(((new NumVal(1)) -> mult_to(new BoolVal(true))), "cannot mutiple numbers");
}

TEST_CASE("BoolVal"){
  CHECK((new BoolVal(true)) -> equals (new BoolVal(true)));
  CHECK(!(new BoolVal(true)) -> equals (new BoolVal(false)));
  CHECK(!(new BoolVal(true)) -> equals (new NumVal(777)));
  CHECK(((new BoolVal(true)) -> to_expr()) -> equals (new BoolExpr(true)));
  CHECK(!((new BoolVal(true)) -> to_expr()) -> equals (new BoolExpr(false)));
  CHECK_THROWS_WITH(((new BoolVal(true)) -> add_to(new BoolVal(true))), "cannot add numbers");
  CHECK_THROWS_WITH(((new BoolVal(true)) -> mult_to(new BoolVal(true))), "cannot mutiple numbers");
    CHECK((new BoolVal(true)) -> to_string() == "_true");
      CHECK((new BoolVal(false)) -> to_string() == "_false");
      CHECK((new BoolVal(true)) -> to_string() != "_false");
}

TEST_CASE("FunVal") {
      CHECK((new FunVal("a", new NumExpr(10)))->equals(new FunVal("a", new NumExpr(10))));
      CHECK(!(new FunVal("a", new NumExpr(10)))->equals(new FunVal("a", new NumExpr(20))));
      CHECK(!(new FunVal("a", new NumExpr(10)))->equals(new NumVal(20)));
      CHECK_THROWS_WITH(((new FunVal("a", new NumExpr(10))) -> add_to(new BoolVal(true))), "cannot add numbers");
      CHECK_THROWS_WITH(((new FunVal("a", new NumExpr(10))) -> mult_to(new BoolVal(true))), "cannot mutiple numbers");
      CHECK((new FunVal("a", new NumExpr(10)))->to_string() == "[function]");
      CHECK((new FunVal("a", new NumExpr(10)))->to_expr()->equals(new FunExpr("a", new NumExpr(10))));
    }




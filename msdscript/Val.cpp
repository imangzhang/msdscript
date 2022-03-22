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





FunVal::FunVal(std::string formal_arg, PTR(Expr) body){
    this->formal_arg = formal_arg;
    this->body = body;
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
    //return (this->body->subst(this->formal_arg,actual_arg->to_expr()))->interp();
    return this->body->interp(NEW(ExtendedEnv)(formal_arg,actual_arg,env));
}

//-------------------------------------TESTS-----------------------------------------

//TEST_CASE("NumVal") {
//  CHECK(!(new NumVal(10)) -> equals (new NumVal(1)));
//  CHECK((new NumVal(10)) -> equals (new NumVal(10)));
//    CHECK(!(new NumVal(10)) -> equals (new BoolVal(true)));
//  CHECK(((new NumVal(10)) -> add_to(new NumVal(20))) -> equals(new NumVal(30)));
//  CHECK(!((new NumVal(10)) -> add_to(new NumVal(20))) -> equals(new NumVal(1)));
//  CHECK(((new NumVal(10)) -> mult_to(new NumVal(2))) -> equals(new NumVal(20)));
//  CHECK(!((new NumVal(10)) -> mult_to(new NumVal(2))) -> equals(new NumVal(10)));
//  CHECK(((new NumVal(10)) -> to_expr()) -> equals (new NumExpr(10)));
//  CHECK(!((new NumVal(10)) -> to_expr()) -> equals (new NumExpr(200)));
//  CHECK((new NumVal(10)) -> to_string() == "10");
//  CHECK((new NumVal(10)) -> to_string() != "2");
//  CHECK_THROWS_WITH(((new NumVal(1)) -> add_to(new BoolVal(true))),
//                    "cannot add numbers");
//    CHECK_THROWS_WITH(((new NumVal(1)) -> mult_to(new BoolVal(true))), "cannot mutiple numbers");
//}
//
//TEST_CASE("BoolVal"){
//  CHECK((new BoolVal(true)) -> equals (new BoolVal(true)));
//  CHECK(!(new BoolVal(true)) -> equals (new BoolVal(false)));
//  CHECK(!(new BoolVal(true)) -> equals (new NumVal(777)));
//  CHECK(((new BoolVal(true)) -> to_expr()) -> equals (new BoolExpr(true)));
//  CHECK(!((new BoolVal(true)) -> to_expr()) -> equals (new BoolExpr(false)));
//  CHECK_THROWS_WITH(((new BoolVal(true)) -> add_to(new BoolVal(true))), "cannot add numbers");
//  CHECK_THROWS_WITH(((new BoolVal(true)) -> mult_to(new BoolVal(true))), "cannot mutiple numbers");
//    CHECK((new BoolVal(true)) -> to_string() == "_true");
//      CHECK((new BoolVal(false)) -> to_string() == "_false");
//      CHECK((new BoolVal(true)) -> to_string() != "_false");
//}
//
//TEST_CASE("FunVal") {
//      CHECK((new FunVal("a", new NumExpr(10)))->equals(new FunVal("a", new NumExpr(10))));
//      CHECK(!(new FunVal("a", new NumExpr(10)))->equals(new FunVal("a", new NumExpr(20))));
//      CHECK(!(new FunVal("a", new NumExpr(10)))->equals(new NumVal(20)));
//      CHECK_THROWS_WITH(((new FunVal("a", new NumExpr(10))) -> add_to(new BoolVal(true))), "cannot add numbers");
//      CHECK_THROWS_WITH(((new FunVal("a", new NumExpr(10))) -> mult_to(new BoolVal(true))), "cannot mutiple numbers");
//      CHECK((new FunVal("a", new NumExpr(10)))->to_string() == "[function]");
//      CHECK((new FunVal("a", new NumExpr(10)))->to_expr()->equals(new FunExpr("a", new NumExpr(10))));
//    }




//
//  expr.cpp
//  msdscript
//
//  Created by Ang Zhang on 1/17/22.
//

#include "expr.hpp"
#include "catch.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include "Val.hpp"
#include "parse.hpp"

//methods of to-string
static std::string to_string(PTR(Expr) e){
    std::stringstream o;
    e->print(o);
    return o.str();
}

static std::string to_string_pretty(PTR(Expr) e){
    std::stringstream o;
    e->pretty_print(o);
    return o.str();
};

std::string Expr::to_string_cmd(){
    std::stringstream ss;
       this -> print(ss);
       return ss.str();
}
    
std::string Expr::to_pretty_string_cmd(){
    std::stringstream ss;
    this -> pretty_print(ss);
    return ss.str();
}


//--------------------------------***Num***--------------------------------
NumExpr::NumExpr(int val){
    this->val = val;
}


//check if the given value equals
bool NumExpr::equals(PTR(Expr) e){
    PTR(NumExpr) c = CAST(NumExpr)(e);
    if(c == NULL){
        return false;
    }else{
        return val == c->val;
    }
}

//return the value to the integer
PTR(Val) NumExpr::interp(PTR(Env) env){
    //return this->val;
    return NEW(NumVal)(this->val);
}

//return false as nums must be an integer
bool NumExpr::has_variable(){
    return false;
}

//return this as it is an integer
//PTR(Expr) NumExpr::subst(std::string original, PTR(Expr) replacement){
//    return THIS;
//}

void NumExpr::print(std::ostream &out){
    out << this ->val;
}

void NumExpr::pretty_print(std::ostream &out){
    pretty_print_at(out, prec_none, 0);
}

void NumExpr::pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed){
    out << this ->val;
}


//--------------------------------***Add***--------------------------------

AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

//check if the given value equals
bool AddExpr::equals(PTR(Expr) e){
    PTR(AddExpr) c = CAST(AddExpr)(e);
    if (c == NULL){
        return false;
    }else{
        return lhs->equals(c->lhs) && rhs->equals(c->rhs);
    }
}

//return the value of an addition expression is the sum of the subexpression values
PTR(Val) AddExpr::interp(PTR(Env) env){
    //return lhs->interp()+rhs->interp();
    return this->lhs->interp(env)->add_to(this->rhs->interp(env));
}

//The result should be true if the expression is a variable or contains a variable, false otherwise

bool AddExpr::has_variable(){
    return lhs->has_variable()||rhs->has_variable();
}

//subst for Add
//PTR(Expr) AddExpr::subst(std::string original, PTR(Expr) replacement){
//    return NEW (AddExpr)(lhs->subst(original, replacement),rhs->subst(original, replacement));
//}

//print for add
void AddExpr::print(std::ostream &out){
//    std::stringstream lhsOut(" ");
//    std::stringstream rhsOut(" ");
//    this->lhs->print(lhsOut);
//    this->rhs->print(rhsOut);
//    out <<"("<< lhsOut.str()<< "+" << rhsOut.str()<<")";
    
    out << "(";
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out <<")";
}

void AddExpr::pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed){
    if(prec == prec_add || prec == prec_mult || prec == prec_let){
        out << "(";
    }
    lhs->pretty_print_at(out, prec_add, spaceNeed);
    out << " + ";
    rhs->pretty_print_at(out, prec_none, spaceNeed);

    if(prec == prec_add || prec == prec_mult || prec == prec_let){
        out <<")";
    }
}

//pretty print for add
void AddExpr::pretty_print(std::ostream &out){
    lhs->pretty_print_at(out, prec_add, 0);
    out << " + ";
    rhs->pretty_print_at(out, prec_none, 0);
}

//--------------------------------***Mult***--------------------------------

MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

//check if the given value equals 
bool MultExpr::equals(PTR(Expr) e){
    PTR(MultExpr) c = CAST(MultExpr)(e);
    if(c == NULL){
        return false;
    }else{
        return lhs->equals(c->lhs) && rhs->equals(c->rhs);
    }
}

//return the value of a multiplication expression is the product of the subexpression values
PTR(Val) MultExpr::interp(PTR(Env) env){
    return lhs->interp(env)->mult_to(rhs->interp(env));
}

//The result should be true if the expression is a variable or contains a variable, false otherwise.

bool MultExpr::has_variable(){
    return lhs->has_variable()||rhs->has_variable();
}

//subst for Mult
//PTR(Expr) MultExpr::subst(std::string original, PTR(Expr) replacement){
//    return NEW (MultExpr)(lhs->subst(original, replacement), rhs->subst(original,replacement));
//}

//print for Mult
void MultExpr::print(std::ostream &out){
    std::stringstream lhsOut(" ");
    std::stringstream rhsOut(" ");
    lhs->print(lhsOut);
    rhs->print(rhsOut);
    out << "(" << lhsOut.str() << "*" << rhsOut.str() << ")";
}

void MultExpr::pretty_print_at(std::ostream &out,precedence_t prec, int spaceNeed){
    if(prec == prec_add){
        this->lhs->pretty_print_at(out, prec_mult, spaceNeed);
        out << " * ";
        this->rhs->pretty_print_at(out, prec_add, spaceNeed);
    }else{
        if(prec == prec_mult){
            out << "(";
        }
        this->lhs->pretty_print_at(out, prec_mult, spaceNeed);
        out << " * ";
        this->rhs->pretty_print_at(out, prec_let, spaceNeed);
        if(prec == prec_mult){
            out<< ")";
        }
    }
}

//pretty print for Mult
void MultExpr::pretty_print(std::ostream &out){
    this->lhs->pretty_print_at(out, prec_mult, 0);
    out << " * ";
    this->rhs->pretty_print_at(out, prec_let, 0);
        
}

//--------------------------------***Var***--------------------------------

VarExpr::VarExpr(std::string variable){
    this->variable = variable;
}

//check if the given value equals
bool VarExpr::equals(PTR(Expr) e){
    PTR(VarExpr) c = CAST(VarExpr)(e);
    if(c ==NULL){
        return false;
    }else{
        return variable == c->variable;
    }
}

//throw an exception as a variable is not an integer
PTR(Val) VarExpr::interp(PTR(Env) env){
    //throw std::runtime_error("This is not a integer!");
    return env->lookup(this->variable);
}

//return true as Var must be a variable
bool VarExpr::has_variable(){
    return true;
}


//return replaced value if variable equals original
//PTR(Expr) VarExpr::subst(std::string original, PTR(Expr)replacement){
//    if(this->variable==original){
//        return replacement;
//    }else{
//        return THIS;
//    }
//}

//print for var
void VarExpr::print(std::ostream &out){
    out << this ->variable;
}

//pretty print for var
void VarExpr::pretty_print(std::ostream &out){
    pretty_print_at(out, prec_none, 0);
}

void VarExpr::pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed){
    out << this ->variable;
}


//--------------------------------***Let***----------------------------------


_letExpr::_letExpr(std::string _letVariable, PTR(Expr) rhs, PTR(Expr) body){
    this->_letVariable = _letVariable;
    this->rhs = rhs;
    this->body = body;
};

bool _letExpr::equals(PTR(Expr) e){
    PTR(_letExpr) c = CAST(_letExpr)(e);
    if(c == NULL){
        return false;
    }else{
        return rhs->equals(c->rhs) && body->equals(c->body)&& _letVariable == (c->_letVariable);
    }
}

PTR(Val) _letExpr::interp(PTR(Env) env){
    PTR(Val) temp = rhs->interp(env);
    //return body->subst(_letVariable, temp->to_expr())->interp(env);
    PTR(Env) newEnv = NEW(ExtendedEnv)(_letVariable, temp, env);
    return body->interp(newEnv);
}


bool _letExpr::has_variable(){
    return rhs->has_variable() || body->has_variable();
}


//PTR(Expr) _letExpr::subst(std::string original, PTR(Expr) replacement){
//    if(_letVariable != original){
//        return NEW (_letExpr)(_letVariable, rhs->subst(original, replacement),body->subst(original,replacement));
//    }else{
//        return NEW (_letExpr)(_letVariable, rhs->subst(original, replacement), body);
//    }
//}


void _letExpr::print(std::ostream &out){
    out<<"(_let "<<_letVariable<<"=";
    rhs->print(out);
    out<<" _in ";
    body->print(out);
    out<<")";
    
}

void _letExpr::pretty_print(std::ostream &out){
//    int letPos = out.tellp();
//    out << "_let " << _letVariable << " = ";
//    rhs->pretty_print_at(out, prec_none
//                         , letPos);
//    out << "\n";
//    letPos = out.tellp();
//    out << "_in ";
//    body->pretty_print_at(out, prec_none, letPos);
    return;
};

void _letExpr::pretty_print_at(std::ostream &out,precedence_t prec, int spaceNeed){
//    int currentPos = out.tellp();
//    if(prec == prec_add || prec == prec_mult){
//        out << "(";
//        currentPos++;
//    }
//
//    out << "_let " << _letVariable << " = ";
//    rhs->pretty_print_at(out, prec_none, spaceNeed);
//    out <<"\n";
//    int newPos = out.tellp();
//    int space = currentPos - spaceNeed;
//    for( int i =0; i<space; i++){
//        out << " ";
//    }
//    out << "_in ";
//    this->body->pretty_print_at(out, prec_none, newPos);
//    if(prec == prec_add || prec == prec_mult){
//        out << ")";
//    }
    return;
}


//--------------------------------***BOOL***----------------------------------
BoolExpr::BoolExpr(bool val) {
  this->boolean = val;
}

bool BoolExpr::equals(PTR(Expr) e) {
    PTR(BoolExpr) otherBoolExpr = CAST(BoolExpr)(e);
  if (otherBoolExpr == NULL) {
    return false;
  }
  else {
    return (this->boolean == otherBoolExpr->boolean);
  }
}

PTR(Val) BoolExpr::interp(PTR(Env) env) {
  return NEW (BoolVal)(this->boolean);
}

bool BoolExpr::has_variable() {
  return false;
}

//PTR(Expr) BoolExpr::subst(std::string str, PTR(Expr) e) {
//  return THIS;
//}

void BoolExpr::print(std::ostream &out) {
  if (this->boolean) {
    out << "_true";
  }
  else {
    out << "_false";
  }
}

void BoolExpr::pretty_print(std::ostream &out) {
  return;
}

void BoolExpr::pretty_print_at(std::ostream &out, precedence_t prec, int space) {
//  if (this->boolean) {
//    out << "_true";
//  }
//  else {
//    out << "_false";
//  }
    return;
}

//--------------------------------***IF***----------------------------------

IfExpr::IfExpr(PTR(Expr) _if, PTR(Expr) _then, PTR(Expr) _else) {
  this->_if = _if;
  this->_then = _then;
  this->_else = _else;
}

bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr) otherIfExpr = CAST(IfExpr)(e);
  if (otherIfExpr == NULL) {
    return false;
  }
  else {
    return ((this->_if->equals(otherIfExpr->_if)) && (this->_then->equals(otherIfExpr->_then)) && (this->_else->equals(otherIfExpr->_else)));
  }
}

PTR(Val) IfExpr::interp(PTR(Env) env) {
  if (this->_if->interp(env)->equals(NEW (BoolVal)(true))) {
    return this->_then->interp(env);
  }
  else{
    return this->_else->interp(env);
  }
}

bool IfExpr::has_variable() {
  return (this->_if->has_variable() || this->_then->has_variable() || this->_else->has_variable());
}

//PTR(Expr) IfExpr::subst(std::string str, PTR(Expr) e) {
//    PTR(Expr) newIf = this->_if->subst(str, e);
//    PTR(Expr) newThen = this->_then->subst(str, e);
//    PTR(Expr) newElse = this->_else->subst(str, e);
//  return NEW(IfExpr)(newIf, newThen, newElse);
//}

void IfExpr::print(std::ostream &out) {
  out << "(" << "_if ";
  this->_if->print(out);
  out << " _then ";
  this->_then->print(out);
  out << " _else ";
  this->_else->print(out);
  out << ")";
}

void IfExpr::pretty_print(std::ostream &out) {
  return;
}

void IfExpr::pretty_print_at(std::ostream &out, precedence_t prec, int space) {

    return;
}

EqualExpr::EqualExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
  this -> lhs = lhs;
  this -> rhs = rhs;
}

bool EqualExpr::equals(PTR(Expr) e) {
    PTR(EqualExpr) otherEqualExpr = CAST(EqualExpr)(e);
  if (otherEqualExpr == NULL) {
    return false;
  }
  else {
    return (this->lhs ->equals(otherEqualExpr->lhs) && (this->rhs->equals(otherEqualExpr->rhs)));
  }
}

PTR(Val) EqualExpr::interp(PTR(Env) env) {
  return NEW (BoolVal)(this->lhs->interp(env)->equals(this->rhs->interp(env)));
}

bool EqualExpr::has_variable() {
  return (this->lhs->has_variable() || this->rhs->has_variable());
}

//PTR(Expr) EqualExpr::subst(std::string str, PTR(Expr) e) {
//    PTR(Expr) newlhs = this->lhs->subst(str, e);
//    PTR(Expr) newrhs = this->rhs->subst(str, e);
//  return NEW (EqualExpr)(newlhs, newrhs);
//}

void EqualExpr::print(std::ostream &out) {
  out << "(";
  this->lhs->print(out);
  out << "==";
  this->rhs->print(out);
  out << ")";
}

void EqualExpr::pretty_print(std::ostream &out) {
  return;
}

void EqualExpr::pretty_print_at(std::ostream &out, precedence_t prec, int space) {
    return;
}

//--------------------------------***FUN***----------------------------------

FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body){
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr) e){
    PTR(FunExpr) c =  CAST(FunExpr)(e);
    if(c ==NULL){
        return false;
    }else{
        return(this->formal_arg == c->formal_arg) && (this->body->equals(c->body));
    }
}

PTR(Val) FunExpr::interp(PTR(Env) env){
    return NEW (FunVal)(this->formal_arg,this->body, env);
}

void FunExpr::print(std::ostream &out){
    out <<"(_fun (";
    out << formal_arg;
    out <<") ";
    this->body->print(out);
    out << ")";
}

bool FunExpr::has_variable(){
    throw std::runtime_error("invalid call");
}

//PTR(Expr) FunExpr::subst(std::string str, PTR(Expr) e){
//    PTR(Expr) bodyExpr = this->body->subst(str, e);
//    if (str != this->formal_arg) {
//      return NEW (FunExpr)(this->formal_arg, bodyExpr);
//    }
//    else {
//      return NEW (FunExpr)(this->formal_arg, this->body);
//    }
//}

void FunExpr::pretty_print(std::ostream &out){
    return;
}

void FunExpr::pretty_print_at(std::ostream &out, precedence_t prec, int space){
    return;
}


//--------------------------------***CALL***----------------------------------
CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg){
    this->to_be_called = to_be_called;
    this->actual_arg =  actual_arg;
}

bool CallExpr::equals(PTR(Expr) e){
    PTR(CallExpr) c =  CAST(CallExpr)(e);
    if(c ==NULL){
        return false;
    }else{
        return(this->to_be_called ->equals( c->to_be_called)) && (this->actual_arg->equals(c->actual_arg));
    }
}

PTR(Val) CallExpr::interp(PTR(Env) env){
    return to_be_called->interp(env) ->call(actual_arg->interp(env));
    
}

void CallExpr::print(std::ostream &out){
    this->to_be_called->print(out);
    out<<"(";
    this->actual_arg->print(out);
    out<<")";
}

bool CallExpr::has_variable(){
    throw std::runtime_error("invalid call");
}

//PTR(Expr) CallExpr::subst(std::string str, PTR(Expr) e){
//    PTR(Expr) newTBC = this->to_be_called->subst(str, e);
//    PTR(Expr) newAA= this->actual_arg->subst(str, e);
//    return NEW(CallExpr)(newTBC, newAA);
//}

void CallExpr::pretty_print(std::ostream &out){
    return;
}

void CallExpr::pretty_print_at(std::ostream &out, precedence_t prec, int space){
    return;
}

//--------------------------------***Tests***--------------------------------

TEST_CASE("print") {
    CHECK(to_string(NEW (NumExpr)(100)) == "100");
    
    CHECK(to_string(NEW (VarExpr)("xyz")) == "xyz");
    CHECK(to_string(NEW (VarExpr)("abc")) != "xyz");
    
    CHECK(to_string((NEW (AddExpr)((NEW (NumExpr)(10)), (NEW (NumExpr)(20))))) == "(10+20)");
    CHECK(to_string((NEW (MultExpr)((NEW (NumExpr)(10)), (NEW (NumExpr)(20))))) == "(10*20)");
    CHECK(to_string((NEW (AddExpr)(NEW (NumExpr)(1), NEW (AddExpr)(NEW (NumExpr)(2), NEW (NumExpr)(3))))) == "(1+(2+3))");
    CHECK(to_string((NEW (AddExpr)(NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2)), NEW (NumExpr)(3)))) == "((1+2)+3)");
}

TEST_CASE("pretty_print") {
    CHECK(to_string_pretty(NEW (NumExpr)(10)) == "10");
    CHECK(to_string_pretty(NEW (VarExpr)("xyz")) == "xyz");
    CHECK(to_string_pretty((NEW (AddExpr)((NEW (NumExpr)(10)), (NEW (NumExpr)(20))))) == "10 + 20");
    CHECK(to_string_pretty((NEW (MultExpr)((NEW (NumExpr)(10)), (NEW (NumExpr)(20))))) == "10 * 20");
    CHECK(to_string_pretty(NEW (AddExpr)(NEW (NumExpr)(1), NEW (MultExpr)(NEW (NumExpr)(2), NEW (NumExpr)(3)))) == "1 + 2 * 3");
    CHECK(to_string_pretty(NEW (MultExpr)(NEW (NumExpr)(1), NEW (AddExpr)(NEW (NumExpr)(2), NEW (NumExpr)(3)))) == "1 * (2 + 3)");
    CHECK(to_string_pretty(NEW (AddExpr)(NEW (MultExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2)), NEW (NumExpr)(3))) == "1 * 2 + 3");
    CHECK(to_string_pretty(NEW (MultExpr)(NEW (MultExpr)(NEW (NumExpr)(2), NEW (NumExpr)(3)), NEW (NumExpr)(4))) == "(2 * 3) * 4");
    CHECK(to_string_pretty(NEW (MultExpr)(NEW (NumExpr)(2), NEW (MultExpr)(NEW (NumExpr)(3), NEW (NumExpr)(4)))) == "2 * 3 * 4");
}


TEST_CASE("equals"){
    SECTION("Num"){
        CHECK( (NEW (NumExpr)(1))->equals(NEW (NumExpr)(1)));
        CHECK( (NEW (NumExpr)(1010))->equals(NEW (NumExpr)(1010)));
        CHECK( ! (NEW (NumExpr)(1))->equals(NEW (NumExpr)(2)));
        CHECK( ! (NEW (NumExpr)(1))->equals(NEW (VarExpr)("b")));
        CHECK( ! (NEW (NumExpr)(1))->equals((NEW (AddExpr)(NEW (NumExpr)(2), NEW (NumExpr)(1)))) );
        CHECK( ! (NEW (NumExpr)(1))->equals((NEW (MultExpr)(NEW (NumExpr)(2), NEW (NumExpr)(3)))) );


    }

    SECTION("Add"){
        CHECK((NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2)))
              ->equals(NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2))));
        CHECK(! (NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2)))
              ->equals(NEW (AddExpr)(NEW (NumExpr)(2),NEW (NumExpr)(1))));
        CHECK(! (NEW (AddExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20)))
              ->equals(NEW (AddExpr)(NEW (NumExpr)(2),NEW (NumExpr)(2))));
        CHECK(! (NEW (AddExpr)(NEW (NumExpr)(100), NEW (NumExpr)(1)))
              ->equals(NEW (NumExpr)(33)));
    }

    SECTION("Mult"){
        CHECK( (NEW (MultExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2)))->equals(NEW (MultExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2))));
        CHECK(!(NEW (MultExpr)(NEW (NumExpr)(10), NEW (NumExpr)(10)))->equals(NEW (AddExpr)(NEW (NumExpr)(10),NEW (NumExpr)(10))));
        CHECK(! (NEW (MultExpr)(NEW (NumExpr)(2), NEW (NumExpr)(4)))
              ->equals(NEW (MultExpr)(NEW (NumExpr)(4),NEW (NumExpr)(4))));
        CHECK(! (NEW (MultExpr)(NEW (NumExpr)(10), NEW (NumExpr)(1)))
              ->equals(NEW (NumExpr)(99)));
    }

    SECTION("Var"){
        CHECK( (NEW (VarExpr)("a"))->equals(NEW (VarExpr)("a")));
        CHECK(! (NEW (VarExpr)("a"))->equals(NEW (VarExpr)("b")));
        CHECK(! (NEW (VarExpr)("a"))->equals(NEW (VarExpr)("abd")));
        CHECK(! (NEW (VarExpr)("a"))->equals(NEW (NumExpr)(1)));
    }


    SECTION("interp") {
        CHECK((NEW (NumExpr)(9))->interp(Env::empty) ->equals(NEW (NumVal)(9)));
        CHECK(!(NEW (NumExpr)(19))->interp(Env::empty) ->equals( NEW (NumVal)(1)));
        CHECK((NEW (AddExpr)((NEW (NumExpr)(100)), (NEW (NumExpr)(100))))->interp(Env::empty) ->equals( NEW (NumVal)(200)));
        CHECK((NEW (MultExpr)((NEW (NumExpr)(12)), (NEW (NumExpr)(2))))->interp(Env::empty) ->equals( NEW (NumVal)(24)));
        CHECK(!(NEW (MultExpr)((NEW (NumExpr)(1)), (NEW (NumExpr)(1))))->interp(Env::empty) ->equals( NEW (NumVal)(3)));
        CHECK_THROWS_WITH((NEW (VarExpr)("x"))
                          ->interp(Env::empty), "free variable: x");
        CHECK_THROWS_WITH((NEW (AddExpr)((NEW (VarExpr)("xyz")), (NEW (VarExpr)("abc"))))
                          ->interp(Env::empty), "free variable: xyz");
        CHECK_THROWS_WITH((NEW (MultExpr)((NEW (VarExpr)("hello")), (NEW (NumExpr)(1000))))
                          ->interp(Env::empty), "free variable: hello");
    }


//    SECTION("subst") {
//        CHECK((new NumExpr(10))->subst("xyz", new VarExpr("abc"))
//              ->equals(new NumExpr(10)));
//        CHECK((new NumExpr(10))->subst("xyz", new NumExpr(10))
//              ->equals(new NumExpr(10)));
//        CHECK((new VarExpr("xyz"))->subst("xyz", new VarExpr("abc"))
//              ->equals(new VarExpr("abc")));
//        CHECK((new VarExpr("xyz"))->subst("xyz", new NumExpr(10))->equals(new NumExpr(10)));
//        CHECK((new VarExpr("xyz"))->subst("xyz", (new AddExpr((new NumExpr(10)), (new NumExpr(20)))))
//              ->equals(new AddExpr((new NumExpr(10)), (new NumExpr(20)))));
//        CHECK((new VarExpr("xyz"))->subst("abc", (new AddExpr((new NumExpr(10)), (new NumExpr(20)))))
//              ->equals(new VarExpr("xyz")));
//        CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
//               ->subst("x", new VarExpr("y"))
//               ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
//        CHECK((new AddExpr((new NumExpr(10)), (new NumExpr(20))))->subst("xyz", new VarExpr("abc"))
//              ->equals(new AddExpr((new NumExpr(10)), (new NumExpr(20)))));
//        CHECK(!(new AddExpr((new NumExpr(10)), (new VarExpr("xyz"))))
//              ->subst("xyz", new VarExpr("abc"))->equals(new AddExpr((new NumExpr(10)), (new VarExpr("xyz")))));
//        CHECK((new AddExpr((new VarExpr("xyz")), (new VarExpr("abc"))))->subst("xyz", new VarExpr("a"))
//              ->equals(new AddExpr((new VarExpr("a")), (new VarExpr("abc")))));
//        CHECK((new AddExpr((new VarExpr("abc")), (new NumExpr(10))))->subst("abc", new VarExpr("xyz"))
//              ->equals(new AddExpr((new VarExpr("xyz")), (new NumExpr(10)))));
//        CHECK((new MultExpr((new NumExpr(10)), (new NumExpr(5))))
//              ->subst("abc", new NumExpr(29))->equals(new MultExpr((new NumExpr(10)), (new NumExpr(5)))));
//        CHECK((new MultExpr((new NumExpr(10)), (new VarExpr("abc"))))
//              ->subst("abc", new NumExpr(20))->equals(new MultExpr((new NumExpr(10)), (new NumExpr(20)))));
//        CHECK((new MultExpr((new VarExpr("abc")), (new VarExpr("abc"))))->subst("abc", new NumExpr(10))
//              ->equals(new MultExpr((new NumExpr(10)), (new NumExpr(10)))));
//        CHECK((new MultExpr((new VarExpr("abc")), (new NumExpr(10))))->subst("abc", new NumExpr(20))->equals(new MultExpr((new NumExpr(20)), (new NumExpr(10)))));
//    }

}

TEST_CASE("_let"){
    SECTION("equal"){
        CHECK((NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (NumExpr)(20)))->equals((NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (NumExpr)(20)))));
        CHECK(!(NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (NumExpr)(20)))->equals((NEW (_letExpr)("a", NEW (NumExpr)(20), NEW (NumExpr)(10)))));
        CHECK(!(NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (NumExpr)(20)))->equals((NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(10))))));
        CHECK((NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(20))))
              ->equals(NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(20)))));
        CHECK(!(NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (NumExpr)(20)))->equals(NEW (NumExpr)(1)));
    }

    SECTION("interp"){
        CHECK((NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (NumExpr)(20)))->interp(Env::empty) ->equals( NEW (NumVal)(20)));
        CHECK((NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(20))))->interp(Env::empty) ->equals( NEW (NumVal)(30)));
        CHECK_THROWS_WITH((NEW (_letExpr)("a", NEW (VarExpr)("a"), NEW (NumExpr)(10)))->interp(Env::empty),"free variable: a");
        CHECK_THROWS_WITH((NEW (_letExpr)("a", NEW (VarExpr)("b"), NEW (AddExpr)(NEW (VarExpr)("b"), NEW (NumExpr)(10))))->interp(Env::empty), "free variable: b");
        CHECK((NEW (BoolExpr)(true))->interp(Env::empty)->equals(NEW (BoolVal)(true)));
          CHECK(!(NEW (BoolExpr)(true))->interp(Env::empty)->equals(NEW (BoolVal)(false)));
          CHECK((NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumExpr)(10), NEW (NumExpr)(20)))
                ->interp(Env::empty)->equals(NEW (NumVal)(10)));
          CHECK((NEW (IfExpr)(NEW (BoolExpr)(false), NEW (NumExpr)(10), NEW (NumExpr)(20)))
                ->interp(Env::empty)->equals(NEW (NumVal)(20)));
          CHECK((NEW (EqualExpr)(NEW (NumExpr)(10), NEW (NumExpr)(10)))
                ->interp(Env::empty)->equals(NEW (BoolVal)(true)));
          CHECK((NEW (EqualExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20)))
                ->interp(Env::empty)->equals(NEW (BoolVal)(false)));

          CHECK((NEW (FunExpr)("a", NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(10))))->interp(Env::empty)->equals(NEW (FunVal)("a", NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(10)), Env::empty)));

          CHECK(!(NEW (CallExpr)((NEW (FunExpr)("a", NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(10)))), NEW (NumExpr)(15)))->interp(Env::empty)->equals(NEW (NumVal)(30)));
    }

    SECTION("has_variable"){
        CHECK(!(NEW (NumExpr)(100))->has_variable());
        CHECK((NEW (VarExpr)("xyz"))->has_variable());
        CHECK(!(NEW (AddExpr)((NEW (NumExpr)(10)), (NEW (NumExpr)(10))))
              ->has_variable());
        CHECK((NEW (AddExpr)((NEW (VarExpr)("xyz")), (NEW (NumExpr)(10))))
              ->has_variable());
        CHECK(!(NEW (MultExpr)((NEW (NumExpr)(10)), (NEW (NumExpr)(10))))
              ->has_variable());
        CHECK((NEW (MultExpr)((NEW (VarExpr)("xyz")), (NEW (NumExpr)(10))))
              ->has_variable());
        CHECK(!(NEW (_letExpr)("x", NEW (NumExpr)(10), NEW (NumExpr)(20)))->has_variable());
        CHECK((NEW (_letExpr)("a", NEW (VarExpr)("b"), NEW (VarExpr)("c")))->has_variable());
        CHECK((NEW (_letExpr)("a", NEW (AddExpr)(NEW (NumExpr)(10), NEW (VarExpr)("b")), NEW (AddExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20))))
              ->has_variable());
        CHECK(!(NEW (_letExpr)("a", NEW (AddExpr)(NEW (NumExpr)(3), NEW (NumExpr)(4)), NEW (AddExpr)(NEW (NumExpr)(100), NEW (NumExpr)(10))))
              ->has_variable());
        CHECK(!(NEW (BoolExpr)(true))->has_variable());
          CHECK(!(NEW (BoolExpr)(false))->has_variable());
          CHECK(!(NEW (IfExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20), NEW (NumExpr)(30)))
                ->has_variable());
          CHECK((NEW (IfExpr)(NEW (VarExpr)("10"), NEW (NumExpr)(20), NEW (NumExpr)(30)))
                ->has_variable());
          CHECK(!(NEW (EqualExpr)(NEW (NumExpr)(10),NEW (NumExpr)(20)))->has_variable());
          CHECK((NEW (EqualExpr)(NEW (VarExpr)("10"),NEW (NumExpr)(20)))->has_variable());

        CHECK_THROWS_WITH((NEW (FunExpr)("a", NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(10))))->has_variable(), "invalid call");
          CHECK_THROWS_WITH((NEW (CallExpr)((NEW (FunExpr)("a", NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(10)))), NEW (NumExpr)(20)))->has_variable(), "invalid call");

    }

//    SECTION("subst"){
//        CHECK((new _letExpr("a", new NumExpr(3), new NumExpr(5)))
//              ->subst("a", new VarExpr("z"))->equals((new _letExpr("a", new NumExpr(3), new NumExpr(5)))));
//        CHECK((new _letExpr("a", new VarExpr("y"), new VarExpr("z")))
//              ->subst("a", new VarExpr("xyz"))->equals((new _letExpr("a", new VarExpr("y"), new VarExpr("z")))));
//        CHECK((new _letExpr("a", new VarExpr("b"), new NumExpr(10)))
//              ->subst("b", new NumExpr(5))->equals((new _letExpr("a", new NumExpr(5), new NumExpr(10)))));
//        CHECK((new _letExpr("a", new AddExpr(new NumExpr(10), new VarExpr("b")), new AddExpr(new VarExpr("b"), new NumExpr(20))))
//              ->subst("b", new NumExpr(5))->equals((new _letExpr("a", new AddExpr(new NumExpr(10), new NumExpr(5)), new AddExpr(new NumExpr(5), new NumExpr(20))))));
//        CHECK((new BoolExpr(true))->subst("MSD", new NumExpr(10))
//              ->equals(new BoolExpr(true)));
//          CHECK(!(new BoolExpr(true))->subst("MSD", new NumExpr(10))->equals(new BoolExpr(false)));
//          CHECK((new IfExpr(new VarExpr("a"), new NumExpr(10), new NumExpr(20)))->subst("a", new VarExpr("b"))->equals(new IfExpr(new VarExpr("b"), new NumExpr(10), new NumExpr(20))));
//          CHECK((new IfExpr(new NumExpr(10), new VarExpr("a"), new NumExpr(7)))->subst("a", new VarExpr("b"))->equals(new IfExpr(new NumExpr(10), new VarExpr("b"), new NumExpr(7))));
//          CHECK((new IfExpr(new NumExpr(10), new NumExpr(20), new VarExpr("a")))->subst("a", new VarExpr("b"))->equals(new IfExpr(new NumExpr(10), new NumExpr(20), new VarExpr("b"))));
//          CHECK((new EqualExpr(new VarExpr("a"), new NumExpr(10)))->subst("a", new VarExpr("b"))->equals(new EqualExpr(new VarExpr("b"), new NumExpr(10))));
//          CHECK((new EqualExpr(new NumExpr(10), new VarExpr("b")))->subst("a", new VarExpr("b"))->equals(new EqualExpr(new NumExpr(10), new VarExpr("b"))));
//
//        CHECK((new FunExpr("a", new VarExpr("b")))->subst("b", new NumExpr(10))->equals(new FunExpr("a", new NumExpr(10))));
//          CHECK((new FunExpr("a", new NumExpr(10)))->subst("a", new NumExpr(54))->equals(new FunExpr("a", new NumExpr(10))));
//
//          CHECK((new CallExpr(new VarExpr("a"), new VarExpr("a")))->subst("a", new VarExpr("b"))->equals(new CallExpr(new VarExpr("b"), new VarExpr("b"))));
//
//    }

    SECTION("print"){
        CHECK(to_string(NEW (_letExpr)("a", NEW (NumExpr)(10), NEW (NumExpr)(20))) == "(_let a=10 _in 20)");
        CHECK(to_string(NEW (_letExpr)("a", NEW (VarExpr)("b"), NEW (VarExpr)("c"))) == "(_let a=b _in c)");
        CHECK(to_string(NEW (_letExpr)("a", NEW (VarExpr)("b"), NEW (AddExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20)))) == "(_let a=b _in (10+20))");
        CHECK(to_string(NEW (_letExpr)("x", NEW (AddExpr)(NEW (NumExpr)(10), NEW (VarExpr)("xyz")), NEW (VarExpr)("a"))) == "(_let x=(10+xyz) _in a)");

        CHECK(to_string( NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10)))) == "(_fun (x) (x+10))");

          CHECK(to_string(NEW (CallExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10))) == "x(10)");

    }

//    SECTION("pretty_print"){
//        CHECK(to_string_pretty(new _letExpr("a", new NumExpr(10), new AddExpr(new VarExpr("a"), new NumExpr(20)))) ==
//             "_let a = 10\n"
//             "_in a + 20");
//        CHECK(to_string_pretty(new _letExpr("a", new NumExpr(10), new AddExpr(new _letExpr("b", new NumExpr(20), new AddExpr(new VarExpr("b"), new NumExpr(6))), new VarExpr("a")))) ==
//              "_let a = 10\n"
//              "_in (_let b = 20\n"
//              "     _in b + 6) + a");
//        CHECK(to_string_pretty(new AddExpr(new MultExpr(new _letExpr("x", new NumExpr(10), new VarExpr("x")), new NumExpr(10)), new NumExpr(20))) ==
//               "(_let x = 10\n"
//               " _in x) * 10 + 20");

//    }
}

TEST_CASE("EQUAL"){
    CHECK((NEW (BoolExpr)(true))->equals(NEW (BoolExpr)(true)));
      CHECK(!(NEW (BoolExpr)(true))->equals(NEW (BoolExpr)(false)));
      CHECK(!(NEW (BoolExpr)(true))->equals(NEW (NumExpr)(0)));
      CHECK((NEW (IfExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20), NEW (NumExpr)(30)))
            ->equals(NEW (IfExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20), NEW (NumExpr)(30))));
      CHECK(!(NEW (IfExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20), NEW (NumExpr)(30)))
            ->equals(NEW (IfExpr)(NEW (NumExpr)(10), NEW (NumExpr)(30), NEW (NumExpr)(20))));
      CHECK(!(NEW (IfExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20), NEW (NumExpr)(30)))
            ->equals(NEW (NumExpr)(20)));
      CHECK((NEW (EqualExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20)))
            ->equals(NEW (EqualExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20))));
      CHECK(!(NEW (EqualExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20)))
            ->equals(NEW (EqualExpr)(NEW (NumExpr)(10), NEW (NumExpr)(30))));
      CHECK(!(NEW (EqualExpr)(NEW (NumExpr)(10), NEW (NumExpr)(20)))
            ->equals(NEW (NumExpr)(20)));



    CHECK((NEW (FunExpr)("a", NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(10))))->equals(NEW (FunExpr)("a", NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(10)))));
      CHECK(!(NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10))))->equals(NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(20)))));
      CHECK(!(NEW (FunExpr)("a", NEW (AddExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(10))))->equals(NEW (NumExpr)(20)));

      CHECK((NEW (CallExpr)((NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10)))), NEW (NumExpr)(20)))->equals(NEW (CallExpr)((NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10)))), NEW (NumExpr)(20))));
      CHECK(!(NEW (CallExpr)((NEW (FunExpr)("a", NEW (AddExpr)(NEW (VarExpr)("b"), NEW (NumExpr)(10)))), NEW (NumExpr)(20)))->equals(NEW (CallExpr)((NEW (FunExpr)("b", NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(20)))), NEW (NumExpr)(10))));
      CHECK(!(NEW (CallExpr)((NEW (FunExpr)("a", NEW (AddExpr)(NEW (VarExpr)("b"), NEW (NumExpr)(20)))), NEW (NumExpr)(10)))->equals(NEW (VarExpr)("b")));

}

TEST_CASE("toString"){

    CHECK(to_string(NEW (BoolExpr)(true)) == "_true");
    CHECK(to_string(NEW (BoolExpr)(false)) == "_false");
    CHECK(to_string(NEW (IfExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(10), NEW (NumExpr)(20))) == "(_if x _then 10 _else 20)");
    CHECK((to_string(NEW (EqualExpr)(NEW (NumExpr)(100), NEW (NumExpr)(10))) == "(100==10)"));
}


//
TEST_CASE("NEW INTERP"){
    CHECK((NEW(NumExpr)(5))->interp(Env::empty)
          ->equals(NEW(NumVal)(5)));
    CHECK((NEW(NumExpr)(-5))->interp(Env::empty)
          ->equals(NEW(NumVal)(-5)));
    CHECK((NEW(NumExpr)(0))->interp(Env::empty)
          ->equals(NEW(NumVal)(0)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(10),NEW(NumExpr)(5)))
          ->interp(Env::empty)->equals(NEW(NumVal)(15)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(-10),NEW(NumExpr)(5)))
           ->interp(Env::empty)->equals(NEW(NumVal)(-5)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(-10),NEW(NumExpr)(10)))
           ->interp(Env::empty)->equals(NEW(NumVal)(0)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(1),NEW(NumExpr)(5)))
           ->interp(Env::empty)->equals(NEW(NumVal)(5)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-1),NEW(NumExpr)(5)))
          ->interp(Env::empty)->equals(NEW(NumVal)(-5)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-1),NEW(NumExpr)(-5)))
          ->interp(Env::empty)->equals(NEW(NumVal)(5)));
    CHECK((NEW(_letExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(10))))->interp(Env::empty)
          ->equals(NEW(NumVal)(15)));
    CHECK(!(NEW(_letExpr)("x", NEW(AddExpr)( NEW(NumExpr)(5), NEW(NumExpr) (10)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->interp(Env::empty)
          ->equals(NEW(NumVal)(8)));
    CHECK((NEW(_letExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(_letExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))->interp(Env::empty)
          ->equals(NEW(NumVal)(10)));
    CHECK((NEW(_letExpr)("x", NEW(NumExpr)(5), NEW(_letExpr)("x", NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->interp(Env::empty)
          ->equals(NEW(NumVal)(8)));
    CHECK(((NEW(BoolExpr)(true))->interp(Env::empty)
           ->equals(NEW(BoolVal)(true))));
    CHECK(((NEW(BoolExpr)(false))->interp(Env::empty)
           ->equals(NEW(BoolVal)(false))));
    CHECK(((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(10), NEW(VarExpr)("x")))->interp(Env::empty)
           ->equals(NEW(NumVal)(10))));
    CHECK(((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5), NEW(NumExpr)(10)))->interp(Env::empty)
           ->equals(NEW(NumVal)(10))));
    CHECK((NEW(FunExpr)("a", NEW(AddExpr)(NEW(VarExpr)("a"), NEW(NumExpr)(10))))->interp(Env::empty)
          ->equals(NEW(FunVal)("a", NEW(AddExpr)(NEW( VarExpr)("a"), NEW(NumExpr)(10)), Env::empty)));
    CHECK(!(NEW(CallExpr)((NEW(FunExpr)("a", NEW(AddExpr)(NEW(VarExpr)("a"), NEW( NumExpr)(10)))), NEW(NumExpr)(15)))->interp(Env::empty)
           ->equals(NEW(NumVal)(30)));
    CHECK(!(NEW(EqualExpr)(NEW(NumExpr)(10),NEW(NumExpr)(20)))->has_variable());
    CHECK((NEW(EqualExpr)(NEW(VarExpr)("10"),NEW(NumExpr)(20)))->has_variable());
    CHECK_THROWS_WITH((NEW(VarExpr)("x"))->interp(Env::empty), "free variable: x" );
    CHECK_THROWS_WITH((NEW(VarExpr)("!!"))->interp(Env::empty), "free variable: !!" );
    CHECK_THROWS_WITH((NEW(VarExpr)(""))->interp(Env::empty), "free variable: " );
    CHECK_THROWS_WITH((NEW(AddExpr)(NEW(VarExpr)("x"),NEW(NumExpr)(10)))->interp(Env::empty), "free variable: x" );
    CHECK_THROWS_WITH((NEW(MultExpr)(NEW(NumExpr)(10),NEW(VarExpr)("x")))->interp(Env::empty), "free variable: x" );
    CHECK_THROWS_WITH((NEW(_letExpr)("x", NEW(VarExpr)("z"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(10))))->interp(Env::empty), "free variable: z");
    CHECK_THROWS_WITH((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(10), NEW(VarExpr)("z")))->interp(Env::empty), "free variable: z");









//    CHECK(parse_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(5)")->interp(Env::empty)->equals(NEW(NumExpr)(120)) );
//    CHECK( parse_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(10)")->interp(Env::empty)->equals(NEW(NumExpr)(3628800)) );
//    CHECK( parse_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in _let factorial = factrl(factrl) _in factorial(5)")->interp(Env::empty)->equals(NEW(NumVal)(120)) );
}


    

        



                                                             


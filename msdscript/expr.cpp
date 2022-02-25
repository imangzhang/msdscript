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

//methods of to-string
static std::string to_string(Expr* e){
    std::stringstream o;
    e->print(o);
    return o.str();
}

static std::string to_string_pretty(Expr* e){
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
bool NumExpr::equals(Expr *e){
    NumExpr *c = dynamic_cast<NumExpr*>(e);
    if(c == NULL){
        return false;
    }else{
        return val == c->val;
    }
}

//return the value to the integer
Val* NumExpr::interp(){
    //return this->val;
    return new NumVal(this->val);
}

//return false as nums must be an integer
bool NumExpr::has_variable(){
    return false;
}

//return this as it is an integer
Expr* NumExpr::subst(std::string original, Expr* replacement){
    return this;
}

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

AddExpr::AddExpr(Expr *lhs, Expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

//check if the given value equals
bool AddExpr::equals(Expr *e){
    AddExpr *c = dynamic_cast<AddExpr*>(e);
    if (c == NULL){
        return false;
    }else{
        return lhs->equals(c->lhs) && rhs->equals(c->rhs);
    }
}

//return the value of an addition expression is the sum of the subexpression values
Val* AddExpr::interp(){
    //return lhs->interp()+rhs->interp();
    return lhs->interp()->add_to(rhs->interp());
}

//The result should be true if the expression is a variable or contains a variable, false otherwise

bool AddExpr::has_variable(){
    return lhs->has_variable()||rhs->has_variable();
}

//subst for Add
Expr* AddExpr::subst(std::string original, Expr* replacement){
    return new AddExpr(lhs->subst(original,replacement),rhs->subst(original, replacement));
}

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

MultExpr::MultExpr(Expr *lhs, Expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

//check if the given value equals 
bool MultExpr::equals(Expr *e){
    MultExpr *c = dynamic_cast<MultExpr*>(e);
    if(c == NULL){
        return false;
    }else{
        return lhs->equals(c->lhs) && rhs->equals(c->rhs);
    }
}

//return the value of a multiplication expression is the product of the subexpression values
Val* MultExpr::interp(){
    return lhs->interp()->mult_to(rhs->interp());
}

//The result should be true if the expression is a variable or contains a variable, false otherwise.

bool MultExpr::has_variable(){
    return lhs->has_variable()||rhs->has_variable();
}

//subst for Mult
Expr* MultExpr::subst(std::string original, Expr *replacement){
    return new MultExpr(lhs->subst(original, replacement), rhs->subst(original,replacement));
}

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
bool VarExpr::equals(Expr *e){
    VarExpr *c = dynamic_cast<VarExpr*>(e);
    if(c ==NULL){
        return false;
    }else{
        return variable == c->variable;
    }
}

//throw an exception as a variable is not an integer
Val* VarExpr::interp(){
    throw std::runtime_error("This is not a integer!");
}

//return true as Var must be a variable
bool VarExpr::has_variable(){
    return true;
}


//return replaced value if variable equals original
Expr* VarExpr::subst(std::string original, Expr *replacement){
    if(this->variable==original){
        return replacement;
    }else{
        return this;
    }
}

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


_letExpr::_letExpr(std::string _letVariable, Expr *rhs, Expr *body){
    this->_letVariable = _letVariable;
    this->rhs = rhs;
    this->body = body;
};

bool _letExpr::equals(Expr *e){
    _letExpr *c = dynamic_cast<_letExpr*>(e);
    if(c == NULL){
        return false;
    }else{
        return rhs->equals(c->rhs) && body->equals(c->body)&& _letVariable == (c->_letVariable);
    }
}

Val* _letExpr::interp(){
    Val* temp = rhs->interp();
    return body->subst(_letVariable, temp->to_expr())->interp();
}


bool _letExpr::has_variable(){
    return rhs->has_variable() || body->has_variable();
}


Expr* _letExpr::subst(std::string original, Expr* replacement){
    if(_letVariable != original){
        return new _letExpr(_letVariable, rhs->subst(original, replacement),body->subst(original,replacement));
    }else{
        return new _letExpr(_letVariable, rhs->subst(original, replacement), body);
    }
}


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



BoolExpr::BoolExpr(bool val) {
  this->boolean = val;
}

bool BoolExpr::equals(Expr *e) {
  BoolExpr *otherBoolExpr = dynamic_cast<BoolExpr*>(e);
  if (otherBoolExpr == NULL) {
    return false;
  }
  else {
    return (this->boolean == otherBoolExpr->boolean);
  }
}

Val* BoolExpr::interp() {
  return new BoolVal(this->boolean);
}

bool BoolExpr::has_variable() {
  return false;
}

Expr* BoolExpr::subst(std::string str, Expr *e) {
  return this;
}

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

IfExpr::IfExpr(Expr *_if, Expr *_then, Expr *_else) {
  this->_if = _if;
  this->_then = _then;
  this->_else = _else;
}

bool IfExpr::equals(Expr *e) {
  IfExpr *otherIfExpr = dynamic_cast<IfExpr*>(e);
  if (otherIfExpr == NULL) {
    return false;
  }
  else {
    return ((this->_if->equals(otherIfExpr->_if)) && (this->_then->equals(otherIfExpr->_then)) && (this->_else->equals(otherIfExpr->_else)));
  }
}

Val* IfExpr::interp() {
  if (this->_if->interp()->equals(new BoolVal(true))) {
    return this->_then->interp();
  }
  else{
    return this->_else->interp();
  }
}

bool IfExpr::has_variable() {
  return (this->_if->has_variable() || this->_then->has_variable() || this->_else->has_variable());
}

Expr* IfExpr::subst(std::string str, Expr *e) {
  Expr* newIf = this->_if->subst(str, e);
  Expr* newThen = this->_then->subst(str, e);
  Expr* newElse = this->_else->subst(str, e);
  return new IfExpr(newIf, newThen, newElse);
}

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

EqualExpr::EqualExpr(Expr *lhs, Expr *rhs) {
  this -> lhs = lhs;
  this -> rhs = rhs;
}

bool EqualExpr::equals(Expr *e) {
  EqualExpr *otherEqualExpr = dynamic_cast<EqualExpr*>(e);
  if (otherEqualExpr == NULL) {
    return false;
  }
  else {
    return (this->lhs ->equals(otherEqualExpr->lhs) && (this->rhs->equals(otherEqualExpr->rhs)));
  }
}

Val* EqualExpr::interp() {
  return new BoolVal(this->lhs->interp()->equals(this->rhs->interp()));
}

bool EqualExpr::has_variable() {
  return (this->lhs->has_variable() || this->rhs->has_variable());
}

Expr* EqualExpr::subst(std::string str, Expr *e) {
  Expr* newlhs = this->lhs->subst(str, e);
  Expr* newrhs = this->rhs->subst(str, e);
  return new EqualExpr(newlhs, newrhs);
}

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

//--------------------------------***Tests***--------------------------------

TEST_CASE("print") {
    CHECK(to_string(new NumExpr(100)) == "100");
    CHECK(to_string(new VarExpr("xyz")) == "xyz");
    CHECK(to_string(new VarExpr("abc")) != "xyz");
    CHECK(to_string((new AddExpr((new NumExpr(10)), (new NumExpr(20))))) == "(10+20)");
    CHECK(to_string((new MultExpr((new NumExpr(10)), (new NumExpr(20))))) == "(10*20)");
    CHECK(to_string((new AddExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))) == "(1+(2+3))");
    CHECK(to_string((new AddExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))) == "((1+2)+3)");
}

TEST_CASE("pretty_print") {
    CHECK(to_string_pretty(new NumExpr(10)) == "10");
    CHECK(to_string_pretty(new VarExpr("xyz")) == "xyz");
    CHECK(to_string_pretty((new AddExpr((new NumExpr(10)), (new NumExpr(20))))) == "10 + 20");
    CHECK(to_string_pretty((new MultExpr((new NumExpr(10)), (new NumExpr(20))))) == "10 * 20");
    CHECK(to_string_pretty(new AddExpr(new NumExpr(1), new MultExpr(new NumExpr(2), new NumExpr(3)))) == "1 + 2 * 3");
    CHECK(to_string_pretty(new MultExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3)))) == "1 * (2 + 3)");
    CHECK(to_string_pretty(new AddExpr(new MultExpr(new NumExpr(1),new NumExpr(2)), new NumExpr(3))) == "1 * 2 + 3");
    CHECK(to_string_pretty(new MultExpr(new MultExpr(new NumExpr(2), new NumExpr(3)), new NumExpr(4))) == "(2 * 3) * 4");
    CHECK(to_string_pretty(new MultExpr(new NumExpr(2), new MultExpr(new NumExpr(3), new NumExpr(4)))) == "2 * 3 * 4");
}


TEST_CASE("equals"){
    SECTION("Num"){
        CHECK( (new NumExpr(1))->equals(new NumExpr(1)));
        CHECK( (new NumExpr(1010))->equals(new NumExpr(1010)));
        CHECK( ! (new NumExpr(1))->equals(new NumExpr(2)));
        CHECK( ! (new NumExpr(1))->equals(new VarExpr("b")));
        CHECK( ! (new NumExpr(1))->equals((new AddExpr(new NumExpr(2), new NumExpr(1)))) );
        CHECK( ! (new NumExpr(1))->equals((new MultExpr(new NumExpr(2), new NumExpr(3)))) );
        
              
    }
    
    SECTION("Add"){
        CHECK((new AddExpr(new NumExpr(1), new NumExpr(2)))
              ->equals(new AddExpr(new NumExpr(1), new NumExpr(2))));
        CHECK(! (new AddExpr(new NumExpr(1), new NumExpr(2)))
              ->equals(new AddExpr(new NumExpr(2),new NumExpr(1))));
        CHECK(! (new AddExpr(new NumExpr(10), new NumExpr(20)))
              ->equals(new AddExpr(new NumExpr(2),new NumExpr(2))));
        CHECK(! (new AddExpr(new NumExpr(100), new NumExpr(1)))
              ->equals(new NumExpr(33)));
    }
    
    SECTION("Mult"){
        CHECK( (new MultExpr(new NumExpr(1), new NumExpr(2)))->equals(new MultExpr(new NumExpr(1),new NumExpr(2))));
        CHECK(!(new MultExpr(new NumExpr(10), new NumExpr(10)))->equals(new AddExpr(new NumExpr(10),new NumExpr(10))));
        CHECK(! (new MultExpr(new NumExpr(2), new NumExpr(4)))
              ->equals(new MultExpr(new NumExpr(4),new NumExpr(4))));
        CHECK(! (new MultExpr(new NumExpr(10), new NumExpr(1)))
              ->equals(new NumExpr(99)));
    }
    
    SECTION("Var"){
        CHECK( (new VarExpr("a"))->equals(new VarExpr("a")));
        CHECK(! (new VarExpr("a"))->equals(new VarExpr("b")));
        CHECK(! (new VarExpr("a"))->equals(new VarExpr("abd")));
        CHECK(! (new VarExpr("a"))->equals(new NumExpr(1)));
    }


    SECTION("interp") {
        CHECK((new NumExpr(9))->interp() ->equals(new NumVal(9)));
        CHECK(!(new NumExpr(19))->interp() ->equals( new NumVal(1)));
        CHECK((new AddExpr((new NumExpr(100)), (new NumExpr(100))))->interp() ->equals( new NumVal(200)));
        CHECK((new MultExpr((new NumExpr(12)), (new NumExpr(2))))->interp() ->equals( new NumVal(24)));
        CHECK(!(new MultExpr((new NumExpr(1)), (new NumExpr(1))))->interp() ->equals( new NumVal(3)));
        CHECK_THROWS_WITH((new VarExpr("x"))
                          ->interp(), "This is not a integer!");
        CHECK_THROWS_WITH((new AddExpr((new VarExpr("xyz")), (new VarExpr("abc"))))
                          ->interp(), "This is not a integer!");
        CHECK_THROWS_WITH((new MultExpr((new VarExpr("hello")), (new NumExpr(1000))))
                          ->interp(), "This is not a integer!");
    }
    

    SECTION("subst") {
        CHECK((new NumExpr(10))->subst("xyz", new VarExpr("abc"))
              ->equals(new NumExpr(10)));
        CHECK((new NumExpr(10))->subst("xyz", new NumExpr(10))
              ->equals(new NumExpr(10)));
        CHECK((new VarExpr("xyz"))->subst("xyz", new VarExpr("abc"))
              ->equals(new VarExpr("abc")));
        CHECK((new VarExpr("xyz"))->subst("xyz", new NumExpr(10))->equals(new NumExpr(10)));
        CHECK((new VarExpr("xyz"))->subst("xyz", (new AddExpr((new NumExpr(10)), (new NumExpr(20)))))
              ->equals(new AddExpr((new NumExpr(10)), (new NumExpr(20)))));
        CHECK((new VarExpr("xyz"))->subst("abc", (new AddExpr((new NumExpr(10)), (new NumExpr(20)))))
              ->equals(new VarExpr("xyz")));
        CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
               ->subst("x", new VarExpr("y"))
               ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
        CHECK((new AddExpr((new NumExpr(10)), (new NumExpr(20))))->subst("xyz", new VarExpr("abc"))
              ->equals(new AddExpr((new NumExpr(10)), (new NumExpr(20)))));
        CHECK(!(new AddExpr((new NumExpr(10)), (new VarExpr("xyz"))))
              ->subst("xyz", new VarExpr("abc"))->equals(new AddExpr((new NumExpr(10)), (new VarExpr("xyz")))));
        CHECK((new AddExpr((new VarExpr("xyz")), (new VarExpr("abc"))))->subst("xyz", new VarExpr("a"))
              ->equals(new AddExpr((new VarExpr("a")), (new VarExpr("abc")))));
        CHECK((new AddExpr((new VarExpr("abc")), (new NumExpr(10))))->subst("abc", new VarExpr("xyz"))
              ->equals(new AddExpr((new VarExpr("xyz")), (new NumExpr(10)))));
        CHECK((new MultExpr((new NumExpr(10)), (new NumExpr(5))))
              ->subst("abc", new NumExpr(29))->equals(new MultExpr((new NumExpr(10)), (new NumExpr(5)))));
        CHECK((new MultExpr((new NumExpr(10)), (new VarExpr("abc"))))
              ->subst("abc", new NumExpr(20))->equals(new MultExpr((new NumExpr(10)), (new NumExpr(20)))));
        CHECK((new MultExpr((new VarExpr("abc")), (new VarExpr("abc"))))->subst("abc", new NumExpr(10))
              ->equals(new MultExpr((new NumExpr(10)), (new NumExpr(10)))));
        CHECK((new MultExpr((new VarExpr("abc")), (new NumExpr(10))))->subst("abc", new NumExpr(20))->equals(new MultExpr((new NumExpr(20)), (new NumExpr(10)))));
    }
    
}

TEST_CASE("_let"){
    SECTION("equal"){
        CHECK((new _letExpr("a", new NumExpr(10), new NumExpr(20)))->equals((new _letExpr("a", new NumExpr(10), new NumExpr(20)))));
        CHECK(!(new _letExpr("a", new NumExpr(10), new NumExpr(20)))->equals((new _letExpr("a", new NumExpr(20), new NumExpr(10)))));
        CHECK(!(new _letExpr("a", new NumExpr(10), new NumExpr(20)))->equals((new _letExpr("a", new NumExpr(10), new AddExpr(new VarExpr("a"), new NumExpr(10))))));
        CHECK((new _letExpr("a", new NumExpr(10), new AddExpr(new VarExpr("a"), new NumExpr(20))))
              ->equals(new _letExpr("a", new NumExpr(10), new AddExpr(new VarExpr("a"), new NumExpr(20)))));
        CHECK(!(new _letExpr("a", new NumExpr(10), new NumExpr(20)))->equals(new NumExpr(1)));
    }
   
    SECTION("interp"){
        CHECK((new _letExpr("a", new NumExpr(10), new NumExpr(20)))->interp() ->equals( new NumVal(20)));
        CHECK((new _letExpr("a", new NumExpr(10), new AddExpr(new VarExpr("a"), new NumExpr(20))))->interp() ->equals( new NumVal(30)));
        CHECK_THROWS_WITH((new _letExpr("a", new VarExpr("a"), new NumExpr(10)))->interp(),"This is not a integer!");
        CHECK_THROWS_WITH((new _letExpr("a", new VarExpr("b"), new AddExpr(new VarExpr("b"), new NumExpr(10))))->interp(), "This is not a integer!");
        CHECK((new BoolExpr(true))->interp()->equals(new BoolVal(true)));
          CHECK(!(new BoolExpr(true))->interp()->equals(new BoolVal(false)));
          CHECK((new IfExpr(new BoolExpr(true), new NumExpr(10), new NumExpr(20)))
                ->interp()->equals(new NumVal(10)));
          CHECK((new IfExpr(new BoolExpr(false), new NumExpr(10), new NumExpr(20)))
                ->interp()->equals(new NumVal(20)));
          CHECK((new EqualExpr(new NumExpr(10), new NumExpr(10)))
                ->interp()->equals(new BoolVal(true)));
          CHECK((new EqualExpr(new NumExpr(10), new NumExpr(20)))
                ->interp()->equals(new BoolVal(false)));
    }
    
    SECTION("has_variable"){
        CHECK(!(new NumExpr(100))->has_variable());
        CHECK((new VarExpr("xyz"))->has_variable());
        CHECK(!(new AddExpr((new NumExpr(10)), (new NumExpr(10))))
              ->has_variable());
        CHECK((new AddExpr((new VarExpr("xyz")), (new NumExpr(10))))
              ->has_variable());
        CHECK(!(new MultExpr((new NumExpr(10)), (new NumExpr(10))))
              ->has_variable());
        CHECK((new MultExpr((new VarExpr("xyz")), (new NumExpr(10))))
              ->has_variable());
        CHECK(!(new _letExpr("x", new NumExpr(10), new NumExpr(20)))->has_variable());
        CHECK((new _letExpr("a", new VarExpr("b"), new VarExpr("c")))->has_variable());
        CHECK((new _letExpr("a", new AddExpr(new NumExpr(10), new VarExpr("b")), new AddExpr(new NumExpr(10), new NumExpr(20))))
              ->has_variable());
        CHECK(!(new _letExpr("a", new AddExpr(new NumExpr(3), new NumExpr(4)), new AddExpr(new NumExpr(100), new NumExpr(10))))
              ->has_variable());
        CHECK(!(new BoolExpr(true))->has_variable());
          CHECK(!(new BoolExpr(false))->has_variable());
          CHECK(!(new IfExpr(new NumExpr(10), new NumExpr(20), new NumExpr(30)))
                ->has_variable());
          CHECK((new IfExpr(new VarExpr("10"), new NumExpr(20), new NumExpr(30)))
                ->has_variable());
          CHECK(!(new EqualExpr(new NumExpr(10),new NumExpr(20)))->has_variable());
          CHECK((new EqualExpr(new VarExpr("10"),new NumExpr(20)))->has_variable());
        
    }
    
    SECTION("subst"){
        CHECK((new _letExpr("a", new NumExpr(3), new NumExpr(5)))
              ->subst("a", new VarExpr("z"))->equals((new _letExpr("a", new NumExpr(3), new NumExpr(5)))));
        CHECK((new _letExpr("a", new VarExpr("y"), new VarExpr("z")))
              ->subst("a", new VarExpr("xyz"))->equals((new _letExpr("a", new VarExpr("y"), new VarExpr("z")))));
        CHECK((new _letExpr("a", new VarExpr("b"), new NumExpr(10)))
              ->subst("b", new NumExpr(5))->equals((new _letExpr("a", new NumExpr(5), new NumExpr(10)))));
        CHECK((new _letExpr("a", new AddExpr(new NumExpr(10), new VarExpr("b")), new AddExpr(new VarExpr("b"), new NumExpr(20))))
              ->subst("b", new NumExpr(5))->equals((new _letExpr("a", new AddExpr(new NumExpr(10), new NumExpr(5)), new AddExpr(new NumExpr(5), new NumExpr(20))))));
        CHECK((new BoolExpr(true))->subst("MSD", new NumExpr(10))
              ->equals(new BoolExpr(true)));
          CHECK(!(new BoolExpr(true))->subst("MSD", new NumExpr(10))->equals(new BoolExpr(false)));
          CHECK((new IfExpr(new VarExpr("a"), new NumExpr(10), new NumExpr(20)))->subst("a", new VarExpr("b"))->equals(new IfExpr(new VarExpr("b"), new NumExpr(10), new NumExpr(20))));
          CHECK((new IfExpr(new NumExpr(10), new VarExpr("a"), new NumExpr(7)))->subst("a", new VarExpr("b"))->equals(new IfExpr(new NumExpr(10), new VarExpr("b"), new NumExpr(7))));
          CHECK((new IfExpr(new NumExpr(10), new NumExpr(20), new VarExpr("a")))->subst("a", new VarExpr("b"))->equals(new IfExpr(new NumExpr(10), new NumExpr(20), new VarExpr("b"))));
          CHECK((new EqualExpr(new VarExpr("a"), new NumExpr(10)))->subst("a", new VarExpr("b"))->equals(new EqualExpr(new VarExpr("b"), new NumExpr(10))));
          CHECK((new EqualExpr(new NumExpr(10), new VarExpr("b")))->subst("a", new VarExpr("b"))->equals(new EqualExpr(new NumExpr(10), new VarExpr("b"))));
    }
    
    SECTION("print"){
        CHECK(to_string(new _letExpr("a", new NumExpr(10), new NumExpr(20))) == "(_let a=10 _in 20)");
        CHECK(to_string(new _letExpr("a", new VarExpr("b"), new VarExpr("c"))) == "(_let a=b _in c)");
        CHECK(to_string(new _letExpr("a", new VarExpr("b"), new AddExpr(new NumExpr(10), new NumExpr(20)))) == "(_let a=b _in (10+20))");
        CHECK(to_string(new _letExpr("x", new AddExpr(new NumExpr(10), new VarExpr("xyz")), new VarExpr("a"))) == "(_let x=(10+xyz) _in a)");
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
//
//    }
}

TEST_CASE("EQUAL"){
    CHECK((new BoolExpr(true))->equals(new BoolExpr(true)));
      CHECK(!(new BoolExpr(true))->equals(new BoolExpr(false)));
      CHECK(!(new BoolExpr(true))->equals(new NumExpr(0)));
      CHECK((new IfExpr(new NumExpr(10), new NumExpr(20), new NumExpr(30)))
            ->equals(new IfExpr(new NumExpr(10), new NumExpr(20), new NumExpr(30))));
      CHECK(!(new IfExpr(new NumExpr(10), new NumExpr(20), new NumExpr(30)))
            ->equals(new IfExpr(new NumExpr(10), new NumExpr(30), new NumExpr(20))));
      CHECK(!(new IfExpr(new NumExpr(10), new NumExpr(20), new NumExpr(30)))
            ->equals(new NumExpr(20)));
      CHECK((new EqualExpr(new NumExpr(10), new NumExpr(20)))
            ->equals(new EqualExpr(new NumExpr(10), new NumExpr(20))));
      CHECK(!(new EqualExpr(new NumExpr(10), new NumExpr(20)))
            ->equals(new EqualExpr(new NumExpr(10), new NumExpr(30))));
      CHECK(!(new EqualExpr(new NumExpr(10), new NumExpr(20)))
            ->equals(new NumExpr(20)));
        
}

TEST_CASE("toString"){
    
    CHECK(to_string(new BoolExpr(true)) == "_true");
    CHECK(to_string(new BoolExpr(false)) == "_false");
    CHECK(to_string(new IfExpr(new VarExpr("x"), new NumExpr(10), new NumExpr(20))) == "(_if x _then 10 _else 20)");
    CHECK((to_string(new EqualExpr(new NumExpr(100), new NumExpr(10))) == "(100==10)"));
}

                                                             


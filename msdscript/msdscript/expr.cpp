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
Num::Num(int val){
    this->val=val;
}


//check if the given value equals
bool Num::equals(Expr *e){
    Num *c = dynamic_cast<Num*>(e);
    if(c == NULL){
        return false;
    }else{
        return val == c->val;
    }
}

//return the value to the integer
int Num::interp(){
    return this->val;
}

//return false as nums must be an integer
bool Num::has_variable(){
    return false;
}

//return this as it is an integer
Expr* Num::subst(std::string original, Expr* replacement){
    return this;
}

void Num::print(std::ostream &out){
    out << this ->val;
}

void Num::pretty_print(std::ostream &out){
    pretty_print_at(out, prec_none, 0);
}

void Num::pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed){
    out << this ->val;
}


//--------------------------------***Add***--------------------------------

Add::Add(Expr *lhs, Expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

//check if the given value equals
bool Add::equals(Expr *e){
    Add *c = dynamic_cast<Add*>(e);
    if (c == NULL){
        return false;
    }else{
        return lhs->equals(c->lhs) && rhs->equals(c->rhs);
    }
}

//return the value of an addition expression is the sum of the subexpression values
int Add::interp(){
    return lhs->interp()+rhs->interp();
}

//The result should be true if the expression is a variable or contains a variable, false otherwise

bool Add::has_variable(){
    return lhs->has_variable()||rhs->has_variable();
}

//subst for Add
Expr* Add::subst(std::string original, Expr* replacement){
    return new Add(lhs->subst(original,replacement),rhs->subst(original, replacement));
}

//print for add
void Add::print(std::ostream &out){
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

void Add::pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed){
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
void Add::pretty_print(std::ostream &out){
    lhs->pretty_print_at(out, prec_add, 0);
    out << " + ";
    rhs->pretty_print_at(out, prec_none, 0);
}

//--------------------------------***Mult***--------------------------------

Mult::Mult(Expr *lhs, Expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

//check if the given value equals 
bool Mult::equals(Expr *e){
    Mult *c = dynamic_cast<Mult*>(e);
    if(c == NULL){
        return false;
    }else{
        return lhs->equals(c->lhs) && rhs->equals(c->rhs);
    }
}

//return the value of a multiplication expression is the product of the subexpression values
int Mult::interp(){
    return lhs->interp()*rhs->interp();
}

//The result should be true if the expression is a variable or contains a variable, false otherwise.

bool Mult::has_variable(){
    return lhs->has_variable()||rhs->has_variable();
}

//subst for Mult
Expr* Mult::subst(std::string original, Expr *replacement){
    return new Mult(lhs->subst(original, replacement), rhs->subst(original,replacement));
}

//print for Mult
void Mult::print(std::ostream &out){
    std::stringstream lhsOut(" ");
    std::stringstream rhsOut(" ");
    lhs->print(lhsOut);
    rhs->print(rhsOut);
    out << "(" << lhsOut.str() << "*" << rhsOut.str() << ")";
}

void Mult::pretty_print_at(std::ostream &out,precedence_t prec, int spaceNeed){
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
void Mult::pretty_print(std::ostream &out){
    this->lhs->pretty_print_at(out, prec_mult, 0);
    out << " * ";
    this->rhs->pretty_print_at(out, prec_let, 0);
        
}

//--------------------------------***Var***--------------------------------

Var::Var(std::string variable){
    this->variable = variable;
}

//check if the given value equals
bool Var::equals(Expr *e){
    Var *c = dynamic_cast<Var*>(e);
    if(c ==NULL){
        return false;
    }else{
        return variable == c->variable;
    }
}

//throw an exception as a variable is not an integer
int Var::interp(){
    throw std::runtime_error("This is not a integer!");
}

//return true as Var must be a variable
bool Var::has_variable(){
    return true;
}


//return replaced value if variable equals original
Expr* Var::subst(std::string original, Expr *replacement){
    if(this->variable==original){
        return replacement;
    }else{
        return this;
    }
}

//print for var
void Var::print(std::ostream &out){
    out << this ->variable;
}

//pretty print for var
void Var::pretty_print(std::ostream &out){
    pretty_print_at(out, prec_none, 0);
}

void Var::pretty_print_at(std::ostream &out, precedence_t prec, int spaceNeed){
    out << this ->variable;
}


//--------------------------------***Let***----------------------------------


_let::_let(std::string _letVariable, Expr *rhs, Expr *body){
    this->_letVariable = _letVariable;
    this->rhs = rhs;
    this->body = body;
};


bool _let::equals(Expr *e){
    _let *c = dynamic_cast<_let*>(e);
    if(c == NULL){
        return false;
    }else{
        return rhs->equals(c->rhs) && body->equals(c->body)&& _letVariable == (c->_letVariable);
    }
}

int _let::interp(){
    int temp = rhs->interp();
    return body->subst(_letVariable, new Num(temp))->interp();
}


bool _let::has_variable(){
    return rhs->has_variable() || body->has_variable();
}


Expr* _let::subst(std::string original, Expr* replacement){
    if(_letVariable != original){
        return new _let(_letVariable, rhs->subst(original, replacement),body->subst(original,replacement));
    }else{
        return new _let(_letVariable, rhs->subst(original, replacement), body);
    }
}


void _let::print(std::ostream &out){
    out<<"(_let "<<_letVariable<<"=";
    rhs->print(out);
    out<<" _in ";
    body->print(out);
    out<<")";
    
}

void _let::pretty_print(std::ostream &out){
    int pos = out.tellp();
    out << "_let " << _letVariable << " = ";
    rhs->pretty_print_at(out, prec_none
                         , pos);
    out << "\n";
    pos = out.tellp();
    out << "_in ";
    body->pretty_print_at(out, prec_none, pos);

};

void _let::pretty_print_at(std::ostream &out,precedence_t prec, int spaceNeed){
    int currentPos = out.tellp();
    if(prec == prec_add || prec == prec_mult){
        out << "(";
        currentPos++;
    }
    
    out << "_let " << _letVariable << " = ";
    rhs->pretty_print_at(out, prec_none, spaceNeed);
    out <<"\n";
    int newPos = out.tellp();
    int space = currentPos - spaceNeed;
    for( int i =0; i<space; i++){
        out << " ";
    }
    out << "_in ";
    this->body->pretty_print_at(out, prec_none, newPos);
    if(prec == prec_add || prec == prec_mult){
        out << ")";
    }
}

//--------------------------------***Tests***--------------------------------

TEST_CASE("print") {
    CHECK(to_string(new Num(100)) == "100");
    CHECK(to_string(new Var("xyz")) == "xyz");
    CHECK(to_string(new Var("abc")) != "xyz");
    CHECK(to_string((new Add((new Num(10)), (new Num(20))))) == "(10+20)");
    CHECK(to_string((new Mult((new Num(10)), (new Num(20))))) == "(10*20)");
    CHECK(to_string((new Add(new Num(1), new Add(new Num(2), new Num(3))))) == "(1+(2+3))");
    CHECK(to_string((new Add(new Add(new Num(1), new Num(2)), new Num(3)))) == "((1+2)+3)");
}

TEST_CASE("pretty_print") {
    CHECK(to_string_pretty(new Num(10)) == "10");
    CHECK(to_string_pretty(new Var("xyz")) == "xyz");
    CHECK(to_string_pretty((new Add((new Num(10)), (new Num(20))))) == "10 + 20");
    CHECK(to_string_pretty((new Mult((new Num(10)), (new Num(20))))) == "10 * 20");
    CHECK(to_string_pretty(new Add(new Num(1), new Mult(new Num(2), new Num(3)))) == "1 + 2 * 3");
    CHECK(to_string_pretty(new Mult(new Num(1), new Add(new Num(2), new Num(3)))) == "1 * (2 + 3)");
    CHECK(to_string_pretty(new Mult(new Mult(new Num(2), new Num(3)), new Num(4))) == "(2 * 3) * 4");
    CHECK(to_string_pretty(new Mult(new Num(2), new Mult(new Num(3), new Num(4)))) == "2 * 3 * 4");
}


TEST_CASE("equals"){
    SECTION("Num"){
        CHECK( (new Num(1))->equals(new Num(1)));
        CHECK( (new Num(1010))->equals(new Num(1010)));
        CHECK( ! (new Num(1))->equals(new Num(2)));
        CHECK( ! (new Num(1))->equals(new Var("b")));
        CHECK( ! (new Num(1))->equals((new Add(new Num(2), new Num(1)))) );
        CHECK( ! (new Num(1))->equals((new Mult(new Num(2), new Num(1)))) );
              
    }
    
    SECTION("Add"){
        CHECK((new Add(new Num(1), new Num(2)))
              ->equals(new Add(new Num(1), new Num(2))));
        CHECK(! (new Add(new Num(1), new Num(2)))
              ->equals(new Add(new Num(2),new Num(1))));
        CHECK(! (new Add(new Num(10), new Num(20)))
              ->equals(new Add(new Num(2),new Num(2))));
        CHECK(! (new Add(new Num(100), new Num(1)))
              ->equals(new Num(33)));
    }
    
    SECTION("Mult"){
        CHECK( (new Mult(new Num(1), new Num(2)))->equals(new Mult(new Num(1),new Num(2))));
        CHECK(!(new Mult(new Num(10), new Num(10)))->equals(new Add(new Num(10),new Num(10))));
        CHECK(! (new Mult(new Num(2), new Num(4)))
              ->equals(new Mult(new Num(4),new Num(4))));
        CHECK(! (new Mult(new Num(10), new Num(1)))
              ->equals(new Num(99)));
    }
    
    SECTION("Var"){
        CHECK( (new Var("a"))->equals(new Var("a")));
        CHECK(! (new Var("a"))->equals(new Var("b")));
        CHECK(! (new Var("a"))->equals(new Var("abd")));
        CHECK(! (new Var("a"))->equals(new Num(1)));
    }

    
    SECTION("interp") {
        CHECK((new Num(9))->interp() == 9);
        CHECK((new Num(19))->interp() != 1);
        CHECK((new Add((new Num(100)), (new Num(100))))->interp() == 200);
        CHECK((new Mult((new Num(12)), (new Num(2))))->interp() == 24);
        CHECK((new Mult((new Num(1)), (new Num(1))))->interp() != 3);
        CHECK_THROWS_WITH((new Var("x"))
                          ->interp(), "This is not a integer!");
        CHECK_THROWS_WITH((new Add((new Var("xyz")), (new Var("abc"))))
                          ->interp(), "This is not a integer!");
        CHECK_THROWS_WITH((new Mult((new Var("hello")), (new Num(1000))))
                          ->interp(), "This is not a integer!");
    }
    
    SECTION("has_variable") {
        CHECK(!(new Num(100))->has_variable());
        CHECK((new Var("xyz"))->has_variable());
        CHECK(!(new Add((new Num(10)), (new Num(10))))
              ->has_variable());
        CHECK((new Add((new Var("xyz")), (new Num(10))))
              ->has_variable());
        CHECK(!(new Mult((new Num(10)), (new Num(10))))
              ->has_variable());
        CHECK((new Mult((new Var("xyz")), (new Num(10))))
              ->has_variable());
    }
    SECTION("subst") {
        CHECK((new Num(10))->subst("xyz", new Var("abc"))
              ->equals(new Num(10)));
        CHECK((new Num(10))->subst("xyz", new Num(10))
              ->equals(new Num(10)));
        
        CHECK((new Var("xyz"))->subst("xyz", new Var("abc"))
              ->equals(new Var("abc")));
        CHECK((new Var("xyz"))->subst("xyz", new Num(10))->equals(new Num(10)));
        
        CHECK((new Var("xyz"))->subst("xyz", (new Add((new Num(10)), (new Num(20)))))
              ->equals(new Add((new Num(10)), (new Num(20)))));
        CHECK((new Var("xyz"))->subst("abc", (new Add((new Num(10)), (new Num(20)))))
              ->equals(new Var("xyz")));
        
        
        CHECK( (new Add(new Var("x"), new Num(7)))
               ->subst("x", new Var("y"))
               ->equals(new Add(new Var("y"), new Num(7))) );
        CHECK((new Add((new Num(10)), (new Num(20))))->subst("xyz", new Var("abc"))
              ->equals(new Add((new Num(10)), (new Num(20)))));
        CHECK(!(new Add((new Num(10)), (new Var("xyz"))))
              ->subst("xyz", new Var("abc"))->equals(new Add((new Num(10)), (new Var("xyz")))));
        CHECK((new Add((new Var("xyz")), (new Var("abc"))))->subst("xyz", new Var("a"))
              ->equals(new Add((new Var("a")), (new Var("abc")))));
        CHECK((new Add((new Var("abc")), (new Num(10))))->subst("abc", new Var("xyz"))
              ->equals(new Add((new Var("xyz")), (new Num(10)))));
       
       
        CHECK((new Mult((new Num(10)), (new Num(5))))
              ->subst("abc", new Num(29))->equals(new Mult((new Num(10)), (new Num(5)))));
        CHECK((new Mult((new Num(10)), (new Var("abc"))))
              ->subst("abc", new Num(20))->equals(new Mult((new Num(10)), (new Num(20)))));
        CHECK((new Mult((new Var("abc")), (new Var("abc"))))->subst("abc", new Num(10))
              ->equals(new Mult((new Num(10)), (new Num(10)))));
        CHECK((new Mult((new Var("abc")), (new Num(10))))->subst("abc", new Num(20))->equals(new Mult((new Num(20)), (new Num(10)))));

    }
    
//    CHECK_THROWS_WITH( (new Var("x"))->interp(), "no value for variable" );
// }
//

}
TEST_CASE("_let"){
    SECTION("equal"){
        CHECK((new _let("a", new Num(10), new Num(20)))->equals((new _let("a", new Num(10), new Num(20)))));
        CHECK(!(new _let("a", new Num(10), new Num(20)))->equals((new _let("a", new Num(20), new Num(10)))));
        CHECK(!(new _let("a", new Num(10), new Num(20)))->equals((new _let("a", new Num(10), new Add(new Var("a"), new Num(10))))));
        CHECK((new _let("a", new Num(10), new Add(new Var("a"), new Num(20))))
              ->equals(new _let("a", new Num(10), new Add(new Var("a"), new Num(20)))));
    }
   
    SECTION("interp"){
        CHECK((new _let("a", new Num(10), new Num(20)))->interp() == 20);
        CHECK((new _let("a", new Num(10), new Add(new Var("a"), new Num(20))))->interp() == 30);
        CHECK_THROWS_WITH((new _let("a", new Var("a"), new Num(10)))->interp(),"This is not a integer!");
        CHECK_THROWS_WITH((new _let("a", new Var("b"), new Add(new Var("b"), new Num(10))))->interp(), "This is not a integer!");
    }
    
    SECTION("has_variable"){
        CHECK(!(new _let("x", new Num(10), new Num(20)))->has_variable());
        CHECK((new _let("a", new Var("b"), new Var("c")))->has_variable());
        CHECK((new _let("a", new Add(new Num(10), new Var("b")), new Add(new Num(10), new Num(20))))
              ->has_variable());
        CHECK(!(new _let("a", new Add(new Num(3), new Num(4)), new Add(new Num(100), new Num(10))))
              ->has_variable());
    }
    
    SECTION("subst"){
        CHECK((new _let("a", new Num(3), new Num(5)))
              ->subst("a", new Var("z"))->equals((new _let("a", new Num(3), new Num(5)))));
        CHECK((new _let("a", new Var("y"), new Var("z")))
              ->subst("a", new Var("xyz"))->equals((new _let("a", new Var("y"), new Var("z")))));
        CHECK((new _let("a", new Var("b"), new Num(10)))
              ->subst("b", new Num(5))->equals((new _let("a", new Num(5), new Num(10)))));
        CHECK((new _let("a", new Add(new Num(10), new Var("b")), new Add(new Var("b"), new Num(20))))
              ->subst("b", new Num(5))->equals((new _let("a", new Add(new Num(10), new Num(5)), new Add(new Num(5), new Num(20))))));
    }
    SECTION("print"){
        CHECK(to_string(new _let("a", new Num(10), new Num(20))) == "(_let a=10 _in 20)");
        CHECK(to_string(new _let("a", new Var("b"), new Var("c"))) == "(_let a=b _in c)");
        CHECK(to_string(new _let("a", new Var("b"), new Add(new Num(10), new Num(20)))) == "(_let a=b _in (10+20))");
        CHECK(to_string(new _let("x", new Add(new Num(10), new Var("xyz")), new Var("a"))) == "(_let x=(10+xyz) _in a)");
    }
    
    SECTION("pretty_print"){
        CHECK(to_string_pretty(new _let("a", new Num(10), new Add(new Var("a"), new Num(20)))) ==
             "_let a = 10\n"
             "_in a + 20");
        CHECK(to_string_pretty(new _let("a", new Num(10), new Add(new _let("b", new Num(20), new Add(new Var("b"), new Num(6))), new Var("a")))) ==
              "_let a = 10\n"
              "_in (_let b = 20\n"
              "     _in b + 6) + a");
        CHECK(to_string_pretty(new Add(new Mult(new _let("x", new Num(10), new Var("x")), new Num(10)), new Num(20))) ==
               "(_let x = 10\n"
               " _in x) * 10 + 20");

    }
}

                                                             

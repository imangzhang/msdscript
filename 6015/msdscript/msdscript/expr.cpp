//
//  expr.cpp
//  msdscript
//
//  Created by Ang Zhang on 1/17/22.
//

#include "expr.hpp"
#include "catch.h"
#include <stdexcept>



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


//--------------------------------***Tests***--------------------------------
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
    
                                                             


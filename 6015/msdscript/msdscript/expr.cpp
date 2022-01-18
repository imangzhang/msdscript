//
//  expr.cpp
//  msdscript
//
//  Created by Ang Zhang on 1/17/22.
//

#include "expr.hpp"
#include "catch.h"

Num::Num(int val){
    this->val=val;
}

bool Num::equals(Expr *e){
    Num *c = dynamic_cast<Num*>(e);
    if(c == NULL){
        return false;
    }else{
        return val == c->val;
    }
}

Add::Add(Expr *lhs, Expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Add::equals(Expr *e){
    Add *c = dynamic_cast<Add*>(e);
    if (c == NULL){
        return false;
    }else{
        return lhs->equals(c->lhs) && rhs->equals(c->rhs);
    }
}

Mult::Mult(Expr *lhs, Expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Mult::equals(Expr *e){
    Mult *c = dynamic_cast<Mult*>(e);
    if(c == NULL){
        return false;
    }else{
        return lhs->equals(c->lhs) && rhs->equals(c->rhs);
    }
}

Var::Var(std::string variable){
    this->variable = variable;
}

bool Var::equals(Expr *e){
    Var *c = dynamic_cast<Var*>(e);
    if(c ==NULL){
        return false;
    }else{
        return variable == c->variable;
    }
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
 }


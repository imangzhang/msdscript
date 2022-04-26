//
//  main.cpp
//  msdscript
//
//  Created by Ang Zhang on 1/12/22.
//
#include <iostream>
#include "cmdline.hpp"
#include "env.hpp"
#include "parse.hpp"
#include "var.hpp"
#include "catch.h"
#include "expr.hpp"

int main(int argc, char **argv){
  //calls use_arguments function 
  use_arguments(argc, argv);
    return 0;
}

//
//  cmdline.cpp
//  msdscript
//
//  Created by Ang Zhang on 1/12/22.
//

#define CATCH_CONFIG_RUNNER
#include "cmdline.hpp"
#include <string>
#include <iostream>
#include "catch.h"

//function of use_arguments
//parameter: argc, argv
void use_arguments(int argc, char **argv){
    int testCount =false; //the default show of "--test" is false
    if(argc >1){ //the condition to go in for loop
    for(int i=1; i<argc;i++){ //start on position 1
        std::string input = argv[i];
        if(input=="--help"){ //"--help" condition
            std::cout<<"There is two arguments are allowed: '--help' and '--test'."<<"\n";
            exit(0);
        }
        else if (input=="--test"){//"--test" condition
            if(testCount==false){//only the "--test" never has been entered, the
                                //test will pass
                //Catch::Session().run();
                if (Catch::Session().run() == 0){
                    std::cout<<"Tests passed."<<"\n";
                    testCount =true;
                }else{
                    std::cout<<"Tests failed."<<"\n";
                    exit(1);
                }
            }
            else{std::cerr<<"error!"<<"\n"; //otherwise, it will warn an error
                exit(1);
            }
        }
        else{std::cout<<"This argument is not allowed!"<<"\n"; //anything else will be an error
        exit(1);
        }
    }
 }
    return;
}

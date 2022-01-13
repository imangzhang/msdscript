//
//  cmdline.cpp
//  msdscript
//
//  Created by Ang Zhang on 1/12/22.
//

#include "cmdline.hpp"
#include <string>
#include <iostream>

//function of use_arguments
//parameter: argc, argv
std::string use_arguments(int argc, char **argv){
    int testCount =false; //the default show of "--test" is false
    if(argc >1){
    for(int i=1; i<argc;i++){
        std::string input = argv[i];
        if(input=="--help"){
            std::cout<<"There is two arguments are allowed: '--help' and '--test'."<<"\n";
            exit(0);
        }
        else if (input=="--test"){
            if(testCount==false){
            std::cout<<"Tests passed."<<"\n";
                testCount =true;
            }
            else{std::cerr<<"error!"<<"\n";
                exit(1);
            }
        }
        else{std::cout<<"This argument is not allowed!"<<"\n";
        exit(1);
        }
    }
 }
        return 0;
}

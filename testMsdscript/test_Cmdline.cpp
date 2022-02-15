//
//  test_Cmdline.cpp
//  testMsdscript
//
//  Created by Ang Zhang on 2/14/22.
//

#include "test_Cmdline.hpp"
#include <iostream>
#include "exec.h"
#include "test_msdscript.hpp"

void use_arguments(int argc, char **argv){
    
 //check the nums of argc and return different values
    if(argc ==1){
        std::cout<<"please insert at least 1 program!\n";
    }
    else if(argc ==2){
        const char * const interp_argv[] = {argv[1], "--interp"};
        const char * const print_argv[] = {argv[1], "--print"};
        const char * const pretty_print_argv[] = {argv[1], "--pretty-print"};
        for(int i = 0; i<100; i++){
        std::string in = random_expr_string();
            std::cout <<"Tring "<<in<<"\n";
            
        ExecResult interp_result = exec_program(2, interp_argv, in);
        ExecResult print_result = exec_program(2, print_argv, in);
        ExecResult pretty_print_result = exec_program(2, pretty_print_argv, in);
            
        ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
        ExecResult interp_again_result2= exec_program(2, interp_argv, pretty_print_result.out);
            
        if(interp_again_result.out != interp_result.out){
                throw std::runtime_error("different result for printed!");
                return;
            }
            
        if(interp_again_result2.out != interp_result.out){
                throw std::runtime_error("different result for printed!");
              return;
            }
            


        if(interp_result.exit_code ==0 || interp_result.exit_code ==1){
                std::cout << "interp successed!\n";
        }else{
                throw std::runtime_error("interp failed!\n");
        }

        if(print_result.exit_code ==0 || print_result.exit_code ==1){
                std::cout << "print successed!\n";
        }else{
                throw std::runtime_error("print failed!\n");
        }

        if(pretty_print_result.exit_code ==0 || pretty_print_result.exit_code ==1){
                std::cout << "pretty-print successed!\n";
        }else{
                throw std::runtime_error("pretty-print failed!\n");
        }
           
    }
        std::cout <<"program 1 all test passed!\n";
    
}else if(argc ==3){
    const char * const interp1_argv[] = {argv[1], "--interp"};
    const char * const print1_argv[] = {argv[1], "--print"};
    const char * const pretty1_print_argv[] = {argv[1], "--pretty-print"};
    
    const char * const interp2_argv[] = {argv[2], "--interp"};
    const char * const print2_argv[] = {argv[2], "--print"};
    const char * const pretty2_print_argv[] = {argv[2], "--pretty-print"};
        
    for(int i = 0; i<100; i++){
        std::string in = random_expr_string();
        std::cout<<"tring "<<in<<"\n";
        
        ExecResult interp1_result = exec_program(2, interp1_argv, in);
        ExecResult interp2_result = exec_program(2, interp2_argv, in);
        
        ExecResult print1_result = exec_program(2, print1_argv, in);
        ExecResult print2_result = exec_program(2, print2_argv, in);
        
        ExecResult pretty_print1_result = exec_program(2, pretty1_print_argv, in);
        ExecResult pretty_print2_result = exec_program(2, pretty2_print_argv, in);
        
        if(interp1_result.out != interp2_result.out){
            throw std::runtime_error("different results");
            return;
        }

        if(print1_result.out != print2_result.out){
            throw std::runtime_error("different results");
            return;
        }

        if(pretty_print1_result.out != pretty_print2_result.out){
            throw std::runtime_error("different results");
            return;
        }

        if(interp1_result.exit_code ==0 || interp1_result.exit_code ==1){
                std::cout << "interp1 successed!\n";
        }else{
                throw std::runtime_error("interp1 failed!\n");
            return;
        }

        if(print1_result.exit_code ==0 ||print1_result.exit_code ==1){
                std::cout << "print1 successed!\n";
        }else{
                throw std::runtime_error("print1 failed!\n");
            return;
        }

        if(pretty_print1_result.exit_code ==0 || pretty_print1_result.exit_code ==1){
                std::cout << "pretty-print1 successed!\n";
        }else{

                throw std::runtime_error("pretty-print1 failed!\n");
            return;
        }

        if(interp2_result.exit_code ==0 || interp2_result.exit_code ==1){
                std::cout << "interp2 successed!\n";
        }else{
                throw std::runtime_error("interp2 failed!\n");
            return;
        }

        if(print2_result.exit_code ==0 || print2_result.exit_code ==1){
                std::cout << "print2 successed!\n";
        }else{
                throw std::runtime_error("print2 failed!\n");
            return;
        }

        if(pretty_print2_result.exit_code ==0 || pretty_print2_result.exit_code ==1){
                std::cout << "pretty-print2 successed!\n";
        }else{
                throw std::runtime_error("pretty-print2 failed!\n");
            return;
        }
        
    }
    std::cout <<"program 2 all test passed!\n";
        
    }else{
        std::cerr << "Error: Only 2 programs allowed\n";
        return;
    }
}


//
//  main.cpp
//  testMsdscript
//
//  Created by Ang Zhang on 2/14/22.
//  @@@

#include <iostream>
#include "test_Cmdline.hpp"

int main(int argc, char ** argv) {
   try {
        use_arguments(argc, argv);
        return 0;
      } catch (std::runtime_error exn) {
        std::cerr << exn.what() << "\n";
        return 1;
      }
}

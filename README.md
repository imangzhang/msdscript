# MSDscript Description
MSDscript is an interpreter built upon a C++ that allows some mathematical expressions. MSDscript is a simple language so far. It doesn't have subtration. It's not far from the kinds of languages that are embedded in applications. For example, someone implementing a calendar program might want to have a language for advanced users to calculate dates for repeated meetings (say, more sophisticated than “every Tuesday”), and MSDscript could just about work for that.
# Getting Started
## Building and executing MSDscript
  1. Go to https://github.com/imangzhang/msdscript download MSDscript files to a local directory.
  2. Open your terminal and type `cd nameOfTheLocalDirectory` to the local directory which you downloaded the MSDscript file.
  3. Type `make` in your terminal
    - 'make' command will generate an excutable file 
  4. You will able to run all the MSDscript operations now
# MSDscript User Guide
## Performing commands in terminal
Here are four arguments are allowed in terminal:
- `./msdscript --help`: which tells you the detail of the arguments are allowed
- `./msdscript --test`: which will run the tests and tells you the results 
- `./msdscript --interp`: which do the mathematical calculations for you. Try below:       
  - type `2 + 2`
  - press Enter
  - press Control + D
  - it will show you the answer is 4         
- `./msdscript --print`: which will show you the Msdscript calculation rules. Try below:
   - type `2 + 3 * 6`
   - press Enter
   - press Control + D
   - it will show you the answer is (2+(3*6))
# MSDscript API
## Grammar 
     ```
	〈expr〉	=〈number〉 			
          |〈variable〉
          |〈boolean〉 
          |〈expr〉==〈expr〉 
          |〈expr〉+〈exr〉|〈expr〉*〈expr〉|〈expr〉(〈expr〉)
          | _let〈variable〉=〈expr〉_in〈expr〉 
          | _if〈expr〉_then〈expr〉_else〈expr〉 
          | _fun(〈variable〉)〈expr〉 
      ```
 

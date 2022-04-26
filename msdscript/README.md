# MSDscript Description
MSDscript is an interpreter built upon a C++ that allows some mathematical expressions. MSDscript is a simple language so far. It doesn't have subtration. It's not far from the kinds of languages that are embedded in applications. For example, someone implementing a calendar program might want to have a language for advanced users to calculate dates for repeated meetings (say, more sophisticated than “every Tuesday”), and MSDscript could just about work for that.
# Getting Started
## Building the excutable MSDscript
  1. Go to https://github.com/imangzhang/msdscript download MSDscript files to a local directory.
  2. Open your terminal and type `cd nameOfTheLocalDirectory` to the local directory which you downloaded the MSDscript file.
  3. Type `make` in your terminal
    - 'make' command will generate an excutable file 
  4. You will able to run all the MSDscript operations now
## Building the implementation as library
  1. Put the the application with the source codes in the same directory.
  2. Include needed ` .h`  files into the program ` .cpp`  file
    ``` 
    #include "parse.hpp"
    #include "expr.hpp"
    #include "env.hpp"
    #include "cmdline.hpp"
    ```
  3. Add the following code into the program ` .cpp`  file as well
    ``` 
    std::cout << parse_str(expr)->interp(Env::empty)->to_string() << std::endl;
    ``` 
  4. Open your terminal and run the commands below:

    `$c++ -std=c++14 -c cmdline.cpp env.cpp expr.cpp parse.cpp Val.cpp` 
    ` $ar -ruv libmsdcript.a cmdline.o env.o expr.o parse.o Val.o` 
    ` $c++ -o programName program.cpp libmsdscript.a` 

# MSDscript User Guide
## General Input and Keywords
MSDscript is able to parse input with extra white sapce and new lines. Any character from ASCII can be variable name except`_`, because some reserved keyword are denoted with `_`. 

###Keywords
- Numbers
    - Numbers in the MSDscript need to be positive or negative integers
    - Good Example: `1`, `80`, `1000000`, `-3`, `-10`, `0`, `-300000`
    - Bad Example: `1.0`, `2 000`, `2/5`, `3.14159`   
     
- Variables
     - Variable in the MSDscript need can be alphabetical `char` or `string` of upper or lowercase. 
     - Good Example: `a`, `abc`, `A`, `ABC`
     - Bad Example: `he11o`, `w o r l d`
     
- Booleans 
     - Boolean only represents ture or false and it has strict keywords in the MSDscript
     - Good Example: `_true`, `_false`
     - Bad Example: `true`, `false`, `-true`, `False`
     
- Addition
     - represents addtion by using `+` operator. `+` is combined with an expression on left-hand(lhs) and right-hand(rhs) sides  
     - Good Example: `1+2`  `1+(-2)` 
     - Bad Example: `1++2`
     
- Multiplication
     - represents multiplication of left-hand(lhs) and right-hand(rhs) sides by using `*` operator 
     - Good Example: `*`
     - Bad Example: `/`, `x`
     
- Comparison
     - uses `==` and represents comparison check of MSDscrit. It compares left-hand side(lhs) with right-hand side(rhs). It is able to compare any kinds of values, but addition and multiplication require that their subexpressions produce number values. 
     - Good Example: `1==1`, `1==5`
     - Bad Example: `1=1`, `1===1`

- Variable Assignment
     - Variable assignment let user could define a value for the varibale expression. 
     - Format: `_let(variable) = (expression) _in(expression)`
     - `_let` let the variable to be assigned, if any variable that appears in `_in`, MSDscript will substitue it. Otherwise, it will remain the original expression
     - Good Example: `_let x = 10 _in x + 1`-->result: 11
     - Bad Example: `let x = 10 in x + 1`
     
- If Statements
     - Used to compare the condition of two expressions. If the condition is true, the function will interp the expression to _then, otherwise, it will inter the expression to _else.
     - Good Example: `_if _true _then 2 _else 4`
     - Bad Example: `_if _true then 2 else 4`
     
- Functions
     - Represent a function expressions. It doesn't do anything in the program.
     - Keyword `_fun` used to present function in MSDscript
     - Good Example: `_fun(x) x + 1`
     - Bad Example: `fun(x) x + 1`, `_fun x x + 1`
     
- Function Calls
    - Call makes Function Expression being called to excute by passing an argument expression. 
    - Good Example: `(_fun(x) x + 1)(2)` -->result: 3
    - Bad Example: `(_fun(x) x + 1)2`

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
   
## Embed MSDscript in which_day program
The `which_day` program takes a week number (counting form 0) as a
command-line argument and tells you which day to meet that week (0 =
Sunday, 1 = Monday, etc.). 
The choice of meeting day is configured by a user who writes an
MSDscript program in "~/.which_day", so a user can set up an
arbitrarily complex meeting schedule. The program in "~/.which_day"
should produce a function that takes a week number.
After you build the implementation as library, type following command in your terminal:
      `which_day 13`
The output should be 4, meaning that you meet on Thursdat in week 13.  



# MSDscript API

## Important Vocabularies
    - Val: the different types of value that expr can interprete. The types include NumVal, BoolVal, and FunVal which represent number, boolean, and function values.  
    - expr: represents expression that can be utilized by MSDscript. MSDscript allows different types of exprs depend on the input.
    - Parse: convert a human readable expression to a script expression.
    - Interp: MSDscript's version that convert an expr to an Val
    - lhs: left-hand side of the expression
    - rhs: right-hand side of the expression
    
## Files
### Core files
    - ` expr.cpp`  and ` expr.hpp` : the main expression file which contains all the expressions
    - ` Val.cpp`  and ` Val.hpp` : store different type of values that needed by expressions
    - ` Env.cpp`  and ` Env.hpp` : allow quicker referencing of variable values
    - ` Step.cpp`  and ` Step.hpp` : for step interpretation that prevents segematation faults
    - ` pointer.cpp`  and ` pointer.hpp` : for switching the shared and regular pointers
    - ` Cont.cpp`  and ` Cont.hpp` : allow step mode interpretation    
### Optional files
    - ` main.cpp`  and ` main.hpp` : This file can be used for easy utilization of parse and interp methods.
    - ` parse.cpp`  and ` parse.hpp` : can parse the input strings to expressions. If you don't use parse then you don't need it
    
## Classes details
### Parse Class
Parse class makes MSDscript able to parse formated types of input to do actions such as: addtion, multiplication, and comparision.
  - Valid Syntax:
    - `PTR(Expr) parse(std::istream &inputStream);` takes in an istream, parse and return it as an Expr
    - `PTR(Expr) parseString(std::string s);` takes in a string and return it as an Expr. It is a wrapper function for the parse() function.

### Val Class
Val class is the main class for all other subclassess
#### Val Types
   - ` NumVal` Represents a number value
   - ` BoolVal`  Represents a boolean value
   - ` FunVal`  Represents a function value
#### Val Functions
   - ` bool equals(PTR(Val) o)`  returns `true` or `false` depends on if two values are equivalent. If they are equal, return `true`, else return `false`
   - ` PTR(Val) addTo(PTR(Val) otherVal)`  returns PTR(Val) of an addition value of the calling value to the parameter otherVal
   - ` PTR(Val) multBy(PTR(Val) otherVal)`  returns PTR(Val) of an mutiplication value of the calling value to the parameter otherVal
   - ` PTR(Val) call (PTR(Val) actualArg)`  returns PTR(Val) which call the function witn the input argument
   - ` std::string toString()`  returns string which converts current value to a string
   
   
### Expr Class
The main expression file which contains all the expressions
#### Expr Grammar 
   ```
    〈expr〉    =〈number〉             
          |〈variable〉
          |〈boolean〉 
          |〈expr〉==〈expr〉 
          |〈expr〉+〈exr〉|〈expr〉*〈expr〉|〈expr〉(〈expr〉)
          | _let〈variable〉=〈expr〉_in〈expr〉 
          | _if〈expr〉_then〈expr〉_else〈expr〉 
          | _fun(〈variable〉)〈expr〉 
          
   ```
#### Expr Types

   - NumExpr: 
     - Valid syntax: `NumExpr(int val)`
     - Member Variables: `int val`
     - Represents integer values
     - Example: `PTR(NumExpr) num2 = NEW(NumExpr)(2);`
     
   - VarExpr:
     - Valid syntax: `VarExpr(std::string variable)`
     - Member Variables: `std::string variable`
     - Represents a variable expression
     - Example: `PTR(VarExpr) varExpr = NEW(VarExpr)("abc");`
     
   - BoolExpr:
     - Valid syntax: `BoolExpr(bool val)`
     - Member Variables: `bool val`
     - represents a boolean expression
     - Example: `PTR(BoolExpr) trueBool = NEW(BoolExpr)("_true");`
     
   - EqualExpr:
     - Valid syntax: `EqualExpr(PTR(Expr) lhs, PTR(Expr) rhs)`
     - Member Variables: `PTR(Expr) lhs, PTR(Expr) rhs`
     - repre
     - Bad Example: `PTR(BoolExpr) trueBool = NEW(BoolExpr)("_true");`
     
   - AddExpr:
     - Valid syntax: `AddExpr(PTR(Expr) lhs, PTR(Expr) rhs)`
     - Member Variables: `PTR(Expr) lhs, PTR(Expr) rhs` 
     - Represents an addition expression
     - Example: `PTR(AddExpr) add = NEW(AddExpr)(num3, num8);` 
   - MultExpr:
     - Valid syntax: `MultExpr(PTR(Expr) lhs, PTR(Expr) rhs)`
     - Member Variables: `PTR(Expr) lhs, PTR(Expr) rhs`
     - Represents a muliplication expression
     - Example: `PTR(MultExpr) mult = NEW(MultExpr)(num10, num8);`
     
   - LetExpr:
     - Valid syntax: `_letExpr(std::string _letVariable, PTR(Expr) rhs, PTR(Expr) body)`
     - Member Variables: `std::string _letVariable, PTR(Expr) rhs, PTR(Expr) body`
     - Represents a variable assignment expression
     - Example: `PTR(LetExpr) basicFunWithLet = NEW(LetExpr) (NEW(VarExpr)("f"), NEW(FunExpr)(("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(10)));`
     
   - IfExpr:
     - Valid syntax: `IfExpr(PTR(Expr) _if, PTR(Expr) _then, PTR(Expr) _else)`
     - Member Variables: `PTR(Expr) _if, PTR(Expr) _then, PTR(Expr) _else`
     - Represents a comparison expression
     - Example: `PTR(IfExpr) compareIsNum = NEW(IfExpr)(num2, num2, num8);`
     
   - FunExpr:
     - Valid syntax: `FunExpr(std::string formal_arg, PTR(Expr) body)`
     - Member Variables: `std::string formal_arg, PTR(Expr) body`
     - Keyword `_fun` used to present function in MSDscript
     - Represents a function expression
     - Example: `PTR(FunExpr) funExpr = NEW(FunExpr)( "x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1) )`
     
  - CallExpr:
    - Valid syntax: `CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg)`
    - Member Variables: `PTR(Expr) to_be_called, PTR(Expr) actual_arg`
    - Represents a call to function expression
    - Example: `PTR(CallExpr) basicFunctionNoLet = NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), num2)), num8);`
    
#### Expr functions
There are total of three functions in Expr class. Each of these functions works the same for all the expr types. 
 
  - `virtual bool equals(PTR(Expr) o);` returns `true` or `false` depends on if two expressions are equivalent. If they are equal, return `true`, else return `false`
  - `virtual PTR(Val) interp(PTR(Env) env);` retruns `PTR(Val)` which interp the values of a given expression
  - `virtual void print(std::ostream &out);` return void which print an expression 
  - `virtual void print(std::ostream &out);` return void which converts an expression to a string
  

 
  


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
	〈expr〉=〈number〉 			
          |〈variable〉
          |〈boolean〉 
          |〈expr〉==〈expr〉 
          |〈expr〉+〈exr〉|〈expr〉*〈expr〉|〈expr〉(〈expr〉)
          | _let〈variable〉=〈expr〉_in〈expr〉 
          | _if〈expr〉_then〈expr〉_else〈expr〉 
          | _fun(〈variable〉)〈expr〉 
   ```
## Expression Details 
   - Numberical Expression: 
     - Numbers in the MSDscript need to be positive or negative integers.
     - Good Example: `1`, `80`, `1000000`, `-3`, `-10`, `0`, `-300000`
     - Bad Example: `1.0`, `2 000`, `2/5`, `3.14159`
   - Variable Expression:
     - Variable in the MSDscript need can be alphabetical `char` or `string` of upper or lowercase. 
     - Good Example: `a`, `abc`, `A`, `ABC`
     - Bad Example: `he11o`, `w o r l d`
   - Boolean Expression:
     - Boolean represents ture or false and it has strict keywords in the MSDscript
     - Good Example: `_true`, `_false`
     - Bad Example: `true`, `false`, `-true`, `False`
   - Comparison Expression:
     - Compares expressions. It uses double equal in the MSDscript
     - Good Example: `==`
     - Bad Example: `=`, `===`
   - Addition Expression:
     - Add numbers together. 
     - Good Example: `+`
     - Bad Example: `++`, 
   - Multiple Expression:
     - Mutiple numbers 
     - Good Example: `*`
     - Bad Example: `/`, `x`
   - Let Expression:
     - Substitute of <variavle> with <expr> at_let:
       - bind same <variable>: don't substitute in the body
       - bind different <variable>: substitue in the body
       - the `_in` keyword indicates where that substitue should be
     - Good Example: `_let x = 10 _in x + 1`
     - Bad Example: `let x = 10 in x + 1`
   - If/else Expression:
     - used to compare the condition of two expressions. If the condition is true, the function will interp the expression to _then, otherwise, it will inter the expression to _else.
     - Good Example: `_if _true _then 2 _else 4`
     - Bad Example: `_if _true then 2 else 4`
   - Function Expression
     - Keyword `_fun` used to present function in MSDscript
     - Good Example: `_fun(x) x + 1`
     - Bad Example: `fun(x) x + 1`, `_fun x x + 1`


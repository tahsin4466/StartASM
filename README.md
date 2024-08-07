# StartASM

## Overview
StartASM is a personal project that aims to create a simple, beginner-friendly assembly language. Its main goals are:
- Create a readable, English-like syntax with greater verbosity and clarity
- Abstract many complicated facets of assembly programming to reduce complexity
- Introduce assembly programming concepts in a straightforward, simplified manner

StartASM was created after personally struggling to learn x86 as a complete newcomer to assembly programming. I want this project to be a simpler, more intuitive introduction to assembly concepts that could be an effective stepping stone toward learning industry-standard languages. It is also a personal exercise in self-learning and is my first time engaging with many of these concepts myself.


## Features
StartASM will be composed of three parts:
- A custom compiler, written in C++, that validates and compiles StartASM code into intermediate instruction objects
- A full 32-bit simulator and runtime environment written in C++ - complete with registers and memory data structures - capable of executing StartASM instructions
- A custom IDE - built using Electron - to provide a visual development environment

The language is based on a load-store model, with separate program and instruction memory (Harvard) in a big-endian layout. The runtime environment will also feature 10 registers, 32 bit addressable memory and instruction pools as well as a simple stack. The following operations will be supported:
- move (between registers)
- load (memory to register)
- store (register to memory)
- create (value to register)
- cast (register)
- arithmetic (add, sub, multiply, divide)
- bitwise operations (and, or, not)
- bitwise shift (left/right)
- comparison (between registers, numerical)
- jump (conditional and unconditional)
- call (unconditional)
- stack manipulation (push and pop)
- inputs and outputs (using registers)
- prints (Denoted with " ")
- labels (Denoted with ' ')
- comments (Denoted with " ")

One of the main features of StartASM is its abstraction to reduce the barrier of entry. This includes:
- Datatypes (ints, floats, bools, chars, addresses) that eliminate the need to interpret hex directly
- A dynamic type system with enforced type safety while allowing users to 'cast' data (interpret the byte sequence differently) 
- Simplified memory model aligned to 32 bits for every datatype
- Simplified I/O and terminal


## Syntax
StartASM is designed to be as close to plain English as possible. The syntax trades away traditional opcode mnemonics used in most ASMs in favor of readable syntax closer to high-level languages. This includes the use of full-word instructions, transitional conjunctions to demonstrate operand relationships, and clearly denoted register (`r0`-`r9`), memory (`m<0>`-`m<999999999>`) and instruction memory (`i[0]`-`i[999999999]`) operands. For example:

`move r1 to r2`
- Denotes the instruction first `move`
- States implicit source operand `r1`
- Provides a transitional conjunction to show where data is flowing `to`
- States explicitly destination operand `r2`

For a more complex example:

`shift right r1 with r2`
- Denotes the instruction first `shift`
- Provides direction after the base instruction `right`
- States implicitly self operand `r1`
- Provides transitional conjunction to show the relationship as an attribute of instruction `by`
- States explicitly the attribute operand `r2`

StartASM will enforce type safety as checked at runtime time. For example:

`add r1 with r2 to r3`

- Will throw an exception if r1 and r2 are not tracked to be of the same datatype
- Will cast r3 into the data type of the result
- Will perform the correct addition operation for the given datatypes (integer arithmetic, floating point arithmetic, etc.)

In summary:
- Transitional conjunctions are used to show operand relationships (except for source/self operands, which are implicit for a natural language feel). e.g. `from`, `to`, `with`, `by`. Conditionals are denoted by `if`
- Operands are stated explicitly and numerically. e.g. `r0`, `m<39>`
- Base instructions are stated in full, with all instruction conditions also stated in full (no mnemonics). e.g. `move`, `shift left`
- Types are stated and enforced but flexible in allowing reinterpretations of the same byte value


## Usage
Download the repository and open it in a code editor. Run the StartASM executable, and install a C++ compiler that supports OpenMP (preferably g++). Create a text file with StartASM code and place it into the 'code' folder, then run the executable. StartASM uses the `.sasm` file extension. When running the executable and providing a filename, you can either include the `.sasm` extension or omit it, but don't append a different file extension (like .txt).

Here are all possible instruction combinations as of now:
- `move (register) to (register)`
- `load (memory/register) to (register)`
- `store (register) to (memory/register)`
- `create integer/float/boolean/character/memory/instruction (value) to (register)`
- `cast integer/float/boolean/character/memory/instruction (register)`
- `add (register) with (register) to (register)`
- `sub (register) with (register) to (register)`
- `multiply (register) with (register) to (register)`
- `divide (register) with (register) to (register)`
- `or (register) with (register)`
- `and (register) with (register)`
- `not (register)`
- `shift left/right (register) by (register)`
- `compare (register) with (register)`
- `jump if (unconditional/greater/less/equal/zero/unequal/nonzero) to (instruction/label/register)`
- `call to (instruction/label/register)`
- `push (register)`
- `pop to (register)`
- `return`
- `stop`
- `input integer/float/boolean/character to (register)`
- `output (register)`
- `print ("comment"/newline)`
- `label '(label)'`
- `comment "(comment)"`

Where: 
- Registers are `r0`-`r9`
- Memory addresses are `m<0>` to `m<999999999>`, with each value being 4 bytes in program memory
- Instruction addresses are `i[0] to i[999999999]`, with each value being a 4 byte instruction
- Labels are any word within single quotes *'likeThis'*
- Comments are any string within double quotes *"Like this"*

StartASM interacts directly in the terminal akin to higher-level languages, no syscalls or kernel interactions necessary. Methods `input` and `output` work strictly with dynamic data stored in registers, whereas there's a seperate `print` statement to allow easy user prompts and debugging. All outputs and inputs are on the same line unless expressly preceded by a `print newline`.

StartASM also supports manipulating instruction and memory addresses, alowing basic pointer functionality and operations. This can be useful for creating contiguous data structures (such as arrays) or jump tables. Thus, address instructions such as `load`, `store`, `jump` and `call` allow both immediates and registers holding valid addresses (though this will be checked for type safety).

Check the 'code' folder for examples. Each code file contains a comment explaning it's purpose. If you would like to stress test the compiler, you can run the included `StressTest.py` script to generate a random stress test file `StressTest.sasm`. Be careful about the size you decide for the stress test as millions of lines of code can easily exhaust RAM and system resources!

```
comment "Let's set a constant 21 and ask the user for their age"
create integer 21 to r1
print "How long till you're 21? Let's find out!"
print newline
print "Enter your age: "
input integer to r2
print newline
compare r2 with r1
jump if equal to 'is21'
jump if greater to 'past21'
jump if less to 'not21'
stop

comment "Case - if the user is exactly 21"
label 'is21'
print "You're just barely 21! Yay!"
stop

comment "Case - if the user is past 21, we'll print their age back to them"
label 'past21'
print "Wow, you're "
output r2
print " and you're using this calculator? Scram!"
stop

comment "Case - if the user is yet to be 21, we'll output the number of years left"
label 'not21'
sub r1 with r2 to r3
print " *sigh* you still have "
output r3
print " years to go till you're 21."
stop
```
The above program demonstrates numerous features of StartASM, including I/O, initializing values, arithmetic operations, and conditional jumps.

Here is a seperate example showing a way to input a string given individual character inputs. It terminates once the ~ is detected:
```
comment "First let's get the input string by looping until ~ is detected"
comment "First create three memory addresses - our start index, our current index and our increment amount"
create memory m<0> to r0
create memory m<0> to r1
create memory m<1> to r2
comment "Next create our terminating character"
create character ~ to r3

comment "Prompt the user for input"
print "Enter your string until you terminate with the ~ key"
print newline

comment "Loop the input until we detect the ~ key"
label 'inputLoop'
input character to r4
compare r4 with r3
jump if equal to 'outputLoop'
store r4 to r1
add r1 with r2 to r1
jump if unconditional to 'inputLoop'

comment "Now let's print the string back to them starting from our start index until it hit's our 'current' (or end) index"
label 'outputLoop'
load r0 to r4
output r4
add r0 with r2 to r0
compare r0 with r1
jump if greater to 'terminateProgram'
jump if unconditional to 'outputLoop'

label 'terminateProgram'
stop
```

The compiler will also return syntax and symbol/scope error messages that show the excepted line, token, and what the compiler expected, if applicable. For example, submitting this invalid code:
``` 
shift right r2 with r1
junk r1
move m<1> to r6
```

Will return the following errors:
```
Invalid syntax at line 1: 'shift right r2 with r1'
Unknown conjunction 'with'. Expected 'by'

Invalid syntax at line 2: 'junk r1'
Unknown instruction 'junk'

Invalid syntax at line 3: 'move m<1> to r6'
Unknown source 'm<1>'. Expected register r0-r9
```

## Technologies
StartASM is, as of now, fully developed in C++. The intent is for the compiler and runtime environment to be built using C++, while the front end will be built using Electron and node.js. This project also uses OpenMP multithreading to improve performance.

## Performance
Here are some test runs on the compiler with various example files:

Ten thousand Lines:
Time taken: 0.134222 seconds

One Million Lines:
Time taken: 13.1573 seconds

Ten Million Lines:
Time taken: 133.811 seconds

## Progress
**Please note that StartASM is still in early development, so many critical features have yet to be implemented. This is a personal project by a student and, as such, is being developed incrementally.**

*Last updated: December 2, 2023*

**State:** StartASM currently consists of a compiler that validates the grammar, including syntax, symbols and semantics. Currently missing features of the compiler include scope checking and code generation.

**Progress:** Currently working on implementing scope checking and code generation.

**Current Goal:** Finish the compiler by the end of the year.


## Last Major Milestone
**From December 1, 2023**

*Reworked Lexer, Parser and AST | Value Creation, Type Systems and I/O | Performance Overhaul*

The entire StartASM compiler was fully upended, moving from regex-based parsing logic to recursive descent. I've implemented numerous changes, including abolishing the InstructionSet.cpp class with a new Parser.cpp class that more closely aligns with industry-standard practices. Created a new ParseTree class to store syntactic information and completely reworked the AST class to represent StartASM syntax better. I also instituted a significant overhaul to the lexer, which now categorizes tokens based on enum constants. Furthermore, I've removed problematic OpenMP parallelized sections (particularly those with unstable access/writing in non-thread-safe containers) while ensuring that currently parallelized segments are thread-safe. SIGNIFICANT performance increases can be found (up to 390x); here is a sample:

Regex (single threaded):
Time taken: 13.000235 seconds

Regex (multi-threaded):
Time taken: 3.149544 seconds

Recursive descent:
Time taken: 0.134222 seconds

[For 10000 lines of incorrect code]

Significant README updates and the introduction of major features, including the "create", "cast", "output", "input" and "print" methods, have been added. StartASM now contains critical features such as the creation of constant values, a fleshed-out type system, and I/O mechanisms. This is one of the most significant milestones in StartASM's development.

## Contact
If you want to contact me about this project, feel free to send an email to tahsinkalkie[at]gmail[dot]com
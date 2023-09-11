# StartASM
A beginner-friendly assembly language and emulator for learning assembly programming.


## Example Syntax (StartASM Simplified)
move r1 to r2
move 0x4948 to r2
move 83 to r1 if zero

load 0x4949 to r2

subtract r1 and r2 to r3
add r2 and r1 to r1
multiply signed r1 and r2 to r3
divide unsigned r3 and r2 to r2

compare r1 and r2
test r1 and r2

jump to 0x49483 if greater
jump to 0x4948

push from r1
pop to r2

xor r2 and r1 to r1
and 774 and r2 to r3
not r1 to r1

shift 8 left r1 to r2
shift 2 right arithmetically 0x3848 to r1
shift 0 right logically 94 to r2

skip
return


## Example Syntax (StartASM Traditional)
mov r1, r2
mov 0x4948, r2
movz 83, r1

ld 0x4949, r2

sub r1, r2, r3
add r2, r1
muls r1, r2, r3
divu r3, r2

jmpg 0x49843
jmp 0x4948

push r1
pop r2
xor r2, r1
and 774, r2, r3
not r1
sl 8, r1, r2
sra 2, 0x3848, r1
srl 0, 94, r2

skp 
ret

## Future Syntax Extensions
set on carry flag
set off zero flag if overflowed 


## Design Philosophy
The design philosophy aims to maintain as many core assembly programming practices as possible while reducing headaches and providing a more beginner-friendly option (Think of this as the Python of ASM, where traditional x86/ARM might be C++). SS is designed to be readable like plain English, using specific conjunctive keywords to illustrate the flow of data and operations performed. TS is designed to be a shortened version of SS, where the same structure remains but is streamlined to allow for faster, more efficient coding. TS is intended to be used once the user is comfortable with SS, and again, ADA allows the conversion between TS and SS syntax interchangeably in an ASM file. This allows the user to use TS syntax on instructions they are very familiar with or may use often (mov, add, ld etc.) while allowing them to use SS syntax for unfamiliar/cumbersome instructions.

SS requires the user to provide two arguments and a destination (e.g. add r1 and r2 to r2) whereas TS allows the user to only provide two arguments if the intent is to save it in the destination (like existing ASMs). This is because TS's goal is to accelerate programming and be more akin to traditional ASMs, while SS is to assist learning and reinforce exactly what the computer is doing. ADA automatically converts 3 argument instructions in SS that save to the destination into two argument instructions when converting SS code to TS. TS still allows 3 arguments saving to the same destination (e.g. muls r1, r2, r2), which will be converted by ADA respectfully.

SS's use of "to" and "from" was a deliberate choice to make clear where the computer is reading and writing. A "to" indicates a write (e.g. "move r1 to r2" indicates that r2 will be written with the value of r1) and "from" indicates a read (e.g. "push from r2" indicates that the value of r2 will be read and pushed to the stack).

The use of "and" and "if" are also there to provide context themselves. "and" indicates a comparison of some sort between two values, while "if" represents a condition. It's important to clarify that these only cover conjunction keywords, as there is also bitwise and (although I don't believe there should be any mix-up).

Additionally, TS is designed to shorten the exact sequence of words used in SS to only a few characters. While some may not follow traditional norms (e.g. using srl instead of sar like x86), this should in theory make it easier for someone to extrapolate most instructions in TS from SS without having to look up documentation. 

There are twenty general purpose registers + a return register that are available to use (r1, r2, r3...r20 + rReturn), 10 argument registers (a1, a2, a3....a10; like x86 %rax, %rbx...), and two pointer registers (pInstruct and pStack; like x86 %rip, %rsp). There is also an additional register to store temporary values "rTemp", but this is generally meant for use by ADA when converting 3 argument instructions into two arguments (The user can access it, but it is not recommended as its value may arbitrarily change in program-sequence). Note that the letter in front is to denote what the purpose of that register is, with 'r' being general purpose, 'a' being an argument register, and 'p' being a pointer register. This was to reduce the confusion of x86 registers I faced being seemingly random and nonsensical. While the ISA aims to provide clarity about the intended purpose of each register, the user is free to use them as they see fit (Just like a mainstream ASM, as I believe this is an important skill that should be developed when learning assembly).

Regarding immediate, unlike mainstream ISAs, StartASM allows the user to use both decimal and hex. ADA automatically converts decimal values into hexadecimal before machine encoding and hexadecimal immediates are denoted by the 0x prefix (as is the norm).



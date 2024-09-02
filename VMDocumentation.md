# RVM Documentation

This is the technical specification for the RVM Virtual Machine.

## RVM Architecture

RVM is a stack machine, all computations are performed on a stack. The stack is composed of 64-bit units, with no type, type is expressed through type parameters on the instructions themselves (see bellow).

The core of execution is the function, a consecutive segment of bytecode containing instructions. Functions have a stack frame, which is the region of the stack that belongs to the function, and also a local frame, which are the local variables of the function.

### Locals

Functions may allocate space for local variables (arguments are local variables that are pre-allocated by the caller). Locals are 64-bit wide, with no type, and are indexed from 0, with the local 0 being the first local allocated (or first parameter if it was passed).


### Functions

Functions are called with an specific number of arguments, specified within the `call` or `callindirect` instructions. They may also return an specific number of arguments, specified within the `ret` instruction.

## Bytecode

### Instruction stream

The RVM uses a combined instruction and data stream for its bytecode, certain instructions may use parts of the bytecode as data. The stream is divided into 64 bit instruction units, and everything in it is aligned to those units. Strings may use multiple instruction units, but must always be terminated by a null byte. 

### Global Data Units

The bytecode is divided into segments called Global Data Units (GDUs). Each GDU has an identifier and its instruction/data stream. Both functions and global variables reside as GDUs, and the VM has specific instructions to operate upon GDUs. GDUs are contiguous in memory.

### Instruction format
Instructions are 64 bits wide, and use the following format:

| Byte | Usage 
| ---- | ---- |
| 0 | 8-bit opcode
| 1 | 8-bit type parameter 0.
| 2 | 8-bit type parameter 1.
| 3 | 8-bit type parameter 2.
| 4 | 32-bit signed integer embeded data.
| 5 | ...
| 6 | ...
| 7 | ...


### Instruction set

#### Parameter format
Note: Unspecified instruction parameters are ignored by the VM.
- `{T0, T1, T2}` denote type parameters.
- `[name]` denotes embeded 32-bit integer data.
- `!name` specifies that the next 64-bit instruction unit will be used as data.
- `$name` specifies that the next instruction(s) will be interpreted as a string, until a null character is encountered. Padding may be necessary to align the next instructions to the 64 bit boundary if the string has a length that is not a multiple of 8.

#### Type parameters
Some instructions take type parameters as input. They are the following:
| Type | ID | Description |
| -- | -- | -- |
| I8 | 1 | 8 bit signed integer. |
| I16 | 2 | 16-bit signed integer. |
| I32 | 3 | 32-bit signed integer. |
| I64 | 4 | 64-bit signed integer. |
| F32 | 5 | 32-bit single precision floating point number. |
| F64 | 6 | 64-bit double precision floating point number. |
| PTR | 7 | 64-bit pointer. |

#### Instruction list

| Opcode (hex) | Mnemonic | Type parameters | Embeded i32 data | Instruction stream data | Usage
| -- | -- | -- | -- | -- | -- |
| 00 | nop | | | |  No operation, does nothing.
| 01 | halt | | | | Stops the virtual machine.
| 02 | load | | `[index]` | | Loads the local at `index` onto the stack.
| 03 | store | | `[index]` | | Pops a value from the stack and stores into the local at `index`.
| 04 | loadconst | | | `!data` | Loads `data` onto the the stack.
| 05 | storeconst | | `[index]` | `!data` | Stores `data` into the local at `index`.
| 06 | convert | `{From, To}` | | | Pops a value from the stack, and applies a type conversion from type `From` into type `To`, then pushes the result back.
| 07 | add | `{Type}` | | | Pops `b`, then pops `a`, and finally pushes the result of `a + b` onto the stack, assuming `a` and `b` to be of type `Type`.
| 08 | sub | `{Type}` | | | Pops `b`, then pops `a`, and finally pushes the result of `a - b` onto the stack, assuming `a` and `b` to be of type `Type`.
| 09 | mul | `{Type}` | | | Pops `b`, then pops `a`, and finally pushes the result of `a * b` onto the stack, assuming `a` and `b` to be of type `Type`.
| 0A | div | `{Type}` | | | Pops `b`, then pops `a`, and finally pushes the result of `a / b` onto the stack, assuming `a` and `b` to be of type `Type`.
| 0B | land | | | | Pops 2 values and performs a logical AND operation. Places result back on the stack.
| 0C | lor | | | | Pops 2 values and performs a logical OR operation. Places result back on the stack.
| 0D | lnot | | | | Pops a value and performs a logical NOT operation. Places result back on the stack.
| 0E | gt | `{Type}`| | | Pops `b`, then pops `a`, and finally pushes the result of the comparison `a > b`, assuming `a` and `b` to be of type `Type`.
| 0F | geq | `{Type}`| | | Pops `b`, then pops `a`, and finally pushes the result of the comparison `a >= b`, assuming `a` and `b` to be of type `Type`.
| 10 | lt | `{Type}`| | | Pops `b`, then pops `a`, and finally pushes the result of the comparison `a < b`, assuming `a` and `b` to be of type `Type`.
| 11 | leq | `{Type}`| | | Pops `b`, then pops `a`, and finally pushes the result of the comparison `a < b`, assuming `a` and `b` to be of type `Type`.
| 12 | eq | `{Type}`| | | Pops `b`, then pops `a`, and finally pushes the result of the comparison `a == b`, assuming `a` and `b` to be of type `Type`.
| 13 | noteq | `{Type}`| | | Pops `b`, then pops `a`, and finally pushes the result of the comparison `a != b`, assuming `a` and `b` to be of type `Type`.
| 14 | band | | | | Pops `b`, then pops `a`, and finally pushes the result of performing a bitwise AND operation.
| 15 | bor | | | | Pops `b`, then pops `a`, and finally pushes the result of performing a bitwise OR operation.
| 16 | bxor | | | | Pops `b`, then pops `a`, and finally pushes the result of performing a bitwise XOR operation.
| 17 | bnot | | | | Pops a value, and pushes the result of performing a bitwise negation operation.
| 18 | lshift | | | | Pops `b`, then pops `a`, and finally pushes the result of the operation `a << (b % 64)`.
| 19 | rshift | | | | Pops `b`, then pops `a`, and finally pushes the result of the operation `a >> (b % 64)`.
| 1A | jmp | | `[Offset]` | | Jumps execution to the instruction located by `Offset` (measured in 64-bit units).
| 1B | jmpif | | `[Offset]` | | Pops a value `a`, then jumps execution to the instruction located by `Offset` (measured in 64-bit units) only if `a` is a `true` value.
| 1C | createlocals | | `[Number]` | | Allocates `Number` locals for the current function frame.
| 1D | call | | `[Argnum]` | `$FunctionName`| Calls the function given by `FunctionName`, and allocates the top `Argnum` values in the stack as locals for the callee function frame, the first argument must be at the top of the stack, so the callee locals are reversed to how they were in the stack originally. 
| 1E | ret | | `[Num]` | | Returns execution to the caller function, passing the top `Num` values of the stack to the caller.
| 1F | callindirect | | `[Argnum]` | | Same as `call`, but after popping the arguments from the stack, it pops an additional parameter `ptr`, that points to the function being called.
| 20 | getglobal | | | `$GlobalName` | Pushes onto the stack the address of a global unit by name `GlobalName`.


### Executable format

RVM executables contain a number of GDUs represented in binary inside them.

For each GDU, the file will have in this order, without padding:
    - GDU header.
    - GDU content.

GDU Header format:

    - Length in bytes of the GDU name string, represented as a decimal number in ASCII characters.
    - `0xFF` separator.
    - GDU name.
    - Length in bytes of the GDU data/instruction content, represented as a decimal number in ASCII characters.
    - `0xFF` separator.


For example, if we have a GDU named 'Hello' and 72 bytes of data, the correct format for the GDU header would be (in hexadecimal):

`35 ff 48 65 6c 6c 6f 37 32 ff [...]`

Or, in a more readable (but not valid for executable file) form:

`5_Hello72_[...]`

After this, all GDUs (header and content) must be concatenated with **no padding between them**.
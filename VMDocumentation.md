# RVM Documentation

This is the technical specification for the RVM Virtual Machine.

## Bytecode

### Instruction format
Instructions are 64 bits wide, and use the following format:

| Byte | Usage 
| ---- | ---- |
| 0 | 8 bit opcode
| 1 | 8 bit type parameter 0
| 2 | 8 bit type parameter 1
| 3 | 8 bit type parameter 2
| 4 | 32 bit signed integer embeded data
| 5 | ...
| 6 | ...
| 7 | ...


### Instruction set

#### Parameter format
Note: Unspecified instruction parameters are ignored by the VM.
- `{T0, T1, T2}` denote type parameters.
- `[name]` denotes embeded 32 bit integer data.
- `!name` specifies that the next 64 bit instruction unit will be used as data.
- `$name` specifies that the next instruction(s) will be interpreted as a string, until a null character is encountered. Padding may be necessary to align the next instructions to the 64 bit boundary if the string has a length that is not a multiple of 8.

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
| 1A | jmp | | `[Offset]` | | Jumps execution to the instruction located by `Offset`.
| 1B | jmpif | | `[Offset]` | | Pops a value `a`, then jumps execution to the instruction located by `Offset` only if `a` is a `true` value.
| 1C | createlocals | | `[Number]` | | Allocates `Number` locals for the current function frame.
| 1D | call | | `[Argnum]` | `$FunctionName`| Calls the function given by `FunctionName`, and allocates the top `Argnum` values in the stack as locals for the callee function frame. 
| 1E | ret | | `[Num]` | | Returns execution to the caller function, passing the top `Num` values of the stack to the caller.
| 1F | callindirect | | `[Argnum]` | | Pops a value `ptr` from the stack, then calls the function located at `ptr` with `Argnum` arguments.
| 20 | getglobal | | | `$GlobalName` | Pushes onto the stack the address of a global unit by name `GlobalName`.

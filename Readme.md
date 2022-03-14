# Sparrow<a name="top"></a>

It's like [Porth](https://gitlab.com/tsoding/porth/-/tree/master/), which is like [Forth](https://www.forth.com/forth/) but written in Python, but written in C++. But I don't actually know for sure since I never programmed in Porth or Forth, I only heard that they are some sort of stack-based programming language. Sparrow is also stack-based programming language. Which makes it just like Porth, just like Forth, am I rite?

## Background

This was literlly fueled by a high I was chasing at that momemt. I wanted to be better at Programming specifically low level programming.

Sparrow is:

- [x] A [stack based](https://en.wikipedia.org/wiki/Stack-oriented_programming) programming language
    I do recommend you to read the link above.
- [x] [Turing complete](https://en.wikipedia.org/wiki/Turing_completeness)
- [x] Compileable to a dynamic executable linked with the C RunTime.
- [x] Totally awesome and annoying language.

## General Information

File Suffix: .spar
Assembler: Nasm  (In Repo)
Linker: GoLink (In Repo)

## Something to Understand Before Checking Sparrow out

Sparrow uses the [system](https://en.cppreference.com/w/cpp/utility/program/system) function within C++ to run external commands on your computer. These commands can be affected by user input, so running corth has the potential to run any command on your system if you tell it to, including malicious ones. Be sure to check and double check any commands you see that use the `-a` or `-l` compiler options, as these tell Sparrow to run a different command than the default. Every command run by Sparrow is echoed to the standard out with a '[CMD]' prefix.
Be careful when running commands that you don't want to run, as they can be used to run malicious code.

## Keywords  <a name="Sparrow-keywords"></a>

| Keyword                | Notation                    | Description                                                            |
|:----------------------:|:---------------------------:|:-----------------------------------------------------------------------|
|[if](#kw-if)            | `[a] -> []`                 | Jump to `else`/`endif` only if popped value is equal to zero.          |
|[else](#kw-else)        | `[] -> []`                  | Inside this block is what will be ran if `if` condition is false.      |
|[endif](#kw-endif)      | `[] -> []`                  | Required block-ending-symbol for `if` keyword.                         |
|[do](#kw-do)            | `[a] -> []`                 | Jumps just past `endwhile` if popped value is zero.                    |
|[while](#kw-while)      | `[] -> []`                  | Generates a label for `endwhile` to jump to.                           |
|[endwhile](#kw-endwhile)| `[] -> []`                  | Generates a label for `do` to jump to upon false condition.            |
|[dup](#kw-dup)          | `[a] -> [a][a]`             | Duplicate top of stack.                                                |
|[twodup](#kw-twodup)    | `[a][b] -> [a][b][a][b]`    | Duplicate two items on top of stack.                                   |
|[drop](#kw-drop)        | `[a] -> []`                 | Deletes the top-most item off the stack.                               |
|[swap](#kw-swap)        | `[a][b] -> [b][a]`          | Pushes two popped values back in reverse order.                        |
|[over](#kw-over)        | `[a][b] -> [a][b][a]`       | Pushes the stack item below the top on to the top.                     |
|[dump](#kw-dump)        | `[a] -> []`                 | Equivalent to [#](#op-dump) operator.                                  |
|[dump_c](#kw-dump-c)    | `[a] -> []`                 | Pops a value off the stack, then prints it formatted as a char.        |
|[dump_s](#kw-dump-s)    | `[a] -> []`                 | Pops a value off the stack, then prints it formatted as a string.      |
|[mem](#kw-mem)          | `[] -> [addr]`              | Pushes the address of the usable memory in Sparrow.                      |
|[store<x>](#kw-store)   | `[addr][a] -> []`           | Stores the popped value at popped memory address.                      |
|[load<x>](#kw-load)     | `[addr] -> [a]`             | Pushes the value read at popped address on to the stack.               |
|[shl](#kw-shl)          | `[a][b] -> [a << b]`        | Equivalent to [<<](#op-bit-shl) operator.                              |
|[shr](#kw-shr)          | `[a][b] -> [a >> b]`        | Equivalent to [>>](#op-bit-shr) operator.                              |
|[and](#kw-and)          | `[a][b] -> [a && b]`        | Equivalent to [&&](#op-bit-and) operator.                              |
|[or](#kw-or)            | `[a][b] -> [a \|\| b]`      | Equivalent to [\|\|](#op-bit-or) operator.                             |
|[mod](#kw-mod)          | `[a][b] -> [a % b]`         | Equivalent to [%](#op-modulo) operator.                                |
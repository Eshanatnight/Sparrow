# Sparrow<a name="top"></a>

It's like [Porth](https://gitlab.com/tsoding/porth/-/tree/master/), which is like [Forth](https://www.forth.com/forth/) but written in Python, but written in C++. But I don't actually know for sure since I never programmed in Porth or Forth, I only heard that they are some sort of stack-based programming language. Sparrow is also stack-based programming language. We going full circle at this point!

## Background

This was literlly fueled by a high I was chasing at that momemt. I wanted to be better at Programming specifically low level programming.

Sparrow is:

- [x] A [stack based](https://en.wikipedia.org/wiki/Stack-oriented_programming) programming language
    I do recommend you to read the link above.
- [x] [Turing complete](https://en.wikipedia.org/wiki/Turing_completeness)(see [rule 110 example](./examples/rule110.spar))
- [x] Compileable to a dynamic executable linked with the C RunTime.
- [x] Totally awesome and annoying language.

## General Information

File Suffix: .spar
Assembler: Nasm  (In Repo)
Linker: GoLink (In Repo)

## Something to Understand Before Checking Sparrow out

Sparrow uses the [system](https://en.cppreference.com/w/cpp/utility/program/system) function within C++       \
to run external commands on your computer. These commands can be affected by user input, so running sparrow   \
has the potential to run any command on your system if you tell it to, including malicious ones. Be sure to   \
check and double check any commands you see that use the `-a` or `-l` compiler options, as these tell Sparrow \
to run a different command than the default. Every command run by Sparrow is echoed to the standard out       \
with a '[CMD]' prefix. Be careful when running commands that you don't want to run, as they can be used to    \
run malicious code.

## How to write in Sparrow <a name="how-to-write-sparrow"></a>
Sparrow, like Porth (like Forth), is a stack based language. We going on full circle boys!  \
This means that in order to do any operation, a value must be pushed onto the stack.        \
Think of this as a literal stack of objects.                                                \
Likely, the operation will take it's arguments from the top of the stack, or `pop` from the stack, and sometimes it will return a value or two back onto it, which is called `pushing` on to the stack.

## Table of Contents

This is where we go get complicated!

- [How to Write in Sparrow](#how-to-write-sparrow)
  - [A Simple Example](#how-to-a-simple-example)
  - [Hello, World](#how-to-hello-world)
  - [Conditional Branching](#how-to-conditional-branching)
  - [Loops](#how-to-loops)
  - [Complications](#how-to-complications)
- [Definitions](#sparrow-definitions)
  - [Stack Notation](#sparrow-stack-notation)
  - [Operators](#sparrow-operators)
  - [Keywords](#sparrow-keywords)
- [How to use Sparrow](#how-to-use-sparrow)
- [How to compile Sparrow from source](#how-to-build-sparrow)

---

### A Simple Example <a name="how-to-a-simple-example"></a>
Take a look at this example in Sparrow:
```
34 35 + #
```
It is a basic Sparrow program that will add two numbers together, and then display the sum to the console.

Expected output for the above program when run: \
<samp>69</samp>

Let's break down how it works piece-by-piece:
| Step | Code | Description                                                                                                                     |
|------|------|---------------------------------------------------------------------------------------------------------------------------------|
|    1 | `34` | A value is pushed on to the stack, meaning stacked on top.                                                                      |
|    2 | `35` | Another value is pushed on to the stack.                                                                                        |
|    3 |  `+` | The `+` symbol will pop the two most-recent values off the stack, add them, then push the sum on to the stack.                  |
|    4 |  `#` | The `#` symbol will dump from the stack, aka pop a value off and then print it to the console.                                  |

Stack breakdown by step:
```
0. []
1. [34]
2. [34][35]
3. [69]
4. []
```

Related:
- Operator: [+](#op-add)
- Operator: [#](#op-dump)


#### Best practices in Sparrow indicate that the stack should be empty by the end of the program.

[To Top](#top)

---

### Hello, World! <a name="how-to-hello-world"></a>
String literals are now supported in Sparrow; this makes printing a string to the console as simple as dumping with the `_s` suffix indicating a string format should be used.

```
"Hello, World!\n" dump_s
```

As you can see, Sparrow supports some escaped characters within strings.
They are `\n`, `\r`, and `\t`, to be exact.

You can also dump single characters using an ASCII code like so:
```
10 dump_c
```

Expected output:
```
Hello, World!


```

Related:
- Keyword: [dump](#kw-dump)

[To Top](#top)

---

### Conditional Branching <a name="how-to-conditional-branching"></a>
Let's look at a slightly more complicated example program:
```
500 80 - 420 = if
  69 #
else
  420 #
endif
```
This program should first evaulate `500 - 80`, then compare if that sum is equal to `420`. If true, print <samp>69</samp>. If false, print <samp>420</samp>. \
Expected output: \
<samp>69</samp>

Let's break down how it works piece-by-piece:
| Step | Code | Description                                                                                                                     |
|------|------|---------------------------------------------------------------------------------------------------------------------------------|
|    1 |`500 80 -` | Push two values on to the stack, subtract them, then push the result on to the stack.                                      |
|    2 |`420` | Push `420` onto the stack                                                                                                       |
|    3 |  `=` | Use the equality comparison operator; it pops two values off the stack, compares them, then pushes back a `1` if they are equal or a `0` if they aren't.|
|    4 | `if` | Pop the condition just pushed onto the stack, jump to `else`/`endif` if it is false, otherwise, like in this case, fall-through to next instruction.|
|    5 |`69 #`| Push a value onto the stack, then dump it to console output.                                                                    |
|    6 |`else`| Label to jump to if `if` condition is false. Label to jump over to `endif` if `if` condition is true.                           |
|    7 |`420 #`| This would push a value onto the stack, then dump it to console output, however it will be jumped over due to the `if` condition evaluating to true.|
|    8 |`endif`| Label to jump to if `if` condition is false and no `else` label is present or if `if` condition is true and an `else` label is present.|

[To Top](#top)

---

### Loops <a name="how-to-loops"></a>
Sparrow now fully supports loops! Check out the following example:
```
1
while dup 30 <= do
  dup dump    // Print loop counter without destroying it
  10  dump_c  // Print a newline character
  1 +         // Increment loop counter
endwhile
```

This program will print every whole number from `1` to `30`, each being on a new line.

Let's break down how it works:
| Step | Code | Description                                                                                                                     |
|------|-------------|---------------------------------------------------------------------------------------------------------------------------------|
|    1 | `1`         | Push a one onto the stack to initialize the loop counter.                                                                       |
|    2 | `while`     | Generate an address to jump to upon reaching endwhile.                                                                          |
|    3 | `dup 30 <=` | Push a boolean condition on the stack comparing whether the last item on the stack (duplicated) is less than or equal to `30`.  |
|    4 | `do`        | Pop the condition just pushed onto the stack, jump just past `endwhile` (step 7) if it is zero, otherwise, like in this case, fall-through to next instruction.|
|    5 | `dup #`     | Duplicate the top-most value onto the stack, then dump the duplicate to console output. This prints the current loop counter, as that is what's on the stack.|
|    6 | `1 +`       | Add 1 to top-most value on the stack. This increments the loop counter.                                                         |
|    7 | `endwhile`  | Upon reaching, jump back to `while` (step 2) and continue execution from there.                                                 |

It is known that this program will trigger a stack validator warning, telling us that the stack at the end of the program is not empty. \
With programs as simple as these, it's okay to do, however best practices indicate that the stack should be empty by the end of the program.

[To Top](#top)

---

### Complications <a name="how-to-complications"></a>
For a more complex example, see [rule 110](./examples/rule110.spar)

Related:
- Keyword: [mem](#kw-mem)
- Keyword: [load](#kw-load)
- Keyword: [store](#kw-store)

[To Top](#top)

---

## Definitions: <a name="sparrow-definitions"></a>

Sections:
- [Stack Notation](#sparrow-stack-notation)
- [Operators](#sparrow-operators)
- [Keywords](#sparrow-keywords)

#### Stack Notation <a name="sparrow-stack-notation"></a>
The stack notation of the Sparrow documentation is quite simple. \
A stack item is surrounded in square brackets. \
The arrow '->' indicates a before and after view of the stack.

Example of a notation with no effect on the stack: \
`[] -> []`

Example of a notation that pushes a value, `a`, on to the stack: \
`[] -> [a]`

Example of a notation that pops two values and pushes the sum of them on to the stack: \
`[a][b] -> [a + b]`

[To Top](#top)

#### Operators <a name="sparrow-operators"></a>
An operator will take value(s) from the stack and optionally push some back on.
The amount of values removed/added from/to the stack by a given operator can be seen by the stack notation in the following table.
For further information on the operator, click on the link to be directed to a more verbose definition of the operator, down below.

| Operator          | Stack Notation        | Description                                                                        |
|:-----------------:|:---------------------:|:-----------------------------------------------------------------------------------|
| [#](#op-dump)     | `[a] -> []`           | Print value on top of stack formatted as unsigned integer.                         |
| [+](#op-add)      | `[a][b] -> [a + b]`   | Sum two elements on top of stack.                                                  |
| [-](#op-subtract) | `[a][b] -> [a - b]`   | Subtract two elements on top of stack.                                             |
| [*](#op-multiply) | `[a][b] -> [a * b]`   | Multiply two elements on top of stack.                                             |
| [/](#op-divide)   | `[a][b] -> [a / b]`   | Divide two elements on top of stack, push quotient.                                |
| [%](#op-modulo)   | `[a][b] -> [a % b]`   | Divide two elements on top of stack, push remainder.                               |
| [=](#op-cmp-eq)   | `[a][b] -> [a == b]`  | Compare if equal top two elements of stack.                                        |
| [>](#op-cmp-gt)   | `[a][b] -> [a > b]`   | Compare greater than top two elements of stack.                                    |
| [<](#op-cmp-lt)   | `[a][b] -> [a < b]`   | Compare less than top two elements of stack.                                       |
| [>=](#op-cmp-ge)  | `[a][b] -> [a >= b]`  | Compare greater than or equal top two elements of stack.                           |
| [<=](#op-cmp-le)  | `[a][b] -> [a <= b]`  | Compare less than or equal top two elements of stack.                              |
| [<<](#op-bit-shl) | `[a][b] -> [a << b]`  | Shift bits of `a` left by `b` bits.                                                |
| [>>](#op-bit-shr) | `[a][b] -> [a >> b]`  | Shift bits of `a` right by `b` bits.                                               |
| [&&](#op-bit-and) | `[a][b] -> [a && b]`  | Bitwise and on top two elements of stack.                                          |
| [\|\|](#op-bit-or)| `[a][b] -> [a \|\| b]`| Bitwise or on top two elements of stack.                                           |

#### '#' - DUMP <a name="op-dump"></a>
Humankind's best friend; pops a single value, `a`,  off the stack, then prints `a` to the console formatted as an unsigned integer.
For alternate formats, see related.

```
[a] -> []
```

Eqiuvalent
- [Keyword: dump](#kw-dump)

Related
- [dump keywords](#sparrow-keywords)

Example:
```
420 #
```
Stack Output:
```
[]
```

Standard Output:
```
420
```

[To Operators](#sparrow-operators)

---

#### '+' - ADD <a name="op-add"></a>
Pops two values, `a` and `b`, off of the stack, then pushes the sum of those values.

```
[a][b] -> [a + b]
```

Equivalent
- No equivalents

Related
- No related

Example:
```
34 35 +
```

Stack Output:
```
[69]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '-' - SUBTRACT <a name="op-subtract"></a>
Pops two values, `a` and `b`, off of the stack, then pushes the difference of those values.

```
[a][b] -> [a - b]
```

Equivalent
- No equivalents

Related
- No related

Example:
```
500 80 -
```

Stack Output:
```
[420]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '*' - MULTIPLY <a name="op-multiply"></a>
Pops two values, `a` and `b`, off of the stack, then pushes the product of those values.

```
[a][b] -> [a * b]
```

Equivalent
- No equivalents

Related
- No related

Example:
```
23 3 *
```

Stack Output:
```
[69]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '/' - DIVIDE <a name="op-divide"></a>
Pops two values, `a` and `b`, off of the stack, then pushes the product of those values.

```
[a][b] -> [a * b]
```

Equivalent
- No equivalents

Related
- No related

Example:
```
1260 3 /
```

Stack Output:
```
[420]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '%' - MODULO <a name="op-modulo"></a>
Pops two values, `a` and `b`, off of the stack, then pushes `a` modulo `b` on to the stack.

A modulus operation entails dividing, yet the result is the remainder, not the quotient.

```
[a][b] -> [a % b]
```

Equivalent
- [Keyword: mod](#kw-mod)

Related
- No related

Example:
```
18 15 %
```

Stack Output:
```
[3]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '=' - EQUAL <a name="op-cmp-eq"></a>
Pops two values off of the stack then pushes a `1` if the values are equal, or a `0` otherwise.

```
[a][b] -> [a == b]
```

Equivalent
- No equivalent

Related
- [Conditional Branching](#kw-if)

Example:
```
5 5 * 25 =
```

Stack Output:
```
[1]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '>' - GREATER-THAN <a name="op-cmp-gt"></a>
Pops two values, `a` and `b`, off of the stack then pushes a `1` if `a` is larger than `b`, or a `0` otherwise.

```
[a][b] -> [a > b]
```

Equivalent
- No equivalent

Related
- [Conditional Branching](#kw-if)

Example:
```
105 4 * 300 >
```

Stack Output:
```
[1]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '<' - LESS-THAN <a name="op-cmp-lt"></a>
Pops two values, `a` and `b`, off of the stack then pushes a `1` if `a` is smaller than `b`, or a `0` otherwise.

```
[a][b] -> [a < b]
```

Equivalent
- No equivalent

Related
- [Conditional Branching](#kw-if)

Example:
```
105 5 - 420 <
```

Stack Output:
```
[1]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '>=' - GREATER-THAN-OR-EQUAL <a name="op-cmp-ge"></a>
Pops two values, `a` and `b`, off of the stack then pushes a `1` if `a` is larger than or equal to `b`, or a `0` otherwise.

```
[a][b] -> [a >= b]
```

Equivalent
- No equivalent

Related
- [Conditional Branching](#kw-if)

Example:
```
105 4 * 420 >=
```

Stack Output:
```
[1]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '<=' - LESS-THAN-OR-EQUAL <a name="op-cmp-le"></a>
Pops two values, `a` and `b`, off of the stack then pushes a `1` if `a` is smaller than or equal to `b`, or a `0` otherwise.

```
[a][b] -> [a <= b]
```

Equivalent
- No equivalent

Related
- [Conditional Branching](#kw-if)

Example:
```
34 35 + 69 <=
```

Stack Output:
```
[1]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '<<' - BITWISE-SHIFT LEFT <a name="op-bit-shl"></a>
Pops two values, `a` and `b`, off of the stack then pushes bits of `a` shifted left by `b` amount of bits.

```
[a][b] -> [a << b]
```

Equivalent
- [Keyword: shl](#kw-shl)

Related
- No related

Example:
```
1 3 <<
```

Stack Output:
```
[8]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '>>' - BITWISE-SHIFT RIGHT <a name="op-bit-shr"></a>
Pops two values, `a` and `b`, off of the stack then pushes bits of `a` shifted right by `b` amount of bits.

```
[a][b] -> [a >> b]
```

Equivalent
- [Keyword: shr](#kw-shr)

Related
- No related

Example:
```
32 2 >>
```

Stack Output:
```
[8]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '&&' - AND <a name="op-bit-and"></a>
Pops two values, `a` and `b`, off of the stack, then pushes the [bitwise-and](https://en.wikipedia.org/wiki/Bitwise_operation#AND) of the two values.

```
[a][b] -> [a && b]
```

Equivalent
- [Keyword: and](#kw-and)

Related
- No related

Example:
```
9 3 &&
```

Stack Output:
```
[1]
```

Standard Output:
```
```

[To Operators](#sparrow-operators)

---

#### '||' - OR <a name="op-bit-or"></a>
Pops two values, `a` and `b`, off of the stack, then pushes the [bitwise-or](https://en.wikipedia.org/wiki/Bitwise_operation#OR) of the two values.

```
[a][b] -> [a || b]
```

Equivalent
- [Keyword: or](#kw-or)

Related
- No related

Example:
```
9 3 ||
```

Stack Output:
```
[11]
```

Standard Output:
```
```

[To Operators](#sparrow-operators) \
[To Definitions](#sparrow-definitions) \
[To Top](#top)

---

#### Keywords  <a name="sparrow-keywords"></a>
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
|[open_file](#kw-f-open) | `[path][mode] -> [ptr]`     | Push a pointer to a file at path on to the                     |
|[write_to_file](#kw-f-write)| `[str][1][len][ptr] -> []` | Write a string `str` to a file `ptr`.                               |
|[close_file](#kw-f-close)| `[ptr] -> []`              | Safely close an opened file.                                           |
|[length_s](#kw-strlen)    | `[str] -> [len]`          | Push the length of a string on to the stack.                           |
|[write](#kw-write)      | `[] -> [mode]`              | Push the `write` file mode constant on to the stack.                   |
|[append](#kw-append)      | `[] -> [mode]`            | Push the `append` file mode constant on to the stack.

---

#### 'if' - Conditional Branch <a name="kw-if"></a><a name="kw-else"></a><a name="kw-endif"></a>
The `if` keyword pops a value off the stack, then jumps to `endif` if the value is zero ,or `else` if it is present between `if`/`endif`.

`if`
```
[a] -> []
```

`else`, `endif`
```
[] -> []
```

Equivalent:
- No equivalent

Related:
- No related

Example:
```
1 1 = if
  420 #
else
  69 #
endif
```

Stack Output:
```
[]
```

Standard Output:
```
420
```

[To Keywords](#sparrow-keywords)

---

#### 'while' - Looping <a name="kw-while"></a><a name="kw-do"></a><a name="kw-endwhile"></a>
The `while` keyword generates a label for `endwhile` to jump to unconditionally. \
The `do` keyword is similar to the `if` keyword; if the item on the top of the stack is zero, it will jump just past `endwhile`, stopping the loop. \
The `endwhile` keyword is a necessary block ending symbol for the `while` keyword.

`do`
```
[a] -> []
```

`while`, `endwhile`
```
[] -> []
```

Equivalent:
- No equivalent

Related:
- No related

Example:
```
1            // initialize loop counter
while dup 5 <= do
  dup dump   // print loop counter
  10 dump_c  // print newline
  1 +        // increment loop counter
endwhile
drop         // drop loop counter from stack
```

Stack Output:
```
[]
```

Standard Output:
```
1
2
3
4
5
```

[To Keywords](#sparrow-keywords)

---

#### 'dup' - Stack Operation <a name="kw-dup"></a>
Duplicates the item at the top of the stack.

```
[a] -> [a][a]
```

Equivalent:
- No equivalent

Related:
- Keyword: [twodup](#kw-twodup)

Example:
```
69 dup
```

Stack Output:
```
[69][69]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'twodup' - Stack Operation <a name="kw-twodup"></a>
Duplicates the top two items of the stack.

```
[a][b] -> [a][b][a][b]
```

Equivalent:
- No equivalent

Related:
- Keyword: [dup](#kw-dup)

Example:
```
69 420
twodup
```

Stack Output:
```
[69][420][69][420]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'drop' - Stack Operation <a name="kw-drop"></a>
Deletes the item at the top of the stack, leaving no reference to it.
This is useful to shut up warnings from the stack validator (aka follow best practices managing your memory).

Most often seen used after [while](#kw-while) loops to delete the loop counter from the stack.

```
[a] -> []
```

Equivalent:
- No equivalent

Related:
- No related

Example:
```
420 drop
```

Stack Output:
```
[]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'swap' - Stack Operation <a name="kw-swap"></a>
Moves the top-most item and the item below it to each other's positions.

```
[a][b] -> [b][a]
```

Equivalent:
- No equivalent

Related:
- No related

Example:
```
80 500 swap
```

Stack Output:
```
[500][80]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'over' - Stack Operation <a name="kw-over"></a>
Push the item below the top of the stack on to the top of the stack, duplicating it.

```
[a][b] -> [a][b][a]
```

Equivalent:
- No equivalent

Related:
- No related

Example:
```
1 2 over
```

Stack Output:
```
[1][2][1]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'dump' - Tool <a name="kw-dump"></a><a name="kw-dump-c"></a><a name="kw-dump-s"></a>
Print the item at the top of the stack to the standard output. \
Internally, this uses the C RunTime `printf` method, so a format needs to be specified by using a specific keyword.

| Keyword       | Print Format     |
|---------------|------------------|
| `dump` or `#` | unsigned integer |
| `dump_c`      | character        |
| `dump_s`      | string           |


```
[a] -> []
```

Equivalent:
- Operator: [#](#op-dump)

Related:
- No related

Example:
```
// Print a number
69420 dump

// Print a newline (10 = ascii newline)
10 dump_c

// Print a string constant
"Dennis is a bastard man\n" dump_s

// Print a string from memory
//  I don't recommend constructing them from hand like
//    this, but it's really useful for building strings
//    within loops and not having to print each character.
//  i = string index
//        c = character ascii code
// <i>   <c>
mem      10   storeb  // <newline>
mem 1  + 82   storeb  // R
mem 2  + 101  storeb  // e
mem 3  + 103  storeb  // g
mem 4  + 103  storeb  // g
mem 5  + 105  storeb  // i
mem 6  + 101  storeb  // e
mem 7  + 32   storeb  // <space>
mem 8  + 87   storeb  // W
mem 9  + 97   storeb  // a
mem 10 + 116  storeb  // t
mem 11 + 116  storeb  // t
mem 12 + 115  storeb  // s
mem 13 + 10   storeb  // <newline>
mem 14 + 0    storeb  // null terminator
mem dump_s
```

Stack Output:
```
[]
```

Standard Output:
```
69420

Dennis is a bastard man

Reggie Watts
```

[To Keywords](#sparrow-keywords)

---

#### 'mem' - Memory Address <a name="kw-mem"></a>
Pushes the address of the memory allocated at run-time.

For now, this is hard-coded in the Sparrow executable to 720kb. Although that should be enough for everyone,
there will be a CCLI option in the future to specify the exact amount of bytes you would like to allocate.
###### Remember, it is up to you to not access invalid memory addresses.
To access any address within the memory, simply add the byte offset to the address, like so `mem <byte offset> +`. \
Next, use it with the memory access keywords that accept memory addresses as arguments (see related).

```
[] -> [addr]
```

Equivalent:
- No equivalent

Related:
- Keyword: [store](#kw-store)
- Keyword: [load](#kw-load)

Example:
```
mem
```

Stack Output:
```
[addr]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'store' - Memory Manipulation <a name="kw-store"></a>
Store a value at an address in the memory allocated during run-time (see [mem](#kw-mem)). \
This allows string-building, variables (albeit un-named ones), and as much as your mind can imagine.

A size of value to store at an address must be selected with the following format: \
`store<x>` \
Where `x` is any of the following:
- 'b' - byte        | 8  bits
- 'w' - word        | 16 bits
- 'd' - double word | 32 bits
- 'q' - quad word   | 64 bits

###### Remember, it is up to you to not access invalid memory addresses.

```
[addr][value] -> []
```

Equivalent:
- No equivalent

Related:
- Keyword: [mem](#kw-mem)
- Keyword: [load](#kw-load)

Example:
```
// Store a byte with value of 69 at mem[0]
mem 69 storeb

// Store a word with value of 69420 at mem[1]
mem 1 + 69420 storew

// Store a double word with value of 6969696969 at mem[3]
// Note the byte offset taken into account due to previously
//   storing a word (2 bytes) at mem[1].
mem 3 + 6969696969 stored

// Store a quad word with value of 19696942042069696969 at mem[7]
mem 7 + 19696942042069696969 storeq

// Final memory layout
// 0  1  2  3  4  5  6  7  8  9  10  11  12  13  14 ...
// 69 69420 6969696969- 19696942042069696969------- ...
```

Stack Output:
```
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'load' - Memory Manipulation <a name="kw-load"></a>
Push a value on to the stack from a given address in the memory allocated during runtime (see related).

A size of value to store at an address must be selected with the following format: \
`load<x>` \
Where `x` is any of the following:
- 'b' - byte        | 8  bits
- 'w' - word        | 16 bits
- 'd' - double word | 32 bits
- 'q' - quad word   | 64 bits

###### Remember, it is up to you to not access invalid memory addresses.

```
[addr] -> [value]
```

Equivalent:
- No equivalent

Related:
- Keyword: [mem](#kw-mem)
- Keyword: [store](#kw-store)

Example:
```
// This is the `store` example from above with a few
//  modifications made after the HERE comment.

// Store a byte with value of 69 at mem[0]
mem 69 storeb

// Store a word with value of 69420 at mem[1]
mem 1 + 69420 storew

// Store a double word with value of 6969696969 at mem[3]
// Note the byte offset taken into account due to previously
//   storing a word (2 bytes) at mem[1].
mem 3 + 6969696969 stored

// Store a quad word with value of 19696942042069696969 at mem[7]
mem 7 + 19696942042069696969 storeq

// Final memory layout
// 0  1  2  3  4  5  6  7  8  9  10  11  12  13  14 ...
// 69 69420 6969696969- 19696942042069696969------- ...

// HERE

// Print 8 bits of memory starting at mem[0]
mem     loadb dump

// Print 16 bits of memory starting at mem[1]
mem 1 + loadw dump

// Print 32 bits of memory starting at mem[3]
mem 3 + loadd dump

// Print 64 bits of memory starting at mem[7]
mem 7 + loadq dump

```

Stack Output:
```
```

Standard Output:
```
69
69420
6969696969
19696942042069696969
```

[To Keywords](#sparrow-keywords)

---

#### 'shl' - Bitwise Operator <a name="kw-shl"></a>
Pushes the address of the memory allocated at run-time.

For now, this is hard-coded in the Sparrow executable to 720kb. Although that should be enough for everyone,
there will be a CCLI option in the future to specify the exact amount of bytes you would like to allocate.
###### Remember, it is up to you to not access invalid memory addresses.
To access any address within the memory, simply add the byte offset to the address, like so `mem <byte offset> +`. \
Next, use it with the memory access keywords that accept memory addresses as arguments (see related).

```
[a][b] -> [a << b]
```

Equivalent:
- Operator: [<<](#op-bit-shl)

Related:
- Keyword: [shr](#kw-shr)

Example:
```
// 1: 001
//      /
//     /
//    V
// 4: 100
1 2 shl
```

Stack Output:
```
[4]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'shr' - Bitwise Operator <a name="kw-shr"></a>
Shifts the bits of `a` to the right by `b` amount of bits.

```
[a][b] -> [a >> b]
```

Equivalent:
- Operator: [>>](#op-bit-shr)

Related:
- Keyword: [shl](#kw-shl)

Example:
```
// 32: 100000
//     \
//      \
//       V
// 8:  001000
32 2 shr
```

Stack Output:
```
[8]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'and' - Bitwise Operator <a name="kw-and"></a>
Perform a bitwise AND operation on two popped values, `a` and `b`.

An AND operation entails the output only containing a `1` if both inputs do.

```
[a][b] -> [a && b]
```

Equivalent:
- Operator: [&&](#op-bit-and)

Related:
- Keyword: [or](#kw-or)

Example:
```
// 7:  0111
// 14: 1110
// 6:  0110
7 14 and
```

Stack Output:
```
[6]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'or' - Bitwise Operator <a name="kw-or"></a>
Perform a bitwise OR operation on two popped values, `a` and `b`.

An OR operation entails the output containing a `1` if one or both of the inputs do.

```
[a][b] -> [a || b]
```

Equivalent:
- Operator: [||](#op-bit-or)

Related:
- Keyword: [and](#kw-and)

Example:
```
// 7:  0111
// 14: 1110
// 15: 1111
7 14 or
```

Stack Output:
```
[15]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'mod' - Operator <a name="kw-mod"></a>
Pop two values off the stack, `a` and `b`, then push the result of `a` modulo `b`.

A modulus operation entails dividing and then taking the remainder, aka what is left-over.

```
[a][b] -> [a % b]
```

Equivalent:
- Operator: [%](#op-modulo)

Related:
- No related

Example:
```
20 15 mod
```

Stack Output:
```
[5]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

### File Operations

The file operations are highly unstable, inperformant. Requires further testing and Fixing.

#### 'open_file' - Operator <a name="kw-f-open"></a>
Pop two values off the stack, `path` and `mode` then push a file pointer to an opened file. \
Used with file operation keywords (see related).

File paths are always relative to wherever the Sparrow executable was when it compiled the program. \
File paths are NOT relative to the source code, or generated executable.

```
[path][mode] -> [file pointer]
```

Equivalent:
- No equivalent

Related:
- Keyword: [write](#kw-write)
- Keyword: [append](#kw-append)
- Keyword: [write_to_file](#kw-f-write)
- Keyword: [close_file](#kw-f-close)

Example:
```
"myFile.txt" write open_file
```

Stack Output:
```
[pointer to writeable file]
```

Standard Output:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'write_to_file' - Operator <a name="kw-f-write"></a>
Pop four values off the stack, then use them as arguments to call `fwrite` from the C RunTime.

Number of bytes per character is usually one unless you are doing some weird utf-16 stuff.

File paths are always relative to wherever the Sparrow executable was when it compiled the program. \
File paths are NOT relative to the source code, or generated executable.

```
[content str][num bytes per character][num characters to write][file pointer] -> []
```

Equivalent:
- No equivalent

Related:
- Keyword: [open_file](#kw-f-open)
- Keyword: [close_file](#kw-f-close)

Example:
```
// Store file pointer in mem[0] thru mem[7]
mem "myFile.txt" write open_file storeq

// Content String
"I want to write this string to a text file\n"

// The length of the string is determined
dup length_s

// Next, the number of bytes per character must be set
1

// Then, it must be arranged correctly in-between the
//   string and it's length
// [str][len][1] -> [str][1][len]
swap

// Load file pointer from memory
mem loadq

// Call `write_to_file`
// 4 arguments: [content str][num bytes per character][num characters][file ptr]
write_to_file

// Load file pointer and close it
mem loadq close_file
```

Stack Output:
```
[]
```

Standard Output:
```
```

`myFile.txt` Contents:
```
I want to write this string to a text file

```

---

#### 'close_file' - Operator <a name="kw-f-close"></a>
Pops a single value off the stack, `file ptr`, then closes the file opened at that pointer.

Best practices indicate that every opened file must be closed before execution halts.

File paths are always relative to wherever the Sparrow executable was when it compiled the program. \
File paths are NOT relative to the source code, or generated executable.

```
[file pointer] -> []
```

Equivalent:
- No equivalent

Related:
- Keyword: [open_file](#kw-f-open)

Example:
```
"myFile.txt" write open_file

close_file
```

Stack Output:
```
[]
```

Standard Output:
```
```

`myFile.txt` Contents:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'length_s' - Operator <a name="kw-strlen"></a>
Pop a string off the stack, then return the length of that string back on to the stack.

```
[string] -> [length]
```

Equivalent:
- No equivalent

Related:
- Keyword: [write_to_file](#kw-f-write)

Example:
```
"Hello, World!" length_s dump
```

Stack Output:
```
[]
```

Standard Output:
```
13
```

[To Keywords](#sparrow-keywords)

---

#### 'write' - Operator <a name="kw-write"></a>
Pushes a file mode constant on to the stack.

Used with `open_file` to indicate that the file should start empty, creating the file if it doesn't already exist. \
This means opening a file in this way over-writes any data previously stored there.

```
[] -> [file mode `write` constant]
```

Equivalent:
- No equivalent

Related:
- Keyword: [open_file](#kw-f-open)
- Keyword: [append](#kw-append)

Example:
```
"myFile.txt" write open_file
close_file
```

Stack Output:
```
[]
```

Standard Output:
```
```

`myFile.txt` Contents:
```
```

[To Keywords](#sparrow-keywords)

---

#### 'append' - Operator <a name="kw-append"></a>
Pushes a file mode constant on to the stack.

Used with `open_file` to indicate that it should keep the contents of the file. \
Anything written to the file is put after the contents that were there already.

```
[] -> [file mode `append` constant]
```

Equivalent:
- No equivalent

Related:
- Keyword: [open_file](#kw-f-open)
- Keyword: [write](#kw-write)

Example:
```
"myFile.txt" append open_file
close_file
```

Stack Output:
```
[]
```

Standard Output:
```
```

`myFile.txt` Contents:
```
```

[To Keywords](#sparrow-keywords) \
[To Definitions](#sparrow-definitions) \
[To Top](#top)

---

## <a name="how-to-use-anchor"></a><a name="cross-platform-anchor"></a> How to build a Sparrow program
So, you've written a program, what do you do now that you want to run it?

Build it yourself using CMake after cloning the repository its really simple! (further instructions [down below](#how-to-build-sparrow)).

There are two assembly syntaxes Sparrow supports (for now):
- [GAS](#use-sparrow-GAS)
  - [Windows](#use-sparrow-GAS-windows)
- [NASM](#use-sparrow-NASM)
  - [Windows](#use-sparrow-NASM-windows)


### GAS, or the [GNU assembler](https://en.wikipedia.org/wiki/GNU_Assembler) <a name="use-sparrow-GAS"></a>

#### Windows <a name="use-sparrow-GAS-windows"></a>
I would not really recommand this!
For Windows, there is a little funky business... MinGW, the 'normal' installation manager for GNU tools on Windows, doesn't support 64 bits. \
Luckily, there is a community-fix, [TDM-GCC-64](https://jmeubank.github.io/tdm-gcc/), that solves this exact problem, so go donate to this person for doing the hard work that all of us can now use. If for some reason the github was taken down, or anything of the sort, [here](https://web.archive.org/web/20220105210812/https://github.com/jmeubank/tdm-gcc) is a link to a wayback machine snapshot. \
It's a very easy to use installer, and comes with a whole host of very useful 64 bit tools on Windows. Install it at the default location, otherwise Sparrow will need to be passed the path to the `gcc` executable using the SCLI `-a` option.

To familiarize yourself with the Sparrow Command Line Interface (CCLI), run the following command:

```
Sparrow.exe -h` or `Sparrow.exe --help
```

This will list all of the possible flags and options that may be passed to Sparrow.

Example command to compile `test.spar` to an executable on Windows: \
```
Sparrow.exe -GAS test.spar
```

Example command with output renamed: \
```
Sparrow.exe -GAS -add-ao "-o my-output-name" -o my-output-name test.spar
```

### NASM <a name="use-sparrow-NASM"></a>
Just use the one that is packed with the Sparrow Repository.

But if you want an updated version on Windows you can [download the installer from the official website](https://www.nasm.us/)


#### You must ensure that you have some sort of linker on your machine that can link against the standard C runtime of whatever platform you're on.

- [GoLink](http://godevtool.com/) is my recommendation on Windows. \
GoLink is easy to use and fast to setup; simply extract it and it's ready.


Once all the pre-requisites are installed, now comes time to use the CCLI, or Sparrow Command Line Interface. \
To avoid headache as much as possible, Sparrow sets default values based on your operating system. \
If you get any errors, there are a multitude of command line options to help rectify the situation. (see [Common Errors](#common-errors-anchor))

### Windows <a name="use-sparrow-NASM-windows"></a>
Open a terminal and navigate to the directory containing `Sparrow.exe`. \
To familiarize yourself with the options of the CCLI, run the following command: \
`Sparrow.exe -h` or `Sparrow.exe --help` \
A lot of different options will come up, with clear explanations on what everything does.

Basic example: \
`Sparrow.exe -o my_program test.spar`

Or, if Sparrow is giving errors about not finding assembler/linker: \
`Sparrow.exe -a /Path/To/NASM/nasm.exe -l /Path/To/GoLink/golink.exe test.spar`

Alternatively, you could add the directory containing the executable to your system's [PATH environment variable](https://www.c-sharpcorner.com/article/add-a-directory-to-path-environment-variable-in-windows-10/): \
`Sparrow.exe -a nasm.exe -l golink.exe test.spar`

By default, the assembler and linker options are setup for Windows, using NASM and GoLink. \
If your situation is different, make sure to specify the correct options using `-ao` and `-lo` respectively.

### <a name="common-errors-anchor"></a>Common Errors
- "Assembler not found at x"
  - Solution: Specify a valid path, including file name and extension, to the assembler executable using `-a` or `--assembler-path`
- "Linker not found at x"
  - Solution: Specify a valid path, including file name and extension, to the linker executable using `-l Path/To/Linker.exe` or `--linker-path Path/To/Linker.exe`
- The stdout and stderr of any commands run are redirected to a log file in the same directory as the Sparrow executable. The contents of these files are printed to the console when the verbose flag is passed to Sparrow through the CCLI with `-v` or `--verbose`.

[To Top](#top)

---

## <a name="how-to-build-sparrow"></a>How to build Sparrow from source
This project uses [CMake](https://cmake.org/) to build Sparrow for any platform that CMake supports (which is a lot). \
This means Sparrow source code can be easily built in your favorite IDE that supports C++.

First, on any platform, clone this repository to your local machine.

Run the following command in the repository directory: \
`cmake -S . -B build/`

This will use CMake to build a build system with the default generator on your platform. \
Once complete, open the build directory, and build Sparrow using the build system you just generated.

Windows example: \
Open Visual Studio solution and build with `F6`

[To Top](#top)

---
### Easy Test Up
Easiest way to see how the Compiler Works is to use the Powershell file. This would basically run the two tests.
```
test.ps1
```
But, you can always create the build manually.

[To Top](#top)

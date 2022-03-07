# Corth<a name="top"></a>

It's like [Porth](https://gitlab.com/tsoding/porth/-/tree/master/), which is like [Forth](https://www.forth.com/forth/) but written in Python, but written in C++. But I don't actually know for sure since I never programmed in Porth or Forth, I only heard that they are some sort of stack-based programming language. Corth is also stack-based programming language. Which makes it just like Porth, just like Forth, am I rite?

## Background

This was literlly fueled by a high I was chasing at that momemt. I wanted to be better at Programming specifically low level programming.

Corth is:
- [x] A [stack based](https://en.wikipedia.org/wiki/Stack-oriented_programming) programming language
    I do recommend you to read the link above.
- [x] [Turing complete](https://en.wikipedia.org/wiki/Turing_completeness)
- [x] Compileable to a dynamic executable linked with the C RunTime.
- [x] Totally awesome and annoying language.


## Something to Understand Before Checking Corth out
Corth uses the [system](https://en.cppreference.com/w/cpp/utility/program/system) function within C++ to run external commands on your computer. These commands can be affected by user input, so running corth has the potential to run any command on your system if you tell it to, including malicious ones. Be sure to check and double check any commands you see that use the `-a` or `-l` compiler options, as these tell Corth to run a different command than the default. Every command run by Corth is echoed to the standard out with a '[CMD]' prefix.
Be careful when running commands that you don't want to run, as they can be used to run malicious code.

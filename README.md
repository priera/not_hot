not_hot
=======

Yet not another "not hot" programming language compiler

About the cool programming language 
-----------------------------------

Cool is a programming language designed by Stanford university, and is used as a reference language in compiler's classes. You can find basic information about it [here](https://en.wikipedia.org/wiki/Cool_(programming_language))

About "not_hot"
---------------

Not_hot is a compiler for the cool language. I tagged it as "yet not another" because it does not follow many of the conventions that many others "cool" compilers follows: 

* It produces a single executable, instead of four.
* Is object oriented: each compiler phase has its own set of classes to do their job. These classes are splitted in several files, instead of being all in the same file.
* Also, this implementation doesn't rely on "extern" variables, nor in macros.

However, not_hot still uses the data structures implemented and provided by Stanford -stringtable, symbol table, and tree package-. Many debugging functionalities -such as printing the program's tree- are those provided by Stanford. The compiler also generates MIPS code, just as the reference compiler, and the custom exception handler is the same than Stanford's.
I do not reclaim any kind of intellectual property over Stanford's code, and not copyright infringement is intended.

Requirements
------------

It has been developed under Linux, so such a system should be needed in order to compile and run it.
You'll also need:

* g++
* cmake
* make
* flex
* bison
* spim -to run the produced code-

Compile and run
---------------

Download the project at a directory of your election. Navigate to that directory and type:

> $mkdir build  
> $cd build  
> $cmake ..  
> $make  

To run the compiler on a file called "test.cl", from the directory "buid", type:

> $./not_hot test.cl  

That will produce a file called test.s. 

Execute the compiled program
----------------------------

If you want to run the file "test.s" using spim, you only have to type:

> $spim -file test.s  

However, before doing that, you will need to change the default exception handler that spim loads at startup. The exception handler you have to use is called trap.handler.s and you'll find it in the directory <project_dir>/build/as . As far as I know, spim doesn't let you set an specific exception handler, so I replaced the one provided by the one not_hot needs.  
You can use another simulator, such as QtSpim, which lets you set a specific exception handler.


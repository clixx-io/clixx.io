Project/Makefile Generator
==========================


A Makefile generator for AVR-GCC
--------------------------------

Motivation
----------

When starting to develop with microcontrollers, one is faced with two different things to learn:

    The controller itself, its IO registers, hardware setup, everything that is written in the datasheet.

    The development environment, its library reference, compiler usage, specific issues of the toolchain used.
    
    How to combine those things with the application requirement

What is so special about AVR-GCC?

Note that by using the term AVR-GCC, this actually refers to the entire toolchain. This toolchain consists of avr-gcc itself, avr-binutils (assembler, linker, object file handling), and avr-libc. Optionally, various add-on software can be used:

    avr-gdb, the GNU debugger port to the AVR controller series

    AVRDUDE, a program to upload/download the AVR controller's memory contents (i. e., program the flash ROM)

Most commercial development environments do not only bundle software components as those mentioned above, but also provide a so-called Integrated Development Environment (IDE) that glues everything together, and usually also provides some form of project management.

In contrast, AVR-GCC is just the compiler and tools, so the developers are free to choose whatever development environment they prefer. Project management is usually left to the standard make utility that is in widespread use to organize software projects, which is controlled by a file conventionally called Makefile.

In order to get started, people are thus required to also understand at least the basic principles of make, including the rough setup of a Makefile. While projects like WinAVR already did a good job by not only providing a single installer (for Windows users) for all the tools mentioned above but also providing a well-commented Makefile template developers could use to base their own Makefile on, it is still observed that many starters struggle in this step. They are eager to get their first project(s) up & running, so after reading the datasheets and avr-libc reference, they'd rather not like to also study the GNU make reference right now as well.
Solution

mk_project has mainly been written to help AVR-GCC developers to quickly setup a Makefile for their project. Don't worry, they'll eventually realize that studying the make manual will be worth its time, too :-), but for a quick start, this Makefile generator is intended to help them getting up & running easily.

During the work on this project, I eventually realized that such a tool can be a help even for experienced users, to avoid them the nuisance of picking up one of the existing Makefiles from a previous project, using it as a template for their new project. In particular small projects (quick tests for some feature etc.) could benefit by this tool as well.

While Programmer's Notepad 2, the editor that ships with WinAVR and that aims to become an IDE for general software development, intends to eventually provide project management capabilities some day (i. e., help maintaining a Makefile), Mfile has been designed as a small and quick project to fill that gap and be available right now, just concentrating on the AVR-GCC platform only.

See also the README that accompanies the distribution.
Download

There are two slightly different versions provided here. Except for the file format (.tar.gz vs. .zip) they are basically the same. However, the .zip version is mainly intented for WinAVR users, its makefile_template is the template shipped by WinAVR. The .tar.gz version is primarily targeted to Unix users, its makefile_template has a few less comments (in the assumption that most Unix users already have a basic knowledge of the make utility), and an attempt has been made to replace any GNU make specific constructs from the WinAVR template by generic make compatible constructs. It is known to run with both, GNU make as well as BSD make, and is believed to be also compatible with SysV make. The main difference in usage is that there is no automagic dependency generation, but the traditional make depend approach is used. Also, the verbose output from the WinAVR template has been reduced to just the usual output that is commonly used in Unix Makefiles.

mk_project can be downloaded here:

    ZIP version (WinAVR)
    .tar.gz version (Unix)

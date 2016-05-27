
# argumatrix

AUTHOR:
Fuan Pu, <Pu.Fuan@gmail.com>,
School of Software, Tsinghua University



## Introduction

Argumatrix is a set of C/C++ codes to intended to 
assist a researcher in studying argumentation algorithms
based on matrix theory. 


## Build

The argumatrix can be build both on Window and Linux. 

### Build on Windows

#### 1. Install dependences
  
  The argumatrix depends on two open source libraries: [boost][BOOST] and [SWI-Prolog][SWI-PROLOG].
  
  - Build Boost on Windows: 
 
    Most Boost libraries are header-only: they consist entirely of header files containing templates and inline functions, and require no separately-compiled library binaries or special treatment when linking. However, some libraries must be built separately (see [Header-Only Libraries](http://www.boost.org/doc/libs/1_61_0/more/getting_started/windows.html#header-only-libraries)). The following Boost libraries are used in argumatrix:
- [x] [Boost.Graph](http://www.boost.org/doc/libs/1_61_0/libs/graph/doc/index.html) is used to construct and manage an argument graph.
- [x] [Boost.dynamic_bitset](http://www.boost.org/doc/libs/1_61_0/libs/dynamic_bitset/dynamic_bitset.html) is used to implement the bitvector and bitmatrix modules.
- [x] [Boost.Regex](http://www.boost.org/doc/libs/1_61_0/libs/regex/doc/html/index.html) is used in module **parser** to read aspartix files.

   in which the Boost.Regex library needs to be built separately. The simplest way to build boost libraries by Visual C++ can refer to [Build Boost libraries by Visual C++](http://www.boost.org/doc/libs/1_61_0/more/getting_started/windows.html#or-build-binaries-from-source). More details see [Boost - Getting Started on Windows][BOOST]
   
- Install SWI-Prolog

  The stable release sources and excutables of SWI-Prolog can be downloaded from <http://www.swi-prolog.org/download/stable>.
  
#### 2. Build argumatrix on windows
 
 The argumatrix can be easily build on Windows, and can be debugged and run on Microsoft Visual Studio 2012. It is compatible with x64 machines in particular (however you need to make the x64 adjustments in VS2012 yourself).
 
- Download the source code package from github.
- Open *argumatrix.sln* in Visual Studio 2012. You will see that the argumatrix consists of several projects. Each project is a separate module and owns independent function. I provide each project with a main function, which can be used for the unit testing of the project, and in particular provides instructions to use the source codes in each module. I have configured these projects successfully, and if you have the different setting with mine, they may not be compiled successfully. Then, you should reconfigure them as the following steps.

```
argumatrix/ ..................................... The "argumatrix" root directory
    argumatrix.sln ............................................ The solution file
    Project/ .........................................The project (module) folder
        XXX.hpp ................................. The source codes of each module
        XXX_main.cpp .................... The cpp code where the main function is
        Project.vcxproj...................The main project file for VC++ projects
        Project.vcxproj.filters................The filters file for VC++ projects 
        Makefile..................The makefile for compiling the project on linux
```


- Link Boost within the Visual Studio IDE

   For header-only libraries, you should specify where the boost header files are. 
   
   + Right-click the project in the _Solution Explorer_ pane and select _Properties_ from the resulting pop-up menu.
   + In _Configuration Properties_ > *C/C++* > *General* > *Additional Include Directories*, enter the path to the Boost root directory, e.g., "D:/Boost/boost_1_57_0".
   
   For separately-compiled libraries, you should also do the following steps:
  + In *Configuration Properties* > *Linker* > *Additional Library Directories*, enter the path to the Boost binaries, e.g. D:/Boost/boost_1_57_0/stage/lib/.

 - Link SWI-Prolog within the Visual Studio IDE
   
   + Add include folder: In *Configuration Properties* > *C/C++* > *General* > *Additional Include Directories*, enter the path to the SWI-Prolog root directory, e.g., "D:/swipl/include/".
   + Add swipl library: In *Configuration Properties* > *Linker* > *Input* > *Additional dependences*, enter the path to the SWI-Prolog library, e.g. "D:/swipl/lib/libswipl.dll.a".
  
- Build the solution in Debug and Release mode.
  
### Build on Linux  

To build the argumatrix on linux may be quite easy since Boost and SWI-Prolog can be installed on linux. Of course, you can install them by compiling the source codes (see [Install SWI-Prolog on Linux from source codes](http://www.swi-prolog.org/build/unix.html)).

#### 1. Install Boost and SWI-Prolog on Linux

- Install Boost

  On Ubuntu, you can use the following command to install boost libraries
  
```
sudo apt-get install libboost-all-dev
```
    
  As I use the Boost.Regex in module parser, you can compile the parser project by the following command
  
```
g++ -std=gnu++11 parser_main.cpp -lboost_regex -I ../
```

- Install SWI-Prolog

```
sudo apt-add-repository ppa:swi-prolog/stable
sudo apt-get update
sudo apt-get install swi-prolog
```
    
  If you need a fairly recent version, I advice to compile the sources of the SWI-Prolog (see [Install SWI-Prolog on Linux from source codes](http://www.swi-prolog.org/build/unix.html)).


#### 2. Build argumatrix on linux

The argumatrix are organized by several projects, representing independent function module. Each project has a main function entry, which can be used for unit testing, and provides instructions to show how to use these codes. I provide each project with a *Makefile*, you can build each project easily with it.
   
```
make                           % default compile and link
make clean                     % clean all intermediate files
make rebuild                   % clean and build
make compile                   % only compile
```

You can also compile each project with one command, e.g., to build the argmat-clpb project, you can use:
```
g++ -std=gnu++11 argmat-clpb-main.cpp -L. -lswipl -lboost_regex -I ../ -I /usr/lib/swi-prolog/include/ -o argmat-clpb.a
```


## Modules and Usages
### dung_theory

  The dung_theory module is the core module, and contains
the basic concepts and operations of Dung's abstract
argumentation. It also provides an abstract reasoner, which
contains some basic operations about Dung's acceptability
semantics, and implements the grounded reasoner. This module 
is essential to all modules.

  **[Usage:]**
  The following codes present a basic way to construct an abstract
  argumentation framework:
  ```C++
  // #include "DungAF.hpp"
  DungAF daf;  // define an abstract argumentation framework
	
  Argument a = daf.addArgument("A");  // add an argument with label "A"
  Argument b = daf.addArgument("B");
  Argument c = daf.addArgument("C");
  Argument d = daf.addArgument("D");
  Argument e = daf.addArgument("E");
  Argument f = daf.addArgument("F");

  // add attacks
  daf.addAttack(a, b);  
  daf.addAttack(b, c);
  daf.addAttack(a, d);
  daf.addAttack(d, e);
  
  // or add attacks by labels
  daf.addAttack("D", "F");
  
  // print the argumentation framework
  cout << daf.toString() << endl;
  
  // Each argument does not only have an unique label, but also has
  // an unique index. The following codes can be used to obtain the
  // index of an argument when given the label or argument
  size_type idx1 = daf.getArgumentIdx("A");   // Given the label
  size_type idx2 = daf.getArgumentIdx(a);     // Given the Argument
  
  // The next code is to obtains all labels of arguments in an 
  // argumentation framework, and store them into a vector of strings.
  vector<string> labels = daf.getArgumentLabels();
  
  // Then, we can achieve the label of an argument by its unique index, e.g.,
  string _arg_label = labels[idx2];
  ```
  

### bitmatrix

  The bitmatrix module defines two data structures, 
_bitvector_ and _bitmatrix_, that are used to represent
an abstract argumentation framework. The _bitvector_ can 
be used to represent a set of arguments as there is a one-to-one correspondence between a set and a Boolean vector. For example, let the universal set \f$\mathcal{X}=\{x_1,x_2,\cdots,x_5\}\f$ and the set \f$S=\{x_1, x_2, x_5\}\f$, then bitvector of \f$S\f$ can be represented as \f$bv=11001\f$, in contrary, given a bitvector, we can easily obtain the set with respect to \f$\mathcal{X}\f$. The set operations, such as union, intersection and negate operations, 
can be easily implemented by the operators of _bitvector_ . The _bitmatrix_ is an array of bitvectors. It can be used to represent the attack
graph of an argumentation framework, or a set of extensions. 


 **[Usage:]**
 The bitvector is implemented based on boost librarirs:  <boost/dynamic_bitset/dynamic_bitset.hpp>. 
 ```C++
 // #include "bitvector.hpp"
 // #include "bitmatrix.hpp"
 
 // Create a bitvector with a string
 bitvector bv1("11001001");
 cout << bv1 << endl;    // standard output
 
 // Create a bitvector of length 8 with all bits are 0s
 bitvector bv2(8, 0);
 
 // intersection 
 bitvector bv3 = bv1 & bv2;
 
 // union
 bitvector bv3 = bv1 | bv2;
 
  // negate
 bitvector bv4 = ~bv1;
 
 // intersects with ..
 bool is_in = bv1.intersects(bv2);
 // or
 bool is_in2 = (bv1 * bv2);
 
 // is subset of
 bool is_sub = bv1.is_subset_of(bv2);
 
 // is equal
 bool is_eq = (bv1 == bv2);
 
 
 // Create an 8x8 bitmatrix 
 bitmatrix bm1(8);
 bm1[1][4] = 1;      // set a bit
 
 // Get the 5-th row of bm1, which is a bitvector
 bitvector bv5 = bm1[5];
 
 // The multiplication of a bitmatrix and a bitvector
 bitvector bv6 = bm1 * bv1;
 
 // Dung's abstract argumentation framework is a directed graph,
 // which can be represented by an adjacent matrix with only 0s and 1s.
 bitmatrix bm2 = daf.getAttackMatrix().
 ```

### parser

   The parser module is provided to read abstract argumentation
frameworks from files. Now, it supports the formats _aspartix_
and _tgf_.

   **[Usage:]**
This module provides two static function to read _aspartix_ and _tgf_ files
respectively.
```C++
std::string inputFile = "../datasets/kleinberg/9/Kle_n3e0.9_01.dl";

DungAF daf;

// read aspartix 
parser::Aspartix2DungAF(inputFile, daf);

// read tgf
parser::TrivialGraph2DungAF(inputFile, daf);
```

### PlReasoner

  The PlReasoner module implements some acceptability semantics based on
the SWI-Prolog engine using the prolog module Constraint Logic Programming
over Boolean variables. It currently contains ConflictfreeReasoner, 
StableReasoner, AdmissibleReasoner and CompleteReasoner. 


  **[Usage:]**
```C++
Reasoner* rsner = new AdmissiblePlReasoner(daf, "AD2");
cout << "EE:" << endl;
rsner->task_EE();
```

### argmat-clpb

  The argmat-clpb module provides a command line interface to excute argumentation reasoning tasks based on PlReasoner. The command line interface follows the _probo_ interface in the [rules of ICCMA15][ICCMA15].

- Command line interface
    + ./argmat-clpb
      
       To display version information and usages.
    
    + ./argmat-clpb --help
       
       To print usages.
    
    + ./argmat-clpb --problem
     
       To print the supported acceptability semantics and reasoning tasks.
    
    + ./argmat-clpb --format
      
       To print the supported file format.
    
    + ./argmat-clpb -p <problem> -f <file> -fo <fileformat> [-a <additional_parameter>] [-o <file>]
      * -p  Specify the problems, 
      * -f  Specify the file which stores the argumentation framework
      * -fo Specify the file format
      * -a  Specify the additional argument(s), if specify a set of arguments, it must be divided by a comma with no space. For example, "-a arg1,arg2,arg3"
      * -o  Specify the output file where the result are stored. This parameter is optional, if it is not specified, then the standard output is used.




[ICCMA15]: http://argumentationcompetition.org/2015/rules.html "rules"
[BOOST]: http://www.boost.org/ "boost"
[BOOST-WIN]: http://www.boost.org/doc/libs/1_61_0/more/getting_started/windows.html "boost-win"
[SWI-PROLOG]: http://www.swi-prolog.org/ "swi-prolog"
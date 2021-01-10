# llvm-ks

llvm-ks ([https://github.com/CadeBrown/llvm-ks](https://github.com/CadeBrown/llvm-ks)) is a binding of the [LLVM](https://llvm.org/) project to [kscript](https://kscript.org)

## Building


### Unix/Linux/MacOS

#### Requirements

First, install [kscript](https://kscript.org).

Next, install [LLVM](https://llvm.org). Ensure you can run the `llvm-config` utility, for example:

```shell
$ llvm-config --version
9.0.1
```

#### Build

Building is quite straightforward; simply run `make` in the main directory:

```shell
$ make
g++ `llvm-config --cxxflags` -I/usr/local/include -Iinclude -fPIC -c -o src/IRBuilder.o src/IRBuilder.cc
g++ `llvm-config --cxxflags` -I/usr/local/include -Iinclude -fPIC -c -o src/Type.o src/Type.cc
...
g++  \
	src/IRBuilder.o src/Type.o src/Module.o src/Context.o src/main.o src/BasicBlock.o src/Value.o \
	-Wl,-znodelete `llvm-config core native --link-static --ldflags --libs` -ldl -lm -L/usr/local/lib -shared -o libksm_llvm.so
```



Once compiled, you should have the utility `llvm-config` available (it may be somewhere outside your shell's search path -- look and see where you installed it to, and add it if neccessary).



### Windows

TODO: I haven't been able to build on Windows yet...


## Running

Some basic examples are given in the `examples` folder. Typically these will either print the LLVM IR of the code, or they run the JIT compiler and execute them.

For IR outputs (i.e. plaintext), you can redirect to the `lli` program (included with LLVM), like so:

```shell
$ ./examples/basic.ks
; ModuleID = '<module>'
source_filename = "<module>"

@0 = private unnamed_addr constant [14 x i8] c"hello, world\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main(i32, i8**) {
entry:
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @0, i32 0, i32 0))
  ret i32 0
}
$ ./examples/basic.ks | lli
hello, world
```

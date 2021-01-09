#!/usr/bin/env ks
""" basic.ks - Simple example showing the kscript LLVM bindings

You can use the 'lli' program (LLVM interpreter) to run the generated IR:

$ ./examples/basic.ks | lli
hello, world


@author: Cade Brown <cade@kscript.org>
"""

# Our bindings
import llvm


# Create a context (needed for everything else)
ctx = llvm.Context()

# Create a module to store our code in
mod = llvm.Module('<module>', ctx)


# Create some basic types to make the code more readable
i32 = llvm.Type.Int(ctx, 32)
char = llvm.Type.Int(ctx, 8)
charp = char.Pointer()
charpp = charp.Pointer()

# Externally declare 'printf'
printf = llvm.Value.Function(mod, 'printf', llvm.Type.Function(i32, (charp,), true))

# Define a 'main' function, which is the entry point
main = llvm.Value.Function(mod, 'main', llvm.Type.Function(i32, (i32, charpp)))

# Create a block, so can add actual code
entry = llvm.BasicBlock(ctx, main, 'entry')

# Now, create a builder (this makes things much easier)
B = llvm.IRBuilder(ctx)
B.SetInsertPoint(entry)

# capture arguments (unneeded in this example)
#argc = main.getFuncArg(0)
#argv = main.getFuncArg(1)

# Insert the code:
# ```
# printf("hello, world\n")
# ```
msg = B.GlobalStringPtr("hello, world\n")
B.Call(printf, (msg,))

# return a status of 0 (success)
B.Ret(i32(0))

# print IR of the module
print (repr(mod))

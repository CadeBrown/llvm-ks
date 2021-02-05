#!/usr/bin/env ks
""" trig.ks - Simple example showing trig functions

You can use the 'lli' program (LLVM interpreter) to run the generated IR:

$ ./examples/trig.ks | lli
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
double = llvm.Type.Double(ctx)
char = llvm.Type.Int(ctx, 8)
charp = char.Pointer()
charpp = charp.Pointer()

# Externally declare 'printf'
printf = llvm.Value.Function(mod, 'printf', llvm.Type.Function(i32, (charp,), true))

# Externally declare 'sin'
sin = llvm.Value.Function(mod, 'llvm.sin', llvm.Type.Function(double, (double,), true))


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
msg = B.GlobalStringPtr("value: sin(%lf) = %lf\n")
B.Call(printf, (msg, double(1.0), B.Call(sin, (double(1.0),))))

# return a status of 0 (success)
B.Ret(i32(0))

# print IR of the module
print (repr(mod))

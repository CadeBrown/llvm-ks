/* llvm-ks.hh - kscript bindings for a subset of LLVM
 *
 * 
 * SEE: https://llvm.org/
 * 
 * @author: Cade Brown <cade@kscript.org>
 */

#ifndef LLVM_KS_H__
#define LLVM_KS_H__

/* kscript header (for C++) */
#include <ks/kscxx.hh>

/* kscript module name */
#define M_NAME "llvm"


/* LLVM API */

#include <llvm/Support/raw_ostream.h>

#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/APFloat.h>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/PassManager.h>

#include <llvm/Target/TargetMachine.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <llvm/IR/LegacyPassManager.h>



using namespace llvm;


/* Types */

/* llvm.Context - Wrapper around 'LLVMContext*'
 *
 */
typedef struct ksllvm_Context_s {
    KSO_BASE

    /* Value being wrapped */
    LLVMContext* val;

}* ksllvm_Context;

/* llvm.IRBuilder - Wrapper around 'IRBuilder<>*'
 *
 */
typedef struct ksllvm_IRBuilder_s {
    KSO_BASE
    
    /* Value being wrapped */
    IRBuilder<>* val;

}* ksllvm_IRBuilder;

/* llvm.Module - Wrapper around 'Module*'
 *
 */
typedef struct ksllvm_Module_s {
    KSO_BASE

    /* Value being wrapped */
    Module* val;

}* ksllvm_Module;

/* llvm.BasicBlock - Wrapper around 'BasicBlock*'
 *
 */
typedef struct ksllvm_BasicBlock_s {
    KSO_BASE

    /* Value being wrapped */
    BasicBlock* val;

}* ksllvm_BasicBlock;

/* llvm.Type - Wrapper around 'Type*'
 *
 */
typedef struct ksllvm_Type_s {
    KSO_BASE

    /* Value being wrapped */
    Type* val;

}* ksllvm_Type;


/* llvm.Value - Wrapper around 'Value*'
 *
 */
typedef struct ksllvm_Value_s {
    KSO_BASE

    /* Value being wrapped */
    Value* val;

}* ksllvm_Value;


/* Functions */


/* Wrapper functions */
ksllvm_Type ksllvm_Type_wrap(Type* val);
ksllvm_Value ksllvm_Value_wrap(Value* val);
ksllvm_BasicBlock ksllvm_BasicBlock_wrap(BasicBlock* val);



/* Types */
extern ks_type
    ksllvmt_Context,
    ksllvmt_IRBuilder,
    ksllvmt_Module,
    ksllvmt_BasicBlock,
    ksllvmt_Type,
    ksllvmt_Value
;


/* Internals */

void _ksllvmi_Context();
void _ksllvmi_IRBuilder();
void _ksllvmi_Module();
void _ksllvmi_BasicBlock();
void _ksllvmi_Type();
void _ksllvmi_Value();

#endif /* LLVM_KS_HH__ */



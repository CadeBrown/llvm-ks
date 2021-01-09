/* IRBuilder.cc - wrapper around 'IRBuilder*'
 *
 * @author: Cade Brown <cade@kscript.org>
 */
#include <llvm-ks.hh>

#define T_NAME M_NAME ".IRBuilder"


/* Internals */

/* C-API */

/* Type Functions */

static KS_TFUNC(T, free) {
    ksllvm_IRBuilder self;
    KS_ARGS("self:*", &self, ksllvmt_IRBuilder);


    KSO_DEL(self);
    return KSO_NONE;
}

static KS_TFUNC(T, init) {
    ksllvm_IRBuilder self;
    ksllvm_Context ctx;
    KS_ARGS("self:* ctx:*", &self, ksllvmt_IRBuilder, &ctx, ksllvmt_Context);

    self->val = new IRBuilder<>(*ctx->val);

    return KSO_NONE;
}


static KS_TFUNC(T, CreateGlobalStringPointer) {
    ksllvm_IRBuilder self;
    ks_str val;
    KS_ARGS("self:* val:*", &self, ksllvmt_IRBuilder, &val, kst_str);

    return (kso)ksllvm_Value_wrap(self->val->CreateGlobalStringPtr(val->data));
}


static KS_TFUNC(T, SetInsertPoint) {
    ksllvm_IRBuilder self;
    ksllvm_BasicBlock bb;
    KS_ARGS("self:* bb:*", &self, ksllvmt_IRBuilder, &bb, ksllvmt_BasicBlock);

    self->val->SetInsertPoint(bb->val);

    return KSO_NONE;
}


static KS_TFUNC(T, Ret) {
    ksllvm_IRBuilder self;
    ksllvm_Value val = NULL;
    KS_ARGS("self:* ?val:*", &self, ksllvmt_IRBuilder, &val, ksllvmt_Value);

    ReturnInst* res = NULL;
    if (val) {
        res = self->val->CreateRet(val->val);
    } else {
        res = self->val->CreateRetVoid();
    }

    return (kso)ksllvm_Value_wrap(res);
}

static KS_TFUNC(T, CreateCall) {
    ksllvm_IRBuilder self;
    ksllvm_Value val;
    kso args = KSO_NONE;
    KS_ARGS("self:* val:* ?args", &self, ksllvmt_IRBuilder, &val, ksllvmt_Value, &args);

    Value* res = NULL;
    if (args == KSO_NONE) {
        res = self->val->CreateCall(val->val, {});
    } else {
        /* Collect arguments */
        vector<Value*> out_args;
        ks_cit cit = ks_cit_make(args);
        kso ob;
        while ((ob = ks_cit_next(&cit)) != NULL) {
            if (!kso_issub(ob->type, ksllvmt_Value)) {
                KS_THROW(kst_TypeError, "Expected all arguments to be of type '%R', but got object of type '%T'", ksllvmt_Value, ob);
                cit.exc = true;
            } else {
                out_args.push_back(((ksllvm_Value)ob)->val);
            }

            KS_DECREF(ob);
        }
        ks_cit_done(&cit);
        if (cit.exc) return NULL;

        res = self->val->CreateCall(val->val, out_args);
    }

    return (kso)ksllvm_Value_wrap(res);
}

static KS_TFUNC(T, CreateAlloca) {
    ksllvm_IRBuilder self;
    ksllvm_Type tp;
    ksllvm_Value num = NULL;
    KS_ARGS("self:* tp:* ?num:*", &self, ksllvmt_IRBuilder, &tp, ksllvmt_Type, &num, ksllvmt_Value);

    return (kso)ksllvm_Value_wrap(self->val->CreateAlloca(tp->val, num ? num->val : NULL));
}



/* Macros to generate methods */

#define WRAP_V(_name) static KS_TFUNC(T, _name) { \
    ksllvm_IRBuilder self; \
    ksllvm_Value a0; \
    KS_ARGS("self:* val:*", &self, ksllvmt_IRBuilder, &a0, ksllvmt_Value); \
    return (kso)ksllvm_Value_wrap(self->val->_name(a0->val)); \
}

#define WRAP_VV(_name) static KS_TFUNC(T, _name) { \
    ksllvm_IRBuilder self; \
    ksllvm_Value a0, a1; \
    KS_ARGS("self:* lhs:* rhs:*", &self, ksllvmt_IRBuilder, &a0, ksllvmt_Value, &a1, ksllvmt_Value); \
    return (kso)ksllvm_Value_wrap(self->val->_name(a0->val, a1->val)); \
}


#define WRAP_VT(_name) static KS_TFUNC(T, _name) { \
    ksllvm_IRBuilder self; \
    ksllvm_Value a0; \
    ksllvm_Type a1; \
    KS_ARGS("self:* val:* tp:*", &self, ksllvmt_IRBuilder, &a0, ksllvmt_Value, &a1, ksllvmt_Type); \
    return (kso)ksllvm_Value_wrap(self->val->_name(a0->val, a1->val)); \
}


WRAP_VT(CreateTrunc)
WRAP_VT(CreateZExt)
WRAP_VT(CreateSExt)
WRAP_VT(CreateZExtOrTrunc)
WRAP_VT(CreateSExtOrTrunc)

WRAP_VT(CreateFPTrunc)
WRAP_VT(CreateFPExt)

WRAP_VT(CreateFPToUI)
WRAP_VT(CreateFPToSI)
WRAP_VT(CreateUIToFP)
WRAP_VT(CreateSIToFP)

WRAP_VT(CreatePtrToInt)
WRAP_VT(CreateIntToPtr)
WRAP_VT(CreateBitCast)
WRAP_VT(CreateAddrSpaceCast)
WRAP_VT(CreateZExtOrBitCast)
WRAP_VT(CreateSExtOrBitCast)
//WRAP_VT(CreateCast)
WRAP_VT(CreatePointerCast)
WRAP_VT(CreateTruncOrBitCast)
WRAP_VT(CreatePointerBitCastOrAddrSpaceCast)
//WRAP_VT(CreateIntCast)
WRAP_VT(CreateBitOrPointerCast)
WRAP_VT(CreateFPCast)


WRAP_V(CreateLoad)
WRAP_VV(CreateStore)


WRAP_V(CreateNot)
WRAP_V(CreateNeg)

WRAP_VV(CreateShl)
WRAP_VV(CreateLShr)
WRAP_VV(CreateAShr)
WRAP_VV(CreateAnd)
WRAP_VV(CreateXor)
WRAP_VV(CreateOr)

WRAP_VV(CreateICmpEQ)
WRAP_VV(CreateICmpNE)
WRAP_VV(CreateICmpSLT)
WRAP_VV(CreateICmpSLE)
WRAP_VV(CreateICmpSGT)
WRAP_VV(CreateICmpSGE)
WRAP_VV(CreateICmpULT)
WRAP_VV(CreateICmpULE)
WRAP_VV(CreateICmpUGT)
WRAP_VV(CreateICmpUGE)

WRAP_VV(CreateAdd)
WRAP_VV(CreateSub)
WRAP_VV(CreateMul)
WRAP_VV(CreateSDiv)
WRAP_VV(CreateUDiv)
WRAP_VV(CreateSRem)
WRAP_VV(CreateURem)

WRAP_V(CreateFNeg)

WRAP_VV(CreateFAdd)
WRAP_VV(CreateFSub)
WRAP_VV(CreateFMul)
WRAP_VV(CreateFDiv)
WRAP_VV(CreateFRem)

WRAP_VV(CreateFCmpOEQ)
WRAP_VV(CreateFCmpONE)
WRAP_VV(CreateFCmpOLT)
WRAP_VV(CreateFCmpOLE)
WRAP_VV(CreateFCmpOGT)
WRAP_VV(CreateFCmpOGE)
WRAP_VV(CreateFCmpORD)

WRAP_VV(CreateFCmpUEQ)
WRAP_VV(CreateFCmpUNE)
WRAP_VV(CreateFCmpULT)
WRAP_VV(CreateFCmpULE)
WRAP_VV(CreateFCmpUGT)
WRAP_VV(CreateFCmpUGE)
WRAP_VV(CreateFCmpUNO)



/* Export */

ks_type ksllvmt_IRBuilder = NULL;

void _ksllvmi_IRBuilder() {

    ksllvmt_IRBuilder = ks::make_type(T_NAME, kst_object, sizeof(ksllvm_IRBuilder_s), -1, "LLVM IRBuilder", {
        {"__free",                 ksf_wrap(T_free_, T_NAME ".__free(self)", "")},
        {"__init",                 ksf_wrap(T_init_, T_NAME ".__init(self, ctx)", "")},

        {"GlobalStringPtr",        ksf_wrap(T_CreateGlobalStringPointer_, T_NAME ".GlobalStringPointer(val)", "")},

        {"SetInsertPoint",         ksf_wrap(T_SetInsertPoint_, T_NAME ".SetInsertPoint(self, bb)", "")},

        {"Ret",                    ksf_wrap(T_Ret_, T_NAME ".Ret(self, val=none)", "")},

        {"Call",                   ksf_wrap(T_CreateCall_, T_NAME ".Call(self, val, args=(,))", "")},

        #define _W(_name, _src, _sig, _doc) {_name, ksf_wrap(_src, T_NAME "." _name _sig, _doc)},

        _W("Alloca",     T_CreateAlloca_, "(self, tp)", "")
        _W("Load",       T_CreateLoad_, "(self, ptr)", "")
        _W("Store",      T_CreateStore_, "(self, val, ptr)", "")

        _W("Neg", T_CreateNeg_, "(self, val)", "")
        _W("Not", T_CreateNot_, "(self, val)", "")

        _W("Shl", T_CreateShl_, "(self, lhs, rhs)", "")
        _W("LShr", T_CreateLShr_, "(self, lhs, rhs)", "")
        _W("AShr", T_CreateAShr_, "(self, lhs, rhs)", "")
        _W("And", T_CreateAnd_, "(self, lhs, rhs)", "")
        _W("Xor", T_CreateXor_, "(self, lhs, rhs)", "")
        _W("Or", T_CreateOr_, "(self, lhs, rhs)", "")

        _W("ICmpEQ", T_CreateICmpEQ_, "(self, lhs, rhs)", "")
        _W("ICmpNE", T_CreateICmpNE_, "(self, lhs, rhs)", "")
        _W("ICmpSLT", T_CreateICmpSLT_, "(self, lhs, rhs)", "")
        _W("ICmpSLE", T_CreateICmpSLE_, "(self, lhs, rhs)", "")
        _W("ICmpSGT", T_CreateICmpSGT_, "(self, lhs, rhs)", "")
        _W("ICmpSGE", T_CreateICmpSGE_, "(self, lhs, rhs)", "")
        _W("ICmpULT", T_CreateICmpULT_, "(self, lhs, rhs)", "")
        _W("ICmpULE", T_CreateICmpULE_, "(self, lhs, rhs)", "")
        _W("ICmpUGT", T_CreateICmpUGT_, "(self, lhs, rhs)", "")
        _W("ICmpUGE", T_CreateICmpUGE_, "(self, lhs, rhs)", "")

        _W("Add",        T_CreateAdd_, "(self, lhs, rhs)", "")
        _W("Sub",        T_CreateSub_, "(self, lhs, rhs)", "")
        _W("Mul",        T_CreateMul_, "(self, lhs, rhs)", "")
        _W("SDiv",       T_CreateSDiv_, "(self, lhs, rhs)", "")
        _W("UDiv",       T_CreateUDiv_, "(self, lhs, rhs)", "")
        _W("SRem",       T_CreateSRem_, "(self, lhs, rhs)", "")
        _W("URem",       T_CreateURem_, "(self, lhs, rhs)", "")

        _W("FNeg",       T_CreateFNeg_, "(self, val)", "")

        _W("FAdd",       T_CreateFAdd_, "(self, lhs, rhs)", "")
        _W("FSub",       T_CreateFSub_, "(self, lhs, rhs)", "")
        _W("FMul",       T_CreateFMul_, "(self, lhs, rhs)", "")
        _W("FDiv",       T_CreateFDiv_, "(self, lhs, rhs)", "")
        _W("FRem",       T_CreateFRem_, "(self, lhs, rhs)", "")

        _W("FCmpOEQ", T_CreateFCmpOEQ_, "(self, lhs, rhs)", "")
        _W("FCmpONE", T_CreateFCmpONE_, "(self, lhs, rhs)", "")
        _W("FCmpOLT", T_CreateFCmpOLT_, "(self, lhs, rhs)", "")
        _W("FCmpOLE", T_CreateFCmpOLE_, "(self, lhs, rhs)", "")
        _W("FCmpOGT", T_CreateFCmpOGT_, "(self, lhs, rhs)", "")
        _W("FCmpOGE", T_CreateFCmpOGE_, "(self, lhs, rhs)", "")
        _W("FCmpORD", T_CreateFCmpORD_, "(self, lhs, rhs)", "")

        _W("FCmpUEQ", T_CreateFCmpUEQ_, "(self, lhs, rhs)", "")
        _W("FCmpUNE", T_CreateFCmpUNE_, "(self, lhs, rhs)", "")
        _W("FCmpULT", T_CreateFCmpULT_, "(self, lhs, rhs)", "")
        _W("FCmpULE", T_CreateFCmpULE_, "(self, lhs, rhs)", "")
        _W("FCmpUGT", T_CreateFCmpUGT_, "(self, lhs, rhs)", "")
        _W("FCmpUGE", T_CreateFCmpUGE_, "(self, lhs, rhs)", "")
        _W("FCmpUNO", T_CreateFCmpUNO_, "(self, lhs, rhs)", "")

    });
}


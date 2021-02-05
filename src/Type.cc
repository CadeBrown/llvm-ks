/* Type.cc - wrapper around 'Type*'
 *
 * @author: Cade Brown <cade@kscript.org>
 */
#include <llvm-ks.hh>

#define T_NAME M_NAME ".Type"


/* Internals */

/* C-API */

ksllvm_Type ksllvm_Type_wrap(Type* val) {
    ksllvm_Type self = KSO_NEW(ksllvm_Type, ksllvmt_Type);

    self->val = val;

    return self;
}

/* Type Functions */

static KS_TFUNC(T, free) {
    ksllvm_Type self;
    KS_ARGS("self:*", &self, ksllvmt_Type);


    KSO_DEL(self);
    return KSO_NONE;
}

static KS_TFUNC(T, new) {
    ks_type tp;
    KS_ARGS("tp:*", &tp, kst_type);

    KS_THROW(kst_TypeError, "Type '%R' is abstract and cannot be created like this", tp);
    return NULL;
}


static KS_TFUNC(T, str) {
    ksllvm_Type self;
    KS_ARGS("self:*", &self, ksllvmt_Type);

    /* Build up stream */
    string sval;
    raw_string_ostream svals(sval);
    svals << *self->val;
    svals.flush();

    return (kso)ks::make_str(sval);
}


static KS_TFUNC(T, call) {
    ksllvm_Type self;
    kso arg = KSO_NONE;
    KS_ARGS("self:* ?arg", &self, ksllvmt_Type, &arg);

    if (self->val->isIntegerTy()) {
        APInt arg_api;
        if (arg == KSO_NONE) {
            arg_api = APInt(self->val->getIntegerBitWidth(), 0);
        } else if (kso_issub(arg->type, kst_str)) {
            arg_api = APInt(self->val->getIntegerBitWidth(), ((ks_str)arg)->data, 10);
        } else if (kso_is_int(arg)) {
            ks_int arg_ksi = kso_int(arg);
            if (!arg_ksi) return NULL;
            /* TODO: use base 16? or some other constructor */
            ks_str arg_str = ks_fmt("%S", arg_ksi);
            KS_DECREF(arg_ksi);
            if (!arg_str) return NULL;

            arg_api = APInt(self->val->getIntegerBitWidth(), arg_str->data, 10);
            KS_DECREF(arg_str);
        } else {
            KS_THROW_CONV(arg->type, self->type);
            return NULL;
        }

        return (kso)ksllvm_Value_wrap(ConstantInt::get(self->val, arg_api));
    } else if (self->val->isFloatingPointTy()) {

        Value* res = NULL;
        if (arg == KSO_NONE) {
            res = ConstantFP::get(self->val, 0.0);
        } else if (kso_issub(arg->type, kst_str)) {
            res = ConstantFP::get(self->val, ((ks_str)arg)->data);
        } else if (kso_is_float(arg) || kso_is_int(arg)) {
            ks_cfloat cf;
            if (!kso_get_cf(arg, &cf)) return NULL;
            /* TODO: use base 16? or some other constructor */
            ks_str arg_str = ks_fmt("%f", cf);
            if (!arg_str) return NULL;
            res = ConstantFP::get(self->val, arg_str->data);
            KS_DECREF(arg_str);
        } else {
            KS_THROW_CONV(arg->type, self->type);
            return NULL;
        }

        return (kso)ksllvm_Value_wrap(res);
    }

  
    KS_THROW_CONV(arg->type, self->type);
    return NULL;
}


static KS_TFUNC(T, Int) {
    ksllvm_Context ctx;
    ks_cint bits = 32;
    KS_ARGS("ctx:* ?bits:cint", &ctx, ksllvmt_Context, &bits);

    return (kso)ksllvm_Type_wrap(Type::getIntNTy(*ctx->val, bits));
}

static KS_TFUNC(T, Half) {
    ksllvm_Context ctx;
    KS_ARGS("ctx:*", &ctx, ksllvmt_Context);

    return (kso)ksllvm_Type_wrap(Type::getHalfTy(*ctx->val));
}

static KS_TFUNC(T, Float) {
    ksllvm_Context ctx;
    KS_ARGS("ctx:*", &ctx, ksllvmt_Context);

    return (kso)ksllvm_Type_wrap(Type::getFloatTy(*ctx->val));
}

static KS_TFUNC(T, Double) {
    ksllvm_Context ctx;
    KS_ARGS("ctx:*", &ctx, ksllvmt_Context);

    return (kso)ksllvm_Type_wrap(Type::getDoubleTy(*ctx->val));
}

static KS_TFUNC(T, FP128) {
    ksllvm_Context ctx;
    KS_ARGS("ctx:*", &ctx, ksllvmt_Context);

    return (kso)ksllvm_Type_wrap(Type::getFP128Ty(*ctx->val));
}

static KS_TFUNC(T, Pointer) {
    ksllvm_Type self;
    KS_ARGS("self:*", &self, ksllvmt_Type);

    return (kso)ksllvm_Type_wrap(self->val->getPointerTo());
}

static KS_TFUNC(T, Function) {
    ksllvm_Type rtype;
    kso argtypes = KSO_NONE;
    bool vararg = false;
    KS_ARGS("rtype:* ?argtypes ?vararg:bool", &rtype, ksllvmt_Type, &argtypes, &vararg);

    ks_list argtypes_l = ks_list_newi(argtypes);
    if (!argtypes_l) return NULL;

    vector<Type*> argtypes_v;
    int i;
    for (i = 0; i < argtypes_l->len; ++i) {
        /* TODO: automatic conversion? */
        ksllvm_Type at = (ksllvm_Type)argtypes_l->elems[i];
        if (!kso_issub(at->type, ksllvmt_Type)) {
            KS_THROW(kst_TypeError, "Expected all of 'argtypes' to be of type '%R', but got object of type '%T'", ksllvmt_Type, at->type);
            KS_DECREF(argtypes_l);
            return NULL;
        }

        argtypes_v.push_back(at->val);
    }

    KS_DECREF(argtypes_l);
    return (kso)ksllvm_Type_wrap(FunctionType::get(rtype->val, argtypes_v, vararg));
}



/* Export */

ks_type ksllvmt_Type = NULL;

void _ksllvmi_Type() {

    ksllvmt_Type = ks::make_type(T_NAME, kst_object, sizeof(ksllvm_Type_s), -1, "LLVM Type", {
        {"__free",                 ksf_wrap(T_free_, T_NAME ".__free(self)", "")},
        {"__new",                  ksf_wrap(T_new_, T_NAME ".__new(self)", "")},
        {"__repr",                 ksf_wrap(T_str_, T_NAME ".__repr(self)", "")},
        {"__str",                  ksf_wrap(T_str_, T_NAME ".__str(self)", "")},

        {"__call",                 ksf_wrap(T_call_, T_NAME ".__call(self, obj=none)", "Create an 'llvm.Value' from a type, by converting it")},


        {"Int",                    ksf_wrap(T_Int_, T_NAME ".Int(ctx, bits=32)", "Get an integral type with the given number of bits. Signedness is in the operations, not type (so there is no distiction between 'signed' and 'unsigned' here)")},

        {"Half",                   ksf_wrap(T_Half_, T_NAME ".Half(ctx)", "Get the 'half' type, which is a 16 bit floating point scalar (IEEE)")},
        {"Float",                  ksf_wrap(T_Float_, T_NAME ".Float(ctx)", "Get the 'float' type, which is a 32 bit floating point scalar (IEEE)")},
        {"Double",                 ksf_wrap(T_Double_, T_NAME ".Double(ctx)", "Get the 'double' type, which is a 64 bit floating point scalar (IEEE)")},
        {"FP128",                  ksf_wrap(T_FP128_, T_NAME ".FP128(ctx)", "Get the 'FP128' type, which is a 128 bit floating point scalar (IEEE)")},
        {"Function",               ksf_wrap(T_Function_, T_NAME ".Function(rtype, argtypes=none, vararg=false)", "Get a type representing a function with signature 'argtypes -> rtype' (or, 'rtype FUNC(argtypes)', with 'vararg' controlling whether it is a variadic function")},
        {"Pointer",                ksf_wrap(T_Pointer_, T_NAME ".Pointer(self)", "Get a type representing a pointer to 'self'")},

    });
}


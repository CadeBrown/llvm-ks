/* Value.cc - wrapper around 'Value*'
 *
 * @author: Cade Brown <cade@kscript.org>
 */
#include <llvm-ks.hh>

#define T_NAME M_NAME ".Value"


/* Internals */

/* C-API */

ksllvm_Value ksllvm_Value_wrap(Value* val) {
    ksllvm_Value self = KSO_NEW(ksllvm_Value, ksllvmt_Value);

    self->val = val;

    return self;
}

/* Type Functions */

static KS_TFUNC(T, free) {
    ksllvm_Value self;
    KS_ARGS("self:*", &self, ksllvmt_Value);


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
    ksllvm_Value self;
    KS_ARGS("self:*", &self, ksllvmt_Value);

    string sval;
    raw_string_ostream svals(sval);
    svals << *self->val;
    svals.flush();

    return (kso)ks::make_str(sval);
}



static KS_TFUNC(T, Function) {
    ksllvm_Module mod;
    ks_str name;
    ksllvm_Type functype;
    KS_ARGS("mod:* name:* functype:*", &mod, ksllvmt_Module, &name, kst_str, &functype, ksllvmt_Type);

    return (kso)ksllvm_Value_wrap(Function::Create(dyn_cast<FunctionType>(functype->val), Function::ExternalLinkage, name->data, *mod->val));
}



/* Export */

ks_type ksllvmt_Value = NULL;

void _ksllvmi_Value() {

    ksllvmt_Value = ks::make_type(T_NAME, kst_object, sizeof(ksllvm_Type_s), -1, "LLVM Type", {
        {"__free",                 ksf_wrap(T_free_, T_NAME ".__free(self)", "")},
        {"__new",                  ksf_wrap(T_new_, T_NAME ".__new(self)", "")},
        {"__repr",                 ksf_wrap(T_str_, T_NAME ".__repr(self)", "")},
        {"__str",                  ksf_wrap(T_str_, T_NAME ".__str(self)", "")},

        {"Function",               ksf_wrap(T_Function_, T_NAME ".Function(mod, name, functype)", "Create a new IR function, in 'mod' with 'name', of function type 'functype'")},

    });
}


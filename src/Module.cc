/* Module.cc - wrapper around 'Module*'
 *
 * @author: Cade Brown <cade@kscript.org>
 */
#include <llvm-ks.hh>

#define T_NAME M_NAME ".Module"


/* Internals */

/* C-API */

/* Type Functions */

static KS_TFUNC(T, free) {
    ksllvm_Module self;
    KS_ARGS("self:*", &self, ksllvmt_Module);

    //delete self->val;

    KSO_DEL(self);
    return KSO_NONE;
}

static KS_TFUNC(T, init) {
    ksllvm_Module self;
    ks_str name;
    ksllvm_Context ctx;
    KS_ARGS("self:* name:* ctx:*", &self, ksllvmt_Module, &name, kst_str, &ctx, ksllvmt_Context);

    self->val = new Module(name->data, *ctx->val);

    return KSO_NONE;
}

static KS_TFUNC(T, str) {
    ksllvm_Module self;
    KS_ARGS("self:*", &self, ksllvmt_Module);

    /* Build up stream */
    string sval;
    raw_string_ostream svals(sval);
    svals << *self->val;
    svals.flush();

    return (kso)ks::make_str(sval);
}


/* Export */

ks_type ksllvmt_Module = NULL;

void _ksllvmi_Module() {

    ksllvmt_Module = ks::make_type(T_NAME, kst_object, sizeof(ksllvm_Module_s), -1, "LLVM Module", {
        {"__free",                 ksf_wrap(T_free_, T_NAME ".__free(self)", "")},
        {"__init",                 ksf_wrap(T_init_, T_NAME ".__init(self)", "")},
        {"__repr",                 ksf_wrap(T_str_, T_NAME ".__repr(self)", "")},
        {"__str",                  ksf_wrap(T_str_, T_NAME ".__str(self)", "")},

    });
}


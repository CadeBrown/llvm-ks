/* Context.cc - wrapper around 'Context*'
 *
 * @author: Cade Brown <cade@kscript.org>
 */
#include <llvm-ks.hh>

#define T_NAME M_NAME ".Context"


/* Internals */

/* C-API */

/* Type Functions */

static KS_TFUNC(T, free) {
    ksllvm_Context self;
    KS_ARGS("self:*", &self, ksllvmt_Context);

    //delete self->val;

    KSO_DEL(self);
    return KSO_NONE;
}

static KS_TFUNC(T, init) {
    ksllvm_Context self;
    KS_ARGS("self:*", &self, ksllvmt_Context);

    self->val = new LLVMContext();

    return KSO_NONE;
}


/* Export */

ks_type ksllvmt_Context = NULL;

void _ksllvmi_Context() {

    ksllvmt_Context = ks::make_type(T_NAME, kst_object, sizeof(ksllvm_Context_s), -1, "LLVM Context", {
        {"__free",                 ksf_wrap(T_free_, T_NAME ".__free(self)", "")},
        {"__init",                 ksf_wrap(T_init_, T_NAME ".__init(self)", "")},
    });
}


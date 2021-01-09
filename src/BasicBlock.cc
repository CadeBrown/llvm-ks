/* BasicBlock.cc - wrapper around 'BasicBlock*'
 *
 * @author: Cade Brown <cade@kscript.org>
 */
#include <llvm-ks.hh>

#define T_NAME M_NAME ".BasicBlock"


/* Internals */

/* C-API */

ksllvm_BasicBlock ksllvm_Type_wrap(BasicBlock* val) {
    ksllvm_BasicBlock self = KSO_NEW(ksllvm_BasicBlock, ksllvmt_BasicBlock);

    self->val = val;

    return self;
}


/* Type Functions */

static KS_TFUNC(T, free) {
    ksllvm_BasicBlock self;
    KS_ARGS("self:*", &self, ksllvmt_BasicBlock);

    KSO_DEL(self);
    return KSO_NONE;
}

static KS_TFUNC(T, init) {
    ksllvm_BasicBlock self;
    ksllvm_Context ctx;
    ksllvm_Value func = NULL;
    ks_str name = NULL;
    KS_ARGS("self:* ctx:* ?func:* ?name:*", &self, ksllvmt_BasicBlock, &ctx, ksllvmt_Context, &func, ksllvmt_Value, &name, kst_str);

    self->val = BasicBlock::Create(*ctx->val, name ? name->data : "bb", func ? dyn_cast<Function>(func->val) : NULL);

    return KSO_NONE;
}

static KS_TFUNC(T, str) {
    ksllvm_BasicBlock self;
    KS_ARGS("self:*", &self, ksllvmt_BasicBlock);

    string sval;
    raw_string_ostream svals(sval);
    svals << *self->val;
    svals.flush();

    return (kso)ks::make_str(sval);
}



/* Export */

ks_type ksllvmt_BasicBlock = NULL;

void _ksllvmi_BasicBlock() {

    ksllvmt_BasicBlock = ks::make_type(T_NAME, kst_object, sizeof(ksllvm_BasicBlock_s), -1, "LLVM BasicBlock", {
        {"__free",                 ksf_wrap(T_free_, T_NAME ".__free(self)", "")},
        {"__init",                 ksf_wrap(T_init_, T_NAME ".__init(self, ctx, func=none, name='bb')", "")},
        {"__str",                  ksf_wrap(T_str_, T_NAME ".__str(self)", "")},
        {"__repr",                 ksf_wrap(T_str_, T_NAME ".__repr(self)", "")},

    });
}


/* main.cc - module implementation for LLVM bindings
 *
 *
 * @author: Cade Brown <cade@kscript.org>
 */
#include <llvm-ks.hh>
#include <ks/cext.h>



/* Export */

static ks_module get() {
    /* LLVM initialization */
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmParser();
    LLVMInitializeNativeAsmPrinter();

    _ksllvmi_Context();
    _ksllvmi_IRBuilder();
    _ksllvmi_Module();
    _ksllvmi_BasicBlock();
    _ksllvmi_Type();
    _ksllvmi_Value();


    ks_module res = ks::make_module(M_NAME, "", "LLVM bindings for kscript", {

        /* Constants */

        /* Types */
        {"Context",                (kso)ksllvmt_Context},
        {"IRBuilder",              (kso)ksllvmt_IRBuilder},
        {"Module",                 (kso)ksllvmt_Module},
        {"BasicBlock",             (kso)ksllvmt_BasicBlock},
        {"Type",                   (kso)ksllvmt_Type},
        {"Value",                  (kso)ksllvmt_Value},

    });

    return res;
}


/* C-extension */
KS_CEXT_DECL(get);

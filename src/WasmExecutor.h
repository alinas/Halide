#ifndef HALIDE_WASM_EXECUTOR_H
#define HALIDE_WASM_EXECUTOR_H

/** \file
 *
 * Support for running Halide compiled JavaScript and/or WASM code in-process.
 * Bindings for parameters, extern calls, etc. are established and the
 * JavaScript/WASM code is executed. Allows calls to relaize to work
 * exactly as if native code had been run, but via a JavaScript VM. This is
 * largely used to run all JIT tests with the JavaScript backend, but
 * could have other uses in the future. Currently, V8 is supported, with
 * SpiderMonkey intended to be included soon as well.
 */

#include "Argument.h"
#include "JITModule.h"
#include "Parameter.h"
#include "Target.h"
#include "Type.h"

namespace Halide {
namespace Internal {

struct WasmModuleContents;

/** Handle to compiled JavaScript code which can be called later. */
struct WasmModule {
    Internal::IntrusivePtr<WasmModuleContents> contents;

    /** If the given target can be executed via the wasm executor, return true. */
    static bool can_jit_target(const Target &target);

    /** Compile generated JavaScript or wasm code with a set of externs. */
    static WasmModule compile(
        const Target &target,
        const std::vector<Argument> &arguments,
        const void *source, size_t source_len,
        const std::string &fn_name,
        const std::map<std::string, JITExtern> &externs,
        const std::vector<JITModule> &extern_deps
    );

    /** Run generated previously compiled JavaScript or wasm code with a set of arguments. */
    int run(const std::vector<std::pair<Argument, const void *>> &args);
};

}  // namespace Internal
}  // namespace Halide

#endif  // HALIDE_WASM_EXECUTOR_H
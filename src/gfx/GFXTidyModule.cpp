#include "ClangTidy.h"
#include "ClangTidyModule.h"
#include "ClangTidyModuleRegistry.h"
#include "ImplementationInNamespaceCheck.h"
#include "MainImplementationFilenameCheck.h"
#include "ClassCohesionCheck.h"
#include "PackageNamespaceCheck.h"

namespace clang {
namespace tidy {
namespace gfx {

class GFXTidyModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<ImplementationInNamespaceCheck>(
        "gfx-implementation-in-namespace");
    CheckFactories.registerCheck<MainImplementationFilenameCheck>(
        "gfx-main-implementation-filename");
    CheckFactories.registerCheck<ClassCohesionCheck>("gfx-experimental-class-cohesion");
    CheckFactories.registerCheck<PackageNamespaceCheck>("gfx-experimental-package-namespace");
  }
};

// Register the GFXTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<GFXTidyModule>
    X("gfx-module", "Adds gfx standards checks.");

} // namespace gfx

// This anchor is used to force the linker to link in the generated object file
// and thus register the GFXTidyModule.
volatile int GFXModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang

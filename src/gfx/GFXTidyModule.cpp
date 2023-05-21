#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "ClassCohesionCheck.h"
#include "ImplementationInNamespaceCheck.h"
#include "MainImplementationFilenameCheck.h"
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
    CheckFactories.registerCheck<ClassCohesionCheck>(
        "gfx-experimental-class-cohesion");
    CheckFactories.registerCheck<PackageNamespaceCheck>(
        "gfx-experimental-package-namespace");
  }
};

} // namespace gfx

static ClangTidyModuleRegistry::Add<gfx::GFXTidyModule>
    X("gfx-module", "Adds gfx standards checks.");

volatile int GFXModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang

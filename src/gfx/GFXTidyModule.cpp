#include "gfx/BasenameDeclarationCheck.h"
#include "gfx/ClassCohesionCheck.h"
#include "gfx/FundamentalTypeCheck.h"
#include "gfx/ImplementationInNamespaceCheck.h"
#include "gfx/MainImplementationFilenameCheck.h"
#include "gfx/PackageNamespaceCheck.h"

#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

namespace clang::tidy
{
namespace gfx
{
class GFXTidyModule : public ClangTidyModule
{
  public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
      CheckFactories.registerCheck<ImplementationInNamespaceCheck>(
          "gfx-implementation-in-namespace");
      CheckFactories.registerCheck<MainImplementationFilenameCheck>(
          "gfx-main-implementation-filename");
      CheckFactories.registerCheck<ClassCohesionCheck>(
          "gfx-experimental-class-cohesion");
      CheckFactories.registerCheck<PackageNamespaceCheck>(
          "gfx-experimental-package-namespace");
      CheckFactories.registerCheck<BasenameDeclarationCheck>(
          "gfx-basename-declaration");
      CheckFactories.registerCheck<FundamentalTypeCheck>("gfx-fundamental-type");
    }
};

} // namespace gfx

// NOLINTBEGIN
static ClangTidyModuleRegistry::Add<gfx::GFXTidyModule> X("gfx-module",
                                                          "Adds gfx standards checks.");

volatile int g_GFXModuleAnchorSource = 0;
// NOLINTEND

} // namespace clang

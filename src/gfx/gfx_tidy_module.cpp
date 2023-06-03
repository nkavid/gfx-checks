// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/basename_declaration_check.hpp"
#include "gfx/braced_initialization_check.hpp"
#include "gfx/class_cohesion_check.hpp"
#include "gfx/fundamental_type_check.hpp"
#include "gfx/implementation_in_namespace_check.hpp"
#include "gfx/main_implementation_filename_check.hpp"
#include "gfx/package_namespace_check.hpp"

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
      CheckFactories.registerCheck<BracedInitializationCheck>(
          "gfx-experimental-braced-initialization");
    }
};

} // namespace gfx

// NOLINTBEGIN
static ClangTidyModuleRegistry::Add<gfx::GFXTidyModule> X("gfx-module",
                                                          "Adds gfx standards checks.");

volatile int g_GFXModuleAnchorSource = 0;
// NOLINTEND

} // namespace clang::tidy

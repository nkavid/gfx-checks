#include "ClangTidyTest.h"
#include "gfx/PackageNamespaceCheck.h"
#include "gtest/gtest.h"

#include <iostream>

namespace clang {
namespace tidy {
namespace test {

TEST(GFXModuleTest, PackageNamespaceCheck) {
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options;
  Options.CheckOptions["test-check-0.Allowed"] = "wow";

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<PackageNamespaceCheck>("namespace gfx\n"
                                        "{\n"
                                        "namespace foo\n"
                                        "{\n"
                                        "namespace wow\n"
                                        "{\n"
                                        "namespace utils\n"
                                        "{\n"
                                        "class Foo;\n"
                                        "}\n"
                                        "}\n"
                                        "}\n"
                                        "}\n",
                                        &Errors, "wow/gfx/foo/bar.cpp", None,
                                        Options);

  for (const auto &error : Errors) {
    std::cout << error.Message.Message << '\n';
  }

  EXPECT_EQ(0U, Errors.size());
}

} // namespace test
} // namespace tidy
} // namespace clang

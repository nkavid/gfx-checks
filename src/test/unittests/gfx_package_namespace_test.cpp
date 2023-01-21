#include "ClangTidyTest.h"
#include "gfx/PackageNamespaceCheck.h"
#include "gtest/gtest.h"

#include <iostream>

namespace clang {
namespace tidy {
namespace test {

template <class T> static std::string getErrorString(const T &Errors) {
  if (Errors.size() == 0) {
    return "no errors";
  }

  std::string string{};
  for (const auto &error : Errors) {
    string = string + error.Message.Message + '\n';
  }

  return string;
}

TEST(GFXModuleTestPackageNamespaceCheck, AllowedOption) {
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

  EXPECT_EQ(0U, Errors.size());
}

TEST(GFXModuleTestPackageNamespaceCheck, IncorrectOrder) {
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<PackageNamespaceCheck>("namespace gfx::bar::foo\n"
                                        "{\n"
                                        "class Foo;\n"
                                        "}\n",
                                        &Errors, "gfx/foo/bar/baz.cpp");

  EXPECT_EQ(0U, Errors.size()) << getErrorString(Errors);
}

} // namespace test
} // namespace tidy
} // namespace clang

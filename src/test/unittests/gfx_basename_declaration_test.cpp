#include "ClangTidyTest.h"
#include "gfx/BasenameDeclarationCheck.h"
#include "gtest/gtest.h"

#include <iostream>

namespace clang
{
namespace tidy
{
namespace test
{

template <class T>
static std::string getErrorString(const T& Errors)
{
  if (Errors.size() == 0)
  {
    return "no errors";
  }

  std::string string{};
  for (const auto& error : Errors)
  {
    string = string + error.Message.Message + '\n';
  }

  return string;
}

TEST(GFXModuleTest, ReportOnceIfNoMatch)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace this_is_foo\n"
                                           "{\n"
                                           "class ThisIsFoo;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/this_is_bar.hpp");

  EXPECT_EQ(1U, Errors.size()) << getErrorString(Errors);
}

TEST(GFXModuleTest, CheckForClass)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace foo\n"
                                           "{\n"
                                           "class ThisIsBar;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/this_is_bar.hpp");

  EXPECT_EQ(0U, Errors.size()) << getErrorString(Errors);
}

TEST(GFXModuleTest, CheckForNamespace)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace this_is_bar\n"
                                           "{\n"
                                           "class Foo;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/this_is_bar.hpp");

  EXPECT_EQ(0U, Errors.size()) << getErrorString(Errors);
}

TEST(GFXModuleTest, AcceptDuplication)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace this_is_bar\n"
                                           "{\n"
                                           "class ThisIsBar;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/this_is_bar.hpp");

  EXPECT_EQ(0U, Errors.size()) << getErrorString(Errors);
}

TEST(GFXModuleTest, SupportConfigurableNamingCases)
{
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options{};
  Options.CheckOptions["test-check-0.TypeCase"]      = "lower_case";
  Options.CheckOptions["test-check-0.NamespaceCase"] = "camelBack";
  Options.CheckOptions["test-check-0.FilenameCase"]  = "CamelCase";

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace thisIsFoo\n"
                                           "{\n"
                                           "class this_is_foo;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/ThisIsFoo.hpp",
                                           std::nullopt,
                                           Options);

  EXPECT_EQ(0U, Errors.size()) << getErrorString(Errors);
}

} // namespace test
} // namespace tidy
} // namespace clang

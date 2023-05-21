#include "ClangTidyTest.h"
#include "gfx/ClassCohesionCheck.h"
#include "gfx/ImplementationInNamespaceCheck.h"
#include "gfx/MainImplementationFilenameCheck.h"
#include "gtest/gtest.h"

#include <iostream>

namespace clang
{
namespace tidy
{
namespace test
{

TEST(GFXModuleTest, WrongFileName)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<MainImplementationFilenameCheck>("int main() {}",
                                                  &Errors,
                                                  "foo/bar.cpp");

  EXPECT_EQ("main definition must be in file with 'main' affix",
            Errors[0].Message.Message);
}

TEST(GFXModuleTest, MainFileNameDefault)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<MainImplementationFilenameCheck>("int main() {}",
                                                  &Errors,
                                                  "foo/main.cpp");

  EXPECT_EQ(0U, Errors.size());
}

TEST(GFXModuleTest, MainSuffixOption)
{
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options;
  Options.CheckOptions["test-check-0.Affix"] = "wow;suffix";

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<MainImplementationFilenameCheck>("int main() {}",
                                                  &Errors,
                                                  "foo/bar_main.cpp",
                                                  std::nullopt,
                                                  Options);

  EXPECT_EQ(0U, Errors.size());
}

TEST(GFXModuleTest, ClassCohesion)
{
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options;
  Options.CheckOptions["test-check-0.Score"] = "50";

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<ClassCohesionCheck>("class Foo\n"
                                     "{\n"
                                     "public:\n"
                                     "  int x() {return 5;}\n"
                                     "  int y() {return a;}\n"
                                     "  int z() {return b + a;}\n"
                                     "private:\n"
                                     "  int a{3};\n"
                                     "  int b{4};\n"
                                     "};\n"
                                     "\n"
                                     "int i{8};\n"
                                     "\n"
                                     "class Bar\n"
                                     "{\n"
                                     "public:\n"
                                     "  int y() {return a;}\n"
                                     "private:\n"
                                     "  int a{1};\n"
                                     "  int b{1};\n"
                                     "};\n",
                                     &Errors,
                                     "foo/bar.cpp",
                                     std::nullopt,
                                     Options);

  EXPECT_EQ(3U, Errors.size());
}

} // namespace test
} // namespace tidy
} // namespace clang

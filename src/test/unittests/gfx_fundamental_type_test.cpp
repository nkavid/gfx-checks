#include "ClangTidyTest.h"
#include "gfx/FundamentalTypeCheck.h"
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

TEST(GFXModuleTest, DisallowTiredMemes)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<FundamentalTypeCheck>("typedef unsigned int uint32_t;\n"
                                       "namespace std { class string{}; };\n"
                                       "uint32_t intVar;\n"
                                       "const std::string stringVar;\n"
                                       "const char* charPtr;\n",
                                       &Errors,
                                       "dummyfilename.cpp");

  EXPECT_EQ(3U, Errors.size()) << getErrorString(Errors);
}

} // namespace test
} // namespace tidy
} // namespace clang

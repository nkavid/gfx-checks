#pragma once

#include "ClangTidyTest.h"

#include <string>
#include <vector>

namespace clang::tidy::test::utils
{
static std::string getErrorString(const std::vector<ClangTidyError>& Errors)
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
} // namespace clang::tidy::test::utils

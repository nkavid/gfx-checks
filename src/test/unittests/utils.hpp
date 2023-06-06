#pragma once

#include "ClangTidyDiagnosticConsumer.h"

#include <string>
#include <string_view>
#include <vector>

namespace clang::tidy::test::utils
{
std::string getErrorString(const std::vector<ClangTidyError>& Errors,
                           const std::string_view source = {});

} // namespace clang::tidy::test::utils

#include "utils.hpp"

#include "ClangTidyDiagnosticConsumer.h"

#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace clang::tidy::test::utils
{
namespace
{
std::tuple<std::string, std::string> getSourceStrings(const std::string_view source,
                                                      size_t fileOffset)
{
  const size_t begin = source.rfind('\n', fileOffset);
  const size_t end   = source.find('\n', fileOffset);

  const auto sourceStr = "\"" + std::string{source.substr(begin + 1, end - begin - 1)}
                       + "\": ";

  const auto locationStr = std::string(fileOffset - begin, ' ') + "^\n";

  return std::make_tuple(sourceStr, locationStr);
}
} // namespace

std::string getErrorString(const std::vector<ClangTidyError>& Errors,
                           const std::string_view source)
{
  if (Errors.size() == 0)
  {
    return "no errors\n";
  }

  std::string errorString{};

  for (const auto& error : Errors)
  {
    if (!source.empty())
    {
      const auto& fileOffset = error.Message.FileOffset;

      const auto& [sourceStr, locationLine] = getSourceStrings(source, fileOffset);

      errorString.append(sourceStr);
      errorString.append(error.Message.Message);
      errorString.append("\n");
      errorString.append(locationLine);
    }
    else
    {
      errorString.append(error.Message.Message).append("\n");
    }
  }

  return errorString;
}
} // namespace clang::tidy::test::utils

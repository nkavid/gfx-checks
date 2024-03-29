set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

string(
  APPEND
  GFX_GCC_FLAGS
  " -Werror"
  # " -O3"
)

string(
  APPEND
  GFX_CXX_WARNING_FLAGS
  ${GFX_GCC_FLAGS}
  " -Wall"
  " -Wextra"
  " -Wshadow"
  " -Wnon-virtual-dtor"
  " -Wold-style-cast"
  " -Wcast-align"
  " -Wunused"
  " -Woverloaded-virtual"
  " -Wpedantic"
  " -Wconversion"
  " -Wsign-conversion"
  " -Wmisleading-indentation"
  " -Wduplicated-cond"
  " -Wduplicated-branches"
  " -Wlogical-op"
  " -Wnull-dereference"
  " -Wuseless-cast"
  " -Wdouble-promotion"
  " -Wformat=2"
  " -Weffc++"
  " -Wvexing-parse"
  " -Wfloat-equal"
  " -Wundef"
  " -Wwrite-strings"
  " -Wunused-macros"
  " -Wmissing-declarations"
  " -Wcast-qual"
  " -Warith-conversion"
  " -Wsuggest-attribute=cold"
  " -Wsuggest-attribute=malloc"
  " -Wsuggest-attribute=noreturn"
  " -Wsuggest-override"
  " -Walloc-zero"
  " -Wsuggest-final-types"
  " -Wsuggest-final-methods"
  " -Wc++20-compat"
  " -Wterminate"
  " -Wsubobject-linkage"
  " -Wrange-loop-construct"
  " -Wpessimizing-move"
  " -Wextra-semi"
  " -Wconversion-null"
  " -Wredundant-move"
  " -Wredundant-tags"
  " -Wreorder"
  " -Wsign-promo"
  " -Wredundant-decls"
  " -Wint-in-bool-context"
)

set(CMAKE_CXX_FLAGS "${GFX_CXX_WARNING_FLAGS}")

string(
  APPEND
  CMAKE_C_FLAGS
  ${GFX_GCC_FLAGS}
  " -Wno-declaration-after-statement"
  " -Wall"
  " -Wextra"
  " -Wshadow"
  " -Wcast-align"
  " -Wunused"
  " -Wpedantic"
  " -Wconversion"
  " -Wsign-conversion"
  " -Wmissing-prototypes"
  " -Wmisleading-indentation"
  " -Wduplicated-cond"
  " -Wduplicated-branches"
  " -Wlogical-op"
  " -Wnull-dereference"
  " -Wdouble-promotion"
  " -Wformat=2"
  " -Wfloat-equal"
  " -Wundef"
  " -Wwrite-strings"
  " -Wunused-macros"
  " -Wmissing-declarations"
  " -Wcast-qual"
  " -Warith-conversion"
  " -Wsuggest-attribute=cold"
  " -Wsuggest-attribute=malloc"
  " -Wsuggest-attribute=noreturn"
  " -Walloc-zero"
  " -Wsuggest-final-types"
  " -Wsuggest-final-methods"
  " -Wredundant-decls"
)

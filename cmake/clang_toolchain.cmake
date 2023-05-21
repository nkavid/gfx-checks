set(CMAKE_CXX_COMPILER clang)
set(CMAKE_C_COMPILER clang)

string(APPEND CMAKE_CXX_FLAGS
  " -Werror"
  " -stdlib=libc++"
  " -Weverything"
  " -Wno-c++98-compat"
  " -Wno-c++98-compat-pedantic"
  " -Wno-switch-enum"
  " -Wno-padded"
)

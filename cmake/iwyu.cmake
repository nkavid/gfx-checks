set(GFX_IWYU_EXECUTABLE include-what-you-use;)

set(IWYU_EXTERNAL_PATH /opt/pkg/llvm/llvm_16.0.5/share/include-what-you-use)

set(GFX_IWYU_OPTIONS
    -w;
    -Xiwyu;any;
    -Xiwyu;iwyu;
    -Xiwyu;args;
    -Xiwyu;--no_default_mappings;
    -Xiwyu;--quoted_includes_first;
    -Xiwyu;--no_fwd_decls;
    -Xiwyu;--no_comments;
    -Xiwyu;--cxx17ns;
    -Xiwyu;--with_color;
    -Xiwyu;--mapping_file=${IWYU_EXTERNAL_PATH}/gcc.libc.imp;
    -Xiwyu;--mapping_file=${IWYU_EXTERNAL_PATH}/gcc.stl.headers.imp;
    -Xiwyu;--mapping_file=${IWYU_EXTERNAL_PATH}/gcc.symbols.imp;
    -Xiwyu;--mapping_file=${IWYU_EXTERNAL_PATH}/libcxx.imp;
    -Xiwyu;--mapping_file=${IWYU_EXTERNAL_PATH}/stl.c.headers.imp;
    -Xiwyu;--mapping_file=${IWYU_EXTERNAL_PATH}/boost-all.imp;
)

message(STATUS "gfx::ENABLED include-what-you-use")
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${GFX_IWYU_EXECUTABLE}; ${GFX_IWYU_OPTIONS};)
set(CMAKE_C_INCLUDE_WHAT_YOU_USE ${GFX_IWYU_EXECUTABLE}; ${GFX_IWYU_OPTIONS};)

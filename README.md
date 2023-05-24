# gfx-checks

clang-tidy checks for gfx repository

## Build

```sh
# called from 'llvm-project/' repo root
cmake -Sllvm -Bbuild \
-DCMAKE_CXX_STANDARD=17 \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=/opt/pkg/llvm/llvm-16.0.5 \
-DCLANG_TIDY_ENABLE_STATIC_ANALYZER=True \
-DLLVM_TARGETS_TO_BUILD=X86 \
-DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
-DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" \
-DLLVM_EXTERNAL_PROJECTS=iwyu \
-DLLVM_EXTERNAL_IWYU_SOURCE_DIR=../include-what-you-use
```

Example setup if `llvm-project` and `gfx-checks` repos are in the
same directory and `setup.sh` is called from this repo root.

```sh
./bin/setup.sh ../llvm-project/ src/
```

```sh
# make and run unit tests for gfx module
make ClangTidyTests
../build/tools/clang/tools/extra/unittests/clang-tidy/ClangTidyTests \
--gtest_filter="GFXModuleTest.*"
```

Changes in `clang-tools-extra/unittests/clang-tidy/CMakeLists.txt`
```diff
# [...]

+ add_subdirectory(gfx)
+ gfx_print_info()

# [...]

add_extra_unittest(ClangTidyTests
+  ${GFX_MODULE_UNIT_TESTS}
  AddConstTest.cpp

# [...]

+target_include_directories(ClangTidyTests PRIVATE
+  ${GFX_MODULE_INCLUDE_PATH}
+)

target_link_libraries(ClangTidyTests
  PRIVATE
+  ${GFX_MODULE_LIBRARY}
  clangTidy

# [...]
```

Enable `find_package(REQUIRED)` instead of `gfx_print_info()`.
- [ ] Fix cmake install to simplify integration with both "llvm-project" and "sandbox-gfx".
- [ ] CMake `COMPONENT` GFXTidyTestSource as `INTERFACE FILE_SET`?
- [ ] Why do I want to compile and run other module unit tests? :(

## Load plugin

```bash
clang-tidy --checks="-*,gfx-*" \
  -load build/lib/libgfx_checks.so \
  --list-checks
```

- [ ] Add as custom command to `ALL` with dependency to GFXTidy? Only if installing?

## Checks

- [ ] Remove copyright scripting
- [ ] Add copyright scripting
In that order!

### gfx-implementation-in-namespace

Imitate "llvmlibc-implementation-in-namespace"

```cpp
namespace gfx//::...
{
```

Skip `main` definitions

### gfx-main-implementation-filename

Define `main` in file named `main.cpp`

Added configuration option of `main` affix in filename: suffix, prefix

### gfx-experimental-package-namespace

Match parent directories to namespace

In file `/foo/bar/baz.hpp`

```cpp
namespace gfx::foo::bar
{

// and

namespace gfx
{
namespace foo
{
namespace bar
{
//...
```

`baz` may be class or namespace or function.

### gfx-basename-declaration

Checks if basename of header file matches declaration names. Does not check if implementation file.

```cpp
// Examples in file foo_bar_baz.hpp
namespace foo_bar_baz
{

// or

T FooBarBaz;

// or

void fooBarBaz();
//...
```

Converts names from snake_case and then lowercase. Exact matches of configurable cases not informative and I want to keep acronyms all lowercase or uppercase.

### gfx-fundamental-type

Intended to be used in "applications" and more integrating components to enforce reliance on abstractions and to identify if an abstraction is not working or missing.

String literals not found. Integer literals also but these are magic numbers though? Use strongly typed instead of stringly typed interfaces. *Finish* parsing configurations and options in config utility components and forward *structured parametrized data* to instead of soft weaksauce strings.

Ignores cv qualifier on type and flags `const` qualified variables.

```cpp
// Examples of disallowed signatures
/* built-ins */
float floatVar;
uint32_t intVar; // is 'unsigned int'
bool boolVar;    // Allow or prefer enums?

/* strings */
std::string stringVar;
char* charPtr;
```

`argc` and `argv` should be forwarded to some CLI argument parser component. `argv` not identified as a `char*` but fine for now because "unsafe buffer usage" and c-style pointer and arrays checkers expose this.

Consider a strongly typed and strongly checked `main` definition wrapper? [C++ Weekly: Is a better 'main' possible?](https://www.youtube.com/watch?v=zCzD9uSDI8c)

### gfx-experimental-class-cohesion

Similar to this python checker

[flake8-cohesion](https://github.com/mschwager/cohesion)

- [x] Check for private member usage in methods
- [ ] Check for private method usage in methods

Support cohesion metric for other stuff than class?

- [ ] Check for local linkage declarations usage (constants, variables, functions, ...)


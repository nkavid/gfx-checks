# :do_not_litter: GFXTidy

> Snyggt byggt, fräsig sammanslagningsbegäran.

Clang-tidy checkers used in [nkavid/sandbox-gfx](https://github.com/nkavid/sandbox-gfx).

## Build

```sh
# called from 'llvm-project/' repo root
cmake -Sllvm -Bbuild \
-DCMAKE_CXX_STANDARD=17 \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=/opt/pkg/llvm/llvm_16.0.5 \
-DCLANG_TIDY_ENABLE_STATIC_ANALYZER=True \
-DLLVM_TARGETS_TO_BUILD="X86;NVPTX" \
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
make ClangTidyTests && \
./tools/clang/tools/extra/unittests/clang-tidy/ClangTidyTests \
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
- [x] Fix cmake install to simplify integration with both "llvm-project" and "sandbox-gfx".
- [ ] CMake `COMPONENT` GFXTidyTestSource as `INTERFACE FILE_SET`?
- [ ] Why do I want to compile and run other module unit tests? :(

## Load plugin

```bash
clang-tidy --checks="-*,gfx-*" \
  -load build/lib/libgfx_checks.so \
  --list-checks
```

- [x] Add as custom command to `ALL` with dependency to GFXTidy? Only if installing?

# Checks

The tag 'experimental' in some checker names enables ignoring WIP checks when working on other stuff not on implementing checks. While developing checkers use [compiler-explorer](https://godbolt.org/) which includes AST viewer.

- [gfx-implementation-in-namespace](#gfx-implementation-in-namespace)
- [gfx-main-implementation-filename](#gfx-main-implementation-filename)
- [gfx-basename-declaration](#gfx-basename-declaration)
- [gfx-fundamental-type](#gfx-fundamental-type)
- [gfx-experimental-braced-initialization](#gfx-experimental-braced-initialization)
- [gfx-experimental-package-namespace](#gfx-experimental-package-namespace)
- [gfx-experimental-package-prefix](#gfx-experimental-package-prefix)
- [gfx-experimental-class-cohesion](#gfx-experimental-class-cohesion)


## gfx-implementation-in-namespace

Imitate "llvmlibc-implementation-in-namespace". Implementations belong in namespaces. Check for `gfx` as top project namespace.

```cpp
// nothing here, no exceptions.
namespace gfx
{
  // all the things here
}
// not here, except for some stuff
```

Skip `main()` definitions. Must be in global namespace.


## gfx-main-implementation-filename

Define `main()` in files named `main.cpp`. Option "none" for filename affix.

Added configuration option of main affix in filename. Assuming snake\_case: suffix example `foo_main.cpp`.

```yaml
# In '.clang-tidy' or other custom configuration file.
CheckOptions:
  gfx-main-implementation-filename.Affix: 'none;suffix'
```

Separate `main()` definition from an "application" instantiable in a test. Handle configuration file parsing or command line argument parsing as separate from application logic.


## gfx-basename-declaration

Checks if basename of header file matches declaration names. Does not check in implementation files.

```cpp
// In file foo_bar_baz.hpp
namespace foo_bar_baz {
// or
T FooBarBaz;
// or
void fooBarBaz();
```

Converts names from snake_case and then lowercases all characters before matching. Exact matches of configurable identifier name cases not needed and I want to keep acronyms all lowercase or uppercase.


## gfx-fundamental-type

Intended to be used in "applications" and more integrating components to enforce reliance on abstractions and to identify if an abstraction is not working or missing.

String literals not found. Integer literals also but these are magic numbers though? Use strongly typed instead of stringly typed interfaces. *Finish* parsing configurations and options in config utility components and forward *structured parametrized data* to instead of soft weaksauce strings.

Ignores cv qualifier on type, flags `const` qualified variables.

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

Consider a strongly typed and strongly checked `main()` definition wrapper? [C++ Weekly: Is a better 'main' possible?](https://www.youtube.com/watch?v=zCzD9uSDI8c)


## gfx-experimental-braced-initialization

Modern Effective C++: Item 7: Distinguish between `()` and `{}` when creating objects.

- [ES.20: Always initialize an object](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es20-always-initialize-an-object)
- [ES.23: Prefer the `{}` initializer syntax](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es23-prefer-the--initializer-syntax)
- [ES.64: Use the `T{e}` notation for construction](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es64-use-the-tenotation-for-construction)
- [T.68: Use `{}` rather than `()` within templates to avoid ambiguities](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#t68-use--rather-than--within-templates-to-avoid-ambiguities)

```cpp
/* not ok */
int intVar;           // Uninitialized variable
std::string myString; // Implicit ctor
int five = 6;         // C-style assignment initialization

/* ok! */
auto typeDeducedVar = /* ... */; // copy ctor
void myFunc(int param) {}        // pass-by-value
for (int i = 0; i < N; ++i) {}   // usually what it looks like :|
auto tildes = std::string(7UL, '~'); // initializer list not applicable
```

Enforcement exceptions a bit arbitrary? Specify cases in test.

- [x] Ignore `for` and `catch` statement ancestors
- [ ] More specific diagnostic messages?
- [x] FIXIT fix-in-place feature!
  - Partial support :D
- [ ] Integer FixItHint depends on max 64 bit width? (and unsigned?)
  - Use `clang::TargetInfo`? Reference [Fundamental Types](https://en.cppreference.com/w/cpp/language/types).
- [ ] Floating point literal. Use `clang::SourceManager` somehow to get the source instead of `llvm::APFloat`? Replace assignment with '{' character somehow?
- [ ] String literal?
- [ ] `<anonymous>::getFixItHint()` is very ugly. `clang::RecursiveASTVisitor<>`?

## gfx-experimental-package-namespace

Match parent directories to namespace

In file `/foo/bar/baz.hpp`

```cpp
namespace gfx::foo::bar
{
// or
namespace gfx {
namespace foo {
namespace bar {
```

`baz` may be class or namespace or function.

```yaml
CheckOptions:
  gfx-experimental-package-namespace.Allowed: 'detail'
```

Rename option to IgnoredNamespace? Does not need to match parent directory. Add option to ignore directories? Such as "src" or "include" as is done in "llvm-header-guard"?

## gfx-experimental-package-prefix

Similar to package namespace and project namespace rule. Simple string matching. Acronyms or other mapping rule also directory depth and ignored directories would be nice at larger scale.

```c
/* foo/bar/baz.h */
/* library + component + decl */

void bar_baz_func(void);

struct bar_baz_data{};

typedef struct /* _bar_baz_data? */
{
  /* ... */
} bar_baz_data;

typedef /* anything */ bar_baz_something;

enum bar_baz_something
{
  bar_baz_lorem,
  bar_baz_ipsum
};
```

Check just header; definitions in implementation files must match. Acronym mapping not implemented.

```json
{
  "PackageAcronyms":
  {
    "foo/bar/baz": "fbb",
    "argparse/options": "apo"
  }
}
```


## gfx-experimental-class-cohesion

Similar to the python checker [flake8-cohesion](https://github.com/mschwager/cohesion).

Score classes for member usage in implementations. Low cohesion score may indicate need to split into separate classes with clearer simpler responsibilities.

Score for a member function is ratio of used member variables over total member variables. A total score for a class is the average of its member function scores.

```cpp
class Foo // total score 50
{
  public:
    Foo() {}                // ctor ignored
    ~Foo() = delete;        // no function body
    int x() {return 5;}     // score 0
    int y() {return a;}     // score 50
    int z() {return b + a;} // score 100

  private:
    int a{3};
    int b{4};
};
```

Ignoring constructor and ignoring `=delete` and `=default`. Several other clang-tidy checks and compiler warnings cover that.

- [x] Check for private member usage in methods
   - Track methods or members?
- [ ] Check for private method usage in methods

Support cohesion metric for other stuff than class?

- [ ] Check for local linkage declarations usage (constants, variables, functions, ...)



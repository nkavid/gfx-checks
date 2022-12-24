# gfx-checks

clang-tidy checks for gfx repository

## Build

```sh
# called from 'llvm-project/build/'
cmake -Sclang-tools-extra \
-DCMAKE_BUILD_TYPE=Debug \
-DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
DLLVM_TARGETS_TO_BUILD=X86 \
../llvm 
```

Example setup if `llvm-project` and `gfx-checks` repos are in the
same directory and `setup.sh` is called from this repo root.

```sh
./bin/setup.sh ../llvm-project/ src/
```

Changes in file `clang-tools-extra/clang-tidy/CMakeLists.txt`

```diff
+set(GFX_CLANG_TIDY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})
+add_subdirectory(gfx)
```
in `set(ALL_CLANG_TIDY_CHECKS [...]` specifically
```diff
+  clangTidyGFXModule
```

Changes in `clang-tools-extra/clang-tidy/ClangTidyForceLinker.h`
```diff
+
+// This anchor is used to force the linker to link the GFXModule.
+extern volatile int GFXModuleAnchorSource;
+static int LLVM_ATTRIBUTE_UNUSED GFXModuleAnchorDestination =
+    GFXModuleAnchorSource;
```

Changes in `clang-tools-extra/unittests/clang-tidy/CMakeLists.txt`
```diff
# in add_extra_unittest(ClangTidyTests [...]
+  GFXModuleTest.cpp
```
```diff
# in target_link_libraries(ClangTidyTests [...]
+  clangTidyGFXModule
```

## TODO

- [x] top project namespace

Imitate "llvmlibc-implementation-in-namespace"

```cpp
namespace gfx//::...
{
```

- [x] also skip `main()` in namespace gfx

- [x] define `main()` in file named `main.cpp`

- [x] working unittest

- [x] define `main()` in file with suffix `_main`

- [x] add configuration option of `main` affix: suffix, prefix

- [ ] nested package namespaces

Match directory to namespace

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

`baz` may be class or namespace or function...

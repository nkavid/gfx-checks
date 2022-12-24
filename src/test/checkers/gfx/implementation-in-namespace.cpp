// RUN: %check_clang_tidy %s gfx-implementation-in-namespace %t

#define MACRO_A "defining macros outside namespace is valid"

class ClassB;
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: declaration must be declared within the 'gfx' namespace
struct StructC {};
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: declaration must be declared within the 'gfx' namespace
char *VarD = MACRO_A;
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: declaration must be declared within the 'gfx' namespace
typedef int typeE;
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: declaration must be declared within the 'gfx' namespace
void funcF() {}
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: declaration must be declared within the 'gfx' namespace

namespace namespaceG {
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 'gfx' needs to be the outermost namespace
namespace gfx {
namespace namespaceH {
class ClassB;
} // namespace namespaceH
struct StructC {};
} // namespace gfx
char *VarD = MACRO_A;
typedef int typeE;
void funcF() {}
} // namespace namespaceG

int main() {}
// definitions of main allowed outside gfx namespace

// Wrapped in correct namespace.
namespace gfx {
// Namespaces within gfx namespace allowed.
namespace namespaceI {
class ClassB;
} // namespace namespaceI
struct StructC {};
char *VarD = MACRO_A;
typedef int typeE;
void funcF() {}
extern "C" void extern_funcJ() {}
} // namespace gfx

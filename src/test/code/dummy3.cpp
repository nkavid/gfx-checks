#include <iostream>

namespace gfx::cool::wow {
static const char *func() { return "func"; }
} // namespace gfx::cool::wow

int main() {
  std::cout << gfx::cool::wow::func() << '\n';

  return 0;
}

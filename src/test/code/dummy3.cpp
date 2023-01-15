#include <iostream>

namespace gfx::cool::wow
{
static const char* func()
{
  return "func";
}
}

int main()
{
  std::cout << gfx::cool::wow::func() << '\n';

  return 0;
}

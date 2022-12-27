#include <iostream>

class Foo {
public:
  int x() { return 5; }
  int y() { return a(); }
  int z() { return b() + a(); }
  int w() { return z() + a(); }

private:
  int a() { return 3; }
  int b() { return 4; }
  int c() { return a() + b(); }
};

int main() {
  Foo dummy{};

  std::cout << dummy.y() << '\n';
  std::cout << dummy.z() << '\n';
  std::cout << dummy.w() << '\n';

  return 0;
}

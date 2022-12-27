#include <iostream>

class Foo
{
public:
  int x() {return 5;}
  int y() {return a;}
  int z() {return b + a;}
private:
  int a{3};
  int b{4};
};

int i{8};

class Bar
{
  int a{1};
  int b{1};
};

int main()
{
  Foo dummy{};
  
  std::cout << dummy.x() << '\n';
  std::cout << dummy.y() << '\n';
  std::cout << dummy.z() << '\n';
  
  return 0;
}

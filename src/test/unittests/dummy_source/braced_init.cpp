int intVar1;
unsigned long intVar2 = 2;
unsigned long intVar3 = 2.0f;
float floatVar        = 0.8f;
int intVar4{};

class Foo
{
    Foo()                      = default;
    Foo(const Foo&)            = default;
    Foo& operator=(const Foo&) = default;
};

Foo foo1;
Foo foo2{};
Foo foo3 = Foo();
Foo foo4 = Foo{};

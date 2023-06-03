class Foo
{
  public:
    Foo() {}

    ~Foo() = default;

    int x();
    int y();
    int z();

  private:
    int _a{3};
    int _b{4};
};

int Foo::x()
{
  return 5;
}

int Foo::y()
{
  return _a;
}

int Foo::z()
{
  return _b + _a;
}

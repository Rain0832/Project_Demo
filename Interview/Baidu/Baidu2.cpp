#include <iostream>
#include <memory>

int main()
{
  std::shared_ptr<int> s1(new int(10));
  std::shared_ptr<int> s2 = s1;
  std::cout << "s1 use_count: " << s1.use_count() << std::endl;
  std::cout << "s2 use_count: " << s2.use_count() << std::endl;
  s1.reset();
  std::cout << "s2 use_count: " << s2.use_count() << std::endl;

  // int *point = new int(1);
  // // std::shared_ptr<int> a = std::shared_ptr<int>(point);
  // auto a = std::shared_ptr<int>(point);
  // std::cout << "a use_count: " << a.use_count() << std::endl;
  // // std::shared_ptr<int> b = a;
  // auto b = a;
  // std::cout << "a use_count: " << a.use_count() << std::endl;
  // std::cout << "b use_count: " << b.use_count() << std::endl;
  // const auto &c = b;
  // std::cout << "a use_count: " << a.use_count() << std::endl;
  // std::cout << "b use_count: " << b.use_count() << std::endl;
  // std::cout << "c use_count: " << c.use_count() << std::endl;
  return 0;
}

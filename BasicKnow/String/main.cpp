#include <cstring>
#include <array>

class String
{
public:
  String(String &rhs)
  {
    data_ = new char(rhs.len_ + 1);
    std::strcpy(data_, rhs.data_);
    len_ = rhs.len_;
  }

  String &operator=(String rhs)
  {
    swap(rhs);
    return *this;
  }

private:
  char *data_;
  int len_;

  void swap(String &rhs)
  {
    std::swap(data_, rhs.data_);
    std::swap(len_, rhs.len_);
  }
};
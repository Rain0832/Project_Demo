#include <iostream>
#include <cstring>

class String
{
public:
  char *data;

  String(const String &other)
  {
    data = new char[strlen(other.data) + 1];
    strcpy(data, other.data);
  }

  ~String()
  {
    delete[] data;
  }
};

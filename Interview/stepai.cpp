#include <iostream>
#include <vector>

using namespace std;

template <size_t Row, size_t Col, typename T = double>
class Matrix
{
private:
  vector<vector<T>> m_matrix;

public:
  Matrix() // 默认初始化
  {
    m_matrix.assign(Row, vector<T>(Col, T{}));
  }

  Matrix(std::initializer_list<T> list) // 按行初始化
  {
    m_matrix.assign(Row, vector<T>(Col, T{}));
    auto it = list.begin();
    for (int row = 0; row < Row; row++)
    {
      for (int col = 0; col < Col; col++)
      {
        m_matrix[row][col] = *it++;
      }
    }
  }

  T &operator()(size_t r, size_t c)
  {
    return m_matrix[r][c];
  }

  const T &operator()(size_t r, size_t c) const
  {
    return m_matrix[r][c];
  }

  Matrix<Row, Col, T> operator+(const Matrix<Row, Col, T> &other) const
  {
    Matrix<Row, Col, T> res_matrix;
    for (int row = 0; row < Row; row++)
    {
      for (int col = 0; col < Col; col++)
      {
        res_matrix.operator()(row, col) = m_matrix[row][col] + other.operator()(row, col);
      }
    }
    return res_matrix;
  }

  template <size_t OtherCol>
  Matrix<Row, OtherCol, T> operator*(const Matrix<Col, OtherCol, T> &other) const
  {
    Matrix<Row, OtherCol, T> res_matrix;
    for (int row = 0; row < Row; row++)
    {
      for (int col = 0; col < OtherCol; col++)
      {
        for (int incol = 0; incol < Col; incol++)
        {
          res_matrix.operator()(row, col) += m_matrix[row][incol] * other.operator()(incol, col);
        }
      }
    }
    return res_matrix;
  }

  void print() const
  {
    for (int row = 0; row < Row; row++)
    {
      for (int col = 0; col < Col; col++)
      {
        cout << this->operator()(row, col) << " ";
      }
      cout << endl;
    }
  }
};

// 示例：
int main()
{
  Matrix<2, 3> A{1, 2, 3,
                 4, 5, 6};

  Matrix<3, 2> B{1, 2,
                 3, 4,
                 5, 6};

  auto C = A * B;
  C.print();

  return 0;
}
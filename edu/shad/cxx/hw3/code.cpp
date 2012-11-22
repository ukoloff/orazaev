#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
using namespace std;
template <typename T>
class Array {
private:
  T * data;
  int size;
public:
  Array(int n, T * a): size(n), data(0) {
    data = new T[n];
    for (int i = 0; i < n; i++)
      data[i] = a[i];
  }
  Array(vector<T> v) {
    size = v.size();
    for (int i = 0; i < size; ++i)
      data[i] = v[i];
  }
  Array(list<T> l) {
    size = l.size();
    int i = 0;
    for (list<T>::iterator it = l.begin(); it != l.end(); it++)
      data[i++] = *it;
  }
  Array(const Array& other) {
    delete data;
    size = other.size();
    for (int i = 0; i < size; i++)
      data[i] = other[i];
  }
  T& operator [] (int i) {
    if (i > size) 
      exit(0);
    return data[i];
  }
  Array& operator += (Array& b) const {
    Array temp(*this);
    for (int i = 0; i < size(); i++)
      temp[i] += b[i];
    return temp;
  }
  int length() {
    return size();
  }
  virtual ~Array() {
    delete data;
  }
};

template <typename T>
ostream& operator << (ostream& out, Array<T> a) {
  for (int i = 0; i < a.length(); ++i)
    out << i << " ";
  return out;
}

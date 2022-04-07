#pragma once

#include <cstdlib>
#include <algorithm>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector()
  {
      data = nullptr;
      end_ = nullptr;
      fill_item_qty = 0;
  }
  explicit SimpleVector(size_t size)
  {
      data = new T[size];
      end_ = data + size;
      fill_item_qty = size;
  }
  ~SimpleVector()
  {
      delete[] data;
  }

  T& operator[](size_t index)
  {
      return data[index];
  }

  void operator=(const SimpleVector);

  T* begin()
  {
      return data;
  }
  T* end()
  {
      return data + fill_item_qty;
  }

  size_t Size() const
  {
      // returns quantity of filled in items of vectors
      return fill_item_qty;
  }
  size_t Capacity() const
  {
      // returns the posibile quantity of items of vector
      return end_ - data;
  }
  void PushBack(const T& value)
  {
      if (Size() == Capacity())
      {
          size_t offset = (fill_item_qty == 0) ? (1) : (Capacity() << 1);
          T* temp = new T[offset];
          for (size_t i = 0; i < fill_item_qty; i++)
          {
              temp[i] = data[i];
          }

          if (data != nullptr)
          {
              delete[] data;
          }
          data = temp;
          end_ = temp + offset;
      }

      data[fill_item_qty] = value;
      fill_item_qty++;
  }


private:
  // Добавьте поля для хранения данных вектора
    T* data;
    T* end_;
    // Quantity of filled in position of vector
    size_t fill_item_qty;
};

template<typename T>
void SimpleVector<T>::operator=(const SimpleVector<T> other)
{
    if (data == nullptr)
    {
        data = new T[other.Capacity()];
    }
    end_ = data + other.Capacity();
    fill_item_qty = other.Size();

    std::copy(other.begin(), other.end(), begin());
}


/*
 * For example, could been write implemetation like this:
template <typename T>
SimpleVector<T>::SimpleVector(size_t size)
  : data(new T[size])
  , size(size)
  , capacity(size)
{
}
*/


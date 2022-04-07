#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
    // Напишите реализацию функции,
    // не копируя сортируемые элементы
    const auto range_size = range_end - range_begin;
    if (range_size < 2)
    {
      return;
    }
    vector<typename RandomIt::value_type> values(make_move_iterator(range_begin), make_move_iterator(range_end));

    MergeSort(range_begin, range_begin + range_size/3);
    MergeSort(range_begin + range_size/3, ((range_begin + range_size/3) + range_size/3));
    MergeSort(((range_begin + range_size/3) + range_size/3), range_end);

    vector<typename RandomIt::value_type> interim_result;
    merge(make_move_iterator(range_begin), make_move_iterator(range_begin + range_size/3),
          make_move_iterator(range_begin + range_size/3), make_move_iterator(((range_begin + range_size/3) + range_size/3)),
          back_inserter(interim_result));

    merge(make_move_iterator(interim_result.begin()), make_move_iterator(interim_result.end()),
          make_move_iterator(((range_begin + range_size/3) + range_size/3)), make_move_iterator(range_end),
          range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}

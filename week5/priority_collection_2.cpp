#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <list>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using Id = size_t/* тип, используемый для идентификаторов */;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        values.push_back({move(object), INITIAL_PRIORITY});
        sorted_values.insert({INITIAL_PRIORITY, max_id});
        max_id++;
        return max_id - 1;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin)
    {
        auto it = range_begin;
        auto ids_it = ids_begin;
        for (; it != range_end; it++, ids_it++) {
            values.push_back({move(*it), INITIAL_PRIORITY});
            sorted_values.insert({INITIAL_PRIORITY, max_id});
            max_id++;
            *ids_it = max_id - 1;
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const
    {
        return ((id < values.size()) && (-1 != values[id].second)) ? true : false;
    }

    // Получить объект по идентификатору
    const T &Get(Id id) const
    {
        return values[id].first;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id)
    {
        size_t old_prior = values[id].second;
        values[id].second++;
        sorted_values.erase({old_prior, id});
        sorted_values.insert({old_prior + 1, id});
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T &, int> GetMax() const
    {
        return values[prev(sorted_values.end())->second];
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax()
    {
        auto it = prev(sorted_values.end());
        auto id = it->second;
        auto prior = it->first;
        //auto res = move(values[id]);
        sorted_values.erase(it);
        values[id].second = -1;
        return make_pair(move(values[id].first), prior);
    }

private:
    // Приватные поля и методы
    // id == object
    // id -> prior
    // prior -> id
    const uint8_t INITIAL_PRIORITY = 0;

    set<pair<int, Id>> sorted_values;
    vector<pair<T, int>> values;
    size_t max_id = 0;
};

class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}

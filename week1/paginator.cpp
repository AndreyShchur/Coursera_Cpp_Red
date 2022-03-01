#include "test_runner.h"

#include <numeric>
#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

// Реализуйте шаблон класса Paginator
template <typename It>
class IteratorRange {
public:
    IteratorRange(It f, It l) : first(f), last(l)
    {
    }

    size_t size() const
    {
        return distance(first, last);
    }

    It begin() const
    {
        return first;
    }
    It end() const
    {
        return last;
    }

private:
    It first, last;
};

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator first, Iterator last, size_t page_size)
    {
        size_t container_size = distance(first, last);
        size_t page_qty;
        if ((container_size > page_size) && (page_size != 0))
        {
            if (0U == (container_size % page_size))
            {
                page_qty = container_size/page_size;
            }
            else
            {
                page_qty = (container_size/page_size) + 1;
            }

            Iterator left_it;
            for (size_t i = 0; i < page_qty; i++)
            {
                left_it = next(first, page_size * i);
                size_t it_dist = distance(left_it, last);
                res.push_back(IteratorRange(left_it, next(left_it, min(page_size, it_dist))));
            }
        }
        else if (container_size != 0U)
        {
            res.push_back(IteratorRange(first, last));
        }
    }

    size_t size() const
    {
        return res.size();
    }

    auto begin() const
    {
        return res.begin();
    }

    auto end() const
    {
        return res.end();
    }

private:
    vector<IteratorRange<Iterator>> res;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(c.begin(), c.end(), page_size);
}

void TestPageCounts() {
  vector<int> v(15);

  ASSERT_EQUAL(Paginate(v, 1).size(), v.size());
  ASSERT_EQUAL(Paginate(v, 3).size(), 5u);
  ASSERT_EQUAL(Paginate(v, 0).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 2).size(), 8u);
  ASSERT_EQUAL(Paginate(v, 5).size(), 3u);
  ASSERT_EQUAL(Paginate(v, 4).size(), 4u);
  ASSERT_EQUAL(Paginate(v, 15).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 16).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 150).size(), 1u);
  ASSERT_EQUAL(Paginate(v, 14).size(), 2u);
  //ASSERT_EQUAL(Paginate(v, 14).size(), 108u);

  vector<int> empty_vec;
    ASSERT_EQUAL(Paginate(empty_vec, 1).size(), 0u);
}

void TestLooping() {
  vector<int> v(15);
  iota(begin(v), end(v), 1);

  Paginator<vector<int>::iterator> paginate_v(v.begin(), v.end(), 6);
  ostringstream os;
  for (const auto& page : paginate_v) {
    for (int x : page) {
      os << x << ' ';
    }
    os << '\n';
  }

  ASSERT_EQUAL(os.str(), "1 2 3 4 5 6 \n7 8 9 10 11 12 \n13 14 15 \n");
}

void TestModification() {
  vector<string> vs = {"one", "two", "three", "four", "five"};
  for (auto page : Paginate(vs, 2)) {
    for (auto& word : page) {
      word[0] = toupper(word[0]);
    }
  }

  const vector<string> expected = {"One", "Two", "Three", "Four", "Five"};
  ASSERT_EQUAL(vs, expected);
}

void TestPageSizes() {
  string letters(26, ' ');

  Paginator letters_pagination(letters.begin(), letters.end(), 11);
  vector<size_t> page_sizes;
  for (const auto& page : letters_pagination) {
    page_sizes.push_back(page.size());
  }

  const vector<size_t> expected = {11, 11, 4};
  ASSERT_EQUAL(page_sizes, expected);
}

void TestConstContainer() {
  const string letters = "abcdefghijklmnopqrstuvwxyz";

  vector<string> pages;
  for (const auto& page : Paginate(letters, 10)) {
    pages.push_back(string(page.begin(), page.end()));
  }

  const vector<string> expected = {"abcdefghij", "klmnopqrst", "uvwxyz"};
  ASSERT_EQUAL(pages, expected);
}

void TestPagePagination() {
  vector<int> v(22);
  iota(begin(v), end(v), 1);

  vector<vector<vector<int>>> lines;
  for (const auto& split_by_9 : Paginate(v, 9)) {
    for (const auto& split_by_4 : Paginate(split_by_9, 4)) {
        lines.push_back({});
    for (const auto& split_by_2 : Paginate(split_by_4, 2)) {
        lines.back().push_back({});
        for (int item: split_by_2) {
            lines.back().back().push_back(item);
        }
    }
    }
  }

    const vector<vector<vector<int>>> expected = {
            {{1, 2},  {3, 4}},
            {{5, 6},  {7, 8}},
            {{9}},
            {{10, 11}, {12, 13}},
            {{14, 15}, {16, 17}},
             {{18}},
            {{19, 20}, {21, 22}}
    };

  ASSERT_EQUAL(lines, expected);
}

void TestSet()
{
    const set<int> letters = {1,2,3,3,4,6,6,7,8,9,11,7,5,1,4};

    vector<set<int>> pages;
    for (const auto& page : Paginate(letters, 4)) {
        pages.push_back(set(page.begin(), page.end()));
    }

    const vector<set<int>> expected = {{1,2,3,4},{5,6,7,8},{9,11}};
    ASSERT_EQUAL(pages, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestPageCounts);
  RUN_TEST(tr, TestLooping);
  RUN_TEST(tr, TestModification);
  RUN_TEST(tr, TestPageSizes);
  RUN_TEST(tr, TestConstContainer);
  RUN_TEST(tr, TestPagePagination);
  RUN_TEST(tr, TestSet);
}


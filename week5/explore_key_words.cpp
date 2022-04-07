#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
#include <functional>
#include <sstream>

using namespace std;

#define THREAD_QTY      (4)

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other)
  {
      for (const auto &item : other.word_frequences)
      {
          this->word_frequences[item.first] += item.second;
      }
  }
};

Stats FindKeysInLine(const set<string>& keys, vector<string>::iterator begin, vector<string>::iterator end)
{
    Stats result;
    size_t pos;
    while (begin != end)
    {
        auto line = move(*begin);
        while ((pos = line.find(' ')) != line.npos)
        {
            string token = line.substr(0, pos);
            if (keys.find(token) != keys.end())
            {
                result.word_frequences[token]++;
            }
            line.erase(0, pos + 1);
        }
        if (keys.find(line) != keys.end())
        {
            result.word_frequences[line]++;
        }
        begin++;
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  // Implement this function
    Stats result;
    string line;
    vector<string> lines;
    vector<future<Stats>> futures;
    while (getline(input, line))
    {
        lines.push_back(move(line));
    }

    size_t lines_qty;
    if (lines.size() % THREAD_QTY == 0)
    {
        lines_qty = lines.size() / THREAD_QTY;
    }
    else
    {
        lines_qty = lines.size() / THREAD_QTY + 1;
    }

    for (int thread_num = 0; thread_num < THREAD_QTY; thread_num++)
    {
        if (lines.size() < lines_qty * thread_num)
        {
            break;
        }
        auto begin = (lines.begin() + (lines_qty * thread_num));
        auto last_valid_item_it = lines.end() - lines_qty + (lines_qty - 1);
        auto end = (last_valid_item_it < begin + lines_qty - 1)
                ? lines.end()
                : begin + lines_qty;
        futures.push_back(
                async(FindKeysInLine,
                      ref(key_words),
                      begin,
                      end
                      ));
    }

    for (auto &f : futures)
    {
        result += f.get();
    }
    return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
  //ss << "Goondex really sucks, but yangle rocks. Use yangle 1\n";
  //ss << "Goondex really sucks, but yangle rocks. Use yangle 2\n";
  //ss << "Goondex really sucks, but yangle rocks. Use yangle 3\n";
  //ss << "Goondex really sucks, but yangle rocks. Use yangle 4\n";
  //ss << "Goondex really sucks, but yangle rocks. Use yangle 5\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}

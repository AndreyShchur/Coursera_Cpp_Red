#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
public:
    ReadingManager()
            : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
              page_user_count_(MAX_PAGE_COUNT_ + 1, 0),
              max_page_(0),
              user_qty_(0)
               {}

    void Read(int user_id, int page_count)
    {
        if (user_page_counts_[user_id] != 0)
        {
            page_user_count_[user_page_counts_[user_id]]--;
        }
        else
        {
            user_qty_++;
        }
        page_user_count_[page_count]++;
        user_page_counts_[user_id] = page_count;
        if (page_count > max_page_)
        {
            max_page_ = page_count;
        }

    }

    double Cheer(int user_id) const
    {
        if (user_page_counts_[user_id] == 0)
        {
            return 0;
        }
        if (user_qty_ == 1)
        {
            return 1;
        }
        int page_count = user_page_counts_[user_id];
        int position = 0;
        if (page_count == max_page_)
        {
            position = page_user_count_[page_count];
        }
        else
        {
            while (position < user_qty_ && page_count != max_page_ + 1)
            {
                position += page_user_count_[page_count];
                page_count++;
            }
        }
        if (position == user_qty_)
        {
            return 0;
        }
        // По умолчанию деление целочисленное, поэтому
        // нужно привести числитель к типу double.
        // Простой способ сделать это — умножить его на 1.0.
        return (user_qty_ - position) * 1.0 / (user_qty_ - 1);
    }

private:
    // Статическое поле не принадлежит какому-то конкретному
    // объекту класса. По сути это глобальная переменная,
    // в данном случае константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи
    // следующим образом: ReadingManager::MAX_USER_COUNT.
    static const int MAX_USER_COUNT_ = 100'000;
    static const int MAX_PAGE_COUNT_ = 1000;

    vector<int> user_page_counts_;
    vector<int> page_user_count_;   // отсортированы по убыванию количества страниц
    int max_page_;
    int user_qty_;

    //int GetUserCount() const {
    //    return sorted_users_.size();
    //}
    //void SwapUsers(int lhs_position, int rhs_position) {
    //    const int lhs_id = sorted_users_[lhs_position];
    //    const int rhs_id = sorted_users_[rhs_position];
    //    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
    //    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
    //}
};


int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    //ios::sync_with_stdio(false);
    //cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }
    return 0;
}
#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <set>
#include <deque>

using namespace std;

class BookingManager {
public:
    BookingManager()
    {}

    void Book(const long long &time,
              const string &hotel_name,
              const int &user_id,
              const int &room_count)
    {
        int hotel_idx = GetHotelIndex(hotel_name);
        current_time_ = time; // 6
        const int diff = current_time_ - SEC_IN_DAY_; // 4
        for (int i = 0; i < time_range_.size(); i++)
        {
            int diff_in_range = time_range_[i].front() - SEC_IN_DAY_;
            while (diff_in_range > 0 && diff_in_range )
            {

            }
        }

    }

    int Clients(const string &hotel_name) const
    {
        return 0;
    }

    int Rooms(const string &hotel_name) const
    {
        return 0;
    }

private:
    static const int MAX_CLIENT_COUNT_ = 1000'000'000;
    static const int SEC_IN_DAY_ = 86400;

    map<string, int> hotel_idx_;
    vector<deque<long long>> time_range_;
    set<deque<pair<int, int>>> clients_and_rooms_;
    long long current_time_ = 0;
    int last_hotel_idx_ = 0;

    int GetHotelIndex(const string &hotel_name)
    {
        auto it = hotel_idx_.find(hotel_name);
        int hotel_idx;
        if (it == hotel_idx_.end())
        {
            hotel_idx_[hotel_name] = last_hotel_idx_;
            last_hotel_idx_++;
            hotel_idx = last_hotel_idx_;
        }
        else
        {
            hotel_idx = it->second;
        }
        return hotel_idx;
    }
};


int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    BookingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id)
    {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK")
        {
            long long time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int user_id;
            cin >> user_id;
            int room_count;
            cin >> room_count;
            manager.Book(time, hotel_name, user_id, room_count);
        }
        else if (query_type == "CLIENTS")
        {
            string hotel_name;
            cin >> hotel_name;
            cout << manager.Clients(hotel_name) << "\n";
        }
        else if (query_type == "ROOMS")
        {
            string hotel_name;
            cin >> hotel_name;
            cout << manager.Rooms(hotel_name) << "\n";
        }
    }
    return 0;
}
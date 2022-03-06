#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <set>
#include <deque>
#include <queue>

#define MY_DEBUG (0)

#if (MY_DEBUG)
#include "test_runner.h"
#endif

using namespace std;

class BookingManager {
public:
    BookingManager()
    : lastHotelIndex(0), current_time_(0)
    {}

    void Book(const long long &time,
              const string &hotel_name,
              const int &user_id,
              const int &room_count)
    {
        auto it = hotelIndexes.find(hotel_name);
        if (it == hotelIndexes.end())
        {
            hotelIndexes[hotel_name] = lastHotelIndex;
            timeUserIdRoomsQtyDataBase.push_back({{time, {user_id, room_count}}});
            roomsQty.push_back(room_count);
            clientIdToOrderCount.push_back({{user_id, 1}});
            lastHotelIndex++;
        }
        else
        {
            timeUserIdRoomsQtyDataBase[it->second].push_back({time, {user_id, room_count}});
            roomsQty[it->second] += room_count;
            if (clientIdToOrderCount[it->second].count(user_id) == 0)
            {
                clientIdToOrderCount[it->second][user_id] = 1;
            }
            else
            {
                ++clientIdToOrderCount[it->second][user_id];
            }
        }

        current_time_ = time;
        orderIsSet = true;
    }

    unsigned long Clients(const string &hotel_name)
    {
        const int hotel_idx = GetHotelIndex(hotel_name);
        if (NO_HOTEL_ != hotel_idx)
        {
            UpdateBookingQueue(hotel_idx);
            return clientIdToOrderCount[hotel_idx].size();
        }
        else
        {
            return 0;
        }
    }

    unsigned long Rooms(const string &hotel_name)
    {
        const int hotel_idx = GetHotelIndex(hotel_name);
        if (NO_HOTEL_ != hotel_idx)
        {
            UpdateBookingQueue(hotel_idx);
            return roomsQty[hotel_idx];
        }
        else
        {
            return 0;
        }
    }

private:
    static const int SEC_IN_DAY_ = 86400;
    static const int NO_HOTEL_ = -1;

    // Indexes of hotel
    map<string, int> hotelIndexes;
    // Map of clients ID to rooms and order quantity
    vector<map<int, int>> clientIdToOrderCount;
    // Queue. Time slot and user ID
    vector<deque<pair<long long, pair<int, int>>>> timeUserIdRoomsQtyDataBase;
    // Quantity of clients and rooms for each hotel
    vector<unsigned long> roomsQty;
    // Actual time slot
    long long current_time_;
    // Actual hotel index
    int lastHotelIndex;

    bool orderIsSet = false;



    void UpdateBookingQueue(const int& hotelId)
    {
        const long long time_diff = current_time_ - SEC_IN_DAY_;

        auto &hotel = timeUserIdRoomsQtyDataBase[hotelId];

        // Check for removing
        for (auto &order : hotel)
        {
            if (order.first <= time_diff)
            {
                auto &user_id_for_remove = hotel.front().second.first;

                // Change quantity of clients and rooms
                auto &remove_item = clientIdToOrderCount[hotelId][user_id_for_remove];
                if (remove_item == 1)
                {
                   clientIdToOrderCount[hotelId].erase(user_id_for_remove);
                }
                else
                {
                   remove_item--;
                }

                roomsQty[hotelId] -= order.second.second;

                hotel.pop_front();
            }
            else
            {
                break;
            }
        }
    }


    int GetHotelIndex(const string &hotel_name)
    {
        auto it = hotelIndexes.find(hotel_name);
        if (it != hotelIndexes.end())
        {
            return it->second;
        }
        else
        {
            return NO_HOTEL_;
        }
    }

};

#if (MY_DEBUG)
void SimpleTest(void)
{
    BookingManager manager;

    manager.Book(10, "hotel1", 1, 1);
    manager.Book(11, "hotel1", 1, 10);
    manager.Book(100000, "hotel1", 1, 11);
    ASSERT_EQUAL(manager.Clients("hotel1"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 11);
}

void SameUserIdTest(void)
{
    BookingManager manager;

    manager.Book(10, "hotel1", 0, 1);
    manager.Book(10, "hotel1", 1, 10);
    auto res = manager.Clients("hotel1");
    ASSERT_EQUAL(res, 2);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 11);
    manager.Book(1000, "hotel1", 1, 200);
    manager.Book(12000, "hotel1", 10, 2);
    ASSERT_EQUAL(manager.Clients("hotel1"), 3);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 213);
    manager.Book(13000, "hotel1", 1, 1);
    ASSERT_EQUAL(manager.Clients("hotel1"), 3);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 214);
}

void MultipleHotel(void)
{
    BookingManager manager;

    manager.Book(10, "hotel1", 0, 1);
    manager.Book(10, "hotel3", 1, 10);
    manager.Book(1000, "hotel3", 1, 11);
    manager.Book(12000, "hotel4", 10, 2);
    ASSERT_EQUAL(manager.Clients("hotel1"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 1);
    ASSERT_EQUAL(manager.Clients("hotel3"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel3"), 21);
    ASSERT_EQUAL(manager.Clients("hotel4"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel4"), 2);
    ASSERT_EQUAL(manager.Clients("hotel5"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel5"), 0);
    manager.Book(12000000, "hotel1", 1, 26);
    ASSERT_EQUAL(manager.Clients("hotel1"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 26);
    ASSERT_EQUAL(manager.Clients("hotel3"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel3"), 0);
    ASSERT_EQUAL(manager.Clients("hotel4"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel4"), 0);
}

void MoreThenOneDay(void)
{
    BookingManager manager;

    manager.Book(10, "hotel1", 0, 1);
    manager.Book(10000, "hotel2", 1, 10);
    manager.Book(86409, "hotel1", 1, 10);
    ASSERT_EQUAL(manager.Clients("hotel1"), 2);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 11);
    manager.Book(86410, "hotel2", 1 , 20);
    ASSERT_EQUAL(manager.Clients("hotel1"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 10);
    manager.Book(400000, "hotel3", 3 , 20);
    ASSERT_EQUAL(manager.Clients("hotel1"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 0);
    auto res = manager.Clients("hotel2");
    ASSERT_EQUAL(res, 0);
    ASSERT_EQUAL(manager.Rooms("hotel2"), 0);
    ASSERT_EQUAL(manager.Clients("hotel3"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel3"), 20);
}

void TwoRemove(void)
{
    BookingManager manager;

    manager.Book(10, "hotel1", 0, 1);
    manager.Book(11, "hotel1", 1, 10);
    manager.Book(1000, "hotel1", 2, 3);
    manager.Book(12000, "hotel1", 3, 2);
    ASSERT_EQUAL(manager.Clients("hotel1"), 4);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 16);
    manager.Book(87399, "hotel1", 3, 5);
    ASSERT_EQUAL(manager.Clients("hotel1"), 2);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 10);
}

void AddClientAfterDelete(void)
{
    BookingManager manager;

    manager.Book(10, "hotel1", 0, 1);
    manager.Book(11, "hotel1", 1, 10);
    ASSERT_EQUAL(manager.Clients("hotel1"), 2);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 11);
    manager.Book(100000, "hotel1", 2, 3);
    ASSERT_EQUAL(manager.Clients("hotel1"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 3);
    manager.Book(120000, "hotel1", 0, 2);
    ASSERT_EQUAL(manager.Clients("hotel1"), 2);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 5);
}

void AllDelete(void)
{
    BookingManager manager;

    manager.Book(10, "hotel1", 0, 1);
    manager.Book(11, "hotel2", 1, 10);
    manager.Book(1000, "hotel3", 1, 11);
    manager.Book(13000, "hotel4", 10, 2);
    manager.Book(120000000, "hotel2", 1, 1);

    ASSERT_EQUAL(manager.Clients("hotel1"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 0);
    ASSERT_EQUAL(manager.Clients("hotel3"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel3"), 0);
    ASSERT_EQUAL(manager.Clients("hotel4"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel4"), 0);
    auto res = manager.Clients("hotel2");
    ASSERT_EQUAL(res, 1);
    ASSERT_EQUAL(manager.Rooms("hotel2"), 1);
}

void EmptyHotel(void)
{
    BookingManager manager;

    ASSERT_EQUAL(manager.Clients("hotel1"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 0);
}

void BigNumber(void)
{
    BookingManager manager;

    manager.Book(10, "hotel1", 1000000000, 1000);
    ASSERT_EQUAL(manager.Clients("hotel1"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 1000);
    ASSERT_EQUAL(manager.Clients("hotel2"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel2"), 0);
    manager.Book(1000000000000000000, "hotel2", 1000000000, 999);
    ASSERT_EQUAL(manager.Clients("hotel1"), 0);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 0);
    ASSERT_EQUAL(manager.Clients("hotel2"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel2"), 999);
}

void LessZeroNumber(void)
{
    BookingManager manager;

    manager.Book(-86400, "hotel1", 2, 2);
    ASSERT_EQUAL(manager.Clients("hotel1"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 2);
    manager.Book(-400, "hotel1", 3, 10);
    ASSERT_EQUAL(manager.Clients("hotel1"), 2);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 12);
    manager.Book(0, "hotel1", 1, 20);
    ASSERT_EQUAL(manager.Clients("hotel1"), 2);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 30);
}

void MaxRooms(void)
{
    BookingManager manager;

    for (int i = 0; i < 86400; i++)
    {
        manager.Book(i, "hotel1", i, 1000);
    }
    ASSERT_EQUAL(manager.Clients("hotel1"), 86400);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 86400000);
    manager.Book(86400, "hotel1", 86400, 1000);
    ASSERT_EQUAL(manager.Clients("hotel1"), 86400);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 86400000);
}

void TreeOrder(void)
{
    BookingManager manager;

    manager.Book(0, "hotel1", 1, 1);
    manager.Book(1, "hotel1", 1, 1);
    manager.Book(2, "hotel1", 1, 1);
    manager.Book(86400, "hotel1", 1, 1);
    ASSERT_EQUAL(manager.Clients("hotel1"), 1);
    ASSERT_EQUAL(manager.Rooms("hotel1"), 3);
}
#endif

int main() {

   #if (!MY_DEBUG)
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
#else
    TestRunner tr;
    RUN_TEST(tr, SimpleTest);
    RUN_TEST(tr, SameUserIdTest);
    RUN_TEST(tr, MultipleHotel);
    RUN_TEST(tr, MoreThenOneDay);
    RUN_TEST(tr, TwoRemove);
    RUN_TEST(tr, AddClientAfterDelete);
    RUN_TEST(tr, AllDelete);
    RUN_TEST(tr, EmptyHotel);
    RUN_TEST(tr, BigNumber);
    RUN_TEST(tr, LessZeroNumber);
    RUN_TEST(tr, MaxRooms);
    RUN_TEST(tr, TreeOrder);
    return 0;
#endif
}

#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

template <typename T>
class Deque
{
public:
    Deque()
    {    }

    bool Empty() const
    {
        if (front.empty() && back.empty())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t Size() const
    {
        return (front.size() + back.size());
    }

    T& operator[](size_t index)
    {
        if (index >= front.size())
        {
            return back[index - front.size()];
        }
        else
        {
            return front[front.size() - index - 1];
        }
    }
    const T& operator[](size_t index) const
    {

        if (index >= front.size())
        {
            return back[index - front.size()];
        }
        else
        {
            return front[front.size() - index - 1];
        }
    }

    const T& At(size_t index) const
    {
        if (index >= front.size())
        {
            if (index >= back.size() + front.size())
            {
                throw out_of_range("");
            }
            else
            {
                return back.at(index - front.size());
            }
        }
        else
        {
            return front.at(front.size() - index - 1);
        }
    }

    T& At(size_t index)
    {
        if (index >= front.size())
        {
            if (index >= back.size() + front.size())
            {
                throw out_of_range("");
            }
            else
            {
                return back.at(index - front.size());
            }
        }
        else
        {
            return front.at(front.size() - index - 1);
        }
    }

    const T& Front(void) const
    {
        if (!front.empty())
            return front.back();
        else
            return back.front();
    }
    T& Front(void)
    {
        if (!front.empty())
            return front.back();
        else
            return back.front();
    }

    const T& Back(void) const
    {
        if (!back.empty())
            return back.back();
        else
            return front.front();
    }
    T& Back(void)
    {
        if (!back.empty())
            return back.back();
        else
            return front.front();
    }

    void PushFront(T value)
    {
        front.push_back(value);
    }
    void PushBack(T value)
    {
        back.push_back(value);
    }

private:
    vector<T> front;
    vector<T> back;
};


int main() {
    Deque<int> a;
    cout << a.Empty();
    //a.PushBack(4);
    a.PushFront(1);
    a.PushFront(2);
    a.PushFront(3);
    //a.PushBack(5);
    cout << a.Empty() << a.Back() << endl;
    cout << a[1] << a[0] << a[2] << endl;
    cout << a.At(1) << a.At(0) << a.At(1);
    cout << a.Front() << a.Back();

    Deque<vector<int>> b;

    return 0;
}


#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate()
  {
      if (deallocObjects_.empty())
      {
          auto p = busyObjects_.insert(new T);
          return *(p.first);
      }
      else
      {
          const auto pObj = deallocObjects_.front();
          deallocObjects_.pop();
          UpdateSets(pObj);
          return pObj;
      }
  }

  T* TryAllocate()
  {
      if (deallocObjects_.empty())
      {
          return nullptr;
      }
      else
      {
          const auto pObj = deallocObjects_.front();
          deallocObjects_.pop();
          UpdateSets(pObj);
          return pObj;
      }
  }

  void Deallocate(T* object)
  {
      auto it = busyObjects_.find(object);
      if (it != busyObjects_.end())
      {
          busyObjects_.erase(object);
          freeObjects_.insert(object);
          deallocObjects_.push(object);
      }
      else
      {
          throw invalid_argument("invalid object!");
      }
  }

  ~ObjectPool()
  {
      for (auto &obj : busyObjects_)
      {
          delete obj;
      }
      for (auto &obj : freeObjects_)
      {
          delete obj;
      }
  }

private:
  // Добавьте сюда поля
  set<T*> freeObjects_;
  set<T*> busyObjects_;
  queue<T*> deallocObjects_;

  void UpdateSets(T* pObj)
  {
      auto it = freeObjects_.find(pObj);
      if (it != freeObjects_.end())
      {
          busyObjects_.insert(*it);
          freeObjects_.erase(pObj);
      }
      else
      {
          throw runtime_error("Where the data?? hmmm....");
      }
  }
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  //string* a = new string;

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  //pool.Deallocate(a);

  pool.Deallocate(p2);
  auto r = pool.Allocate();
  ASSERT_EQUAL(*r, "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  r = pool.Allocate();
  ASSERT_EQUAL(*r, "third");
  r = pool.Allocate();
  ASSERT_EQUAL(*r, "first");

  //r = pool.TryAllocate();
  //ASSERT_EQUAL(*r, nullptr);

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}

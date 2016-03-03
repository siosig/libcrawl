#pragma once
#include <map>
#include <thread>
#include <functional>
#include <future>

/*
class T1 : public crawl::thread<int> {
public:
  T1(){}
  T1(int _i)
    : i(_i)
  {
  }
protected:
  int run() {
    return i;
  }
private:
  int i;

};

int main()
{
  crawl::thread_group<int> grp;
  grp.add(new T1(1));
  grp.add(new T1(2));
  grp.start();
  grp.join();
  BOOST_FOREACH(crawl::thread<int> *t, grp.list()) {
    std::cout << t->get() << std::endl;
  }
}
*/

namespace crawl {

  template <typename R>
  class thread {
  public:
    thread()
      : t(NULL)
      , callback(std::bind(&thread::noop, this, std::placeholders::_1, std::placeholders::_2))
      , id(0)
    {
    }

    virtual ~thread() {
      if(t) {
        delete t;
        t = NULL;
      }
    }

    void start() {
      t = new std::thread(std::bind(&thread::_run, this, std::ref(p)));
    }

    void join() const {
      if (t) {
        t->join();
      }
    }

    R get() {
      std::future<R> f = p.get_future();
      return f.get();
    }

    void setId(uint32_t _id) {
      id = _id;
    }

    void setCallbackFunction(std::function<void (uint32_t, const R&)> &f) {
      callback = f;
    }
  protected:
    virtual R run(){}

  private:
    void _run(std::promise<R> &p) {
      R ret = run();
      p.set_value(ret);
      callback(id, ret);
    }
    void noop(uint32_t id, const R&) {
    }
    uint32_t id;
    std::promise<R> p;
    std::thread *t;
    std::function<void (uint32_t, const R&)> callback;
  };

  template <typename R>
  class thread_group{
  public:
    thread_group() : cnt(0) {}
    virtual ~thread_group() {
      for (typename std::map<uint32_t, thread<R>*>::iterator it = th_map.begin(); it != th_map.end(); it++) {
        delete it->second;
      }
    }

    uint32_t add(thread<R> *th, bool start = false) {
      // XXX: ださい
      th->setId(++cnt);
      th_map.insert(std::make_pair(cnt, th));
      if (start) {
        th->start();
      }
      return cnt;
    }

    void start() const {
      for (typename std::map<uint32_t, thread<R>*>::iterator it = th_map.begin(); it != th_map.end(); it++) {
        it->second->start();
      }
    }

    void join() {
      for (typename std::map<uint32_t, thread<R>*>::iterator it = th_map.begin(); it != th_map.end(); it++) {
        it->second->join();
      }
    }

    thread<R>* get(uint32_t id) {
      return th_map[id];
    }

    const std::map<uint32_t, thread<R>* > &map() const {
      return th_map;
    }

    void clear() {
      for (typename std::map<uint32_t, thread<R>*>::iterator it = th_map.begin(); it != th_map.end(); it++) {
        delete it->second;
      }
      th_map.clear();
      cnt = 0;
    }

  private:
    std::map<uint32_t, thread<R>* > th_map;
    std::function<void (uint32_t, const R&)> callback;
    uint32_t cnt;
  };
}

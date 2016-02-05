#pragma once
#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <map>

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
      t = new boost::thread(boost::bind(&thread::_run, this, boost::ref(p)));
    }

    void join() {
      if (t) {
        t->join();
      }
    }

    R get() {
      boost::unique_future<R> f = p.get_future();
      return f.get();
    }

    void setId(uint32_t _id) {
      id = _id;
    }

    void setCallbackFunction(boost::function<void (uint32_t, const R&)> &f) {
      callback = f;
    }
  protected:
    virtual R run(){}

  private:
    void _run(boost::promise<R> &p) {
      std::string ret = run();
      p.set_value(ret);
      if (!callback.empty()) {
        callback(id, ret);
      }
    }
    void noop(R) {
    }
    uint32_t id;
    boost::promise<R> p;
    boost::thread *t;
    boost::function<void (uint32_t, const R&)> callback;
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

    void start() {
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
    boost::function<void (uint32_t, const R&)> callback;
    uint32_t cnt;
  };
}

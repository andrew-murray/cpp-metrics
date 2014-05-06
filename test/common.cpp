#include "common.hpp"

namespace mock {

  static std::atomic<int> g_time;

  mock::clock::time_point clock::now() {
      return time_point(duration((int)g_time));
  }

  void clock::set_time(const int& val) {
  	g_time = val;
  }

  int clock::time(){
  	return (int)g_time;
  }
}
#pragma once
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <boost/test/unit_test.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include "snapshot.hpp"

namespace bacc = boost::accumulators;

inline void test_snapshot(const std::vector<int>& vec, const metrics::snapshot& snap){
	int min = *std::min_element(vec.begin(),vec.end());
	int max = *std::max_element(vec.begin(),vec.end());

	BOOST_CHECK_EQUAL(min,snap.min());
	BOOST_CHECK_EQUAL(max,snap.max());

	double total = std::accumulate(vec.begin(),vec.end(),0.0);
	BOOST_CHECK_CLOSE(total/vec.size(),snap.mean(),0.0001);

	bacc::accumulator_set<double, bacc::stats<bacc::tag::variance> > acc;
  	for_each(vec.begin(), vec.end(), std::bind<void>(std::ref(acc), std::placeholders::_1));

  	BOOST_CHECK_CLOSE(bacc::mean(acc),snap.mean(),1e-6);
  	// TODO : Evaluate how to allow sample stats in std dev nicely
  	if(vec.size() > 1){
  		BOOST_CHECK_CLOSE(sqrt((bacc::variance(acc)) * vec.size() / (vec.size() - 1)),snap.std_dev(),1e-6);
  	} else {
  		BOOST_CHECK_CLOSE(0.0,snap.std_dev(),1e-6);
  	}	
}

namespace mock {
  class clock {
  public:
    typedef std::chrono::seconds duration;
    typedef std::chrono::time_point<clock> time_point;
    static time_point now();
    static void set_time(const int& val);
    static int time();
  };
}
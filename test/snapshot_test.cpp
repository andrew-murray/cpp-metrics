#include <thread>
#include <algorithm>
#include <cmath>
#include <boost/test/unit_test.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include "snapshot.hpp"

namespace bacc = boost::accumulators;

BOOST_AUTO_TEST_CASE(snapshot_test){
	std::vector<int> vec(17);
	for(int i = 0; i < vec.size(); ++i){
		vec[i] = (32-i)*(i+18) + 2;
	}

	snapshot snap(vec);

	int min = *std::min_element(vec.begin(),vec.end());
	int max = *std::max_element(vec.begin(),vec.end());

	BOOST_CHECK_EQUAL(min,snap.min());
	BOOST_CHECK_EQUAL(max,snap.max());

	double total = std::accumulate(vec.begin(),vec.end(),0.0);
	BOOST_CHECK_CLOSE(total/vec.size(),snap.mean(),0.0001);

	bacc::accumulator_set<double, bacc::stats<bacc::tag::variance> > acc;
  	for_each(vec.begin(), vec.end(), std::bind<void>(std::ref(acc), std::placeholders::_1));

  	BOOST_CHECK_EQUAL(bacc::mean(acc),snap.mean());
  	// This test fails - I calculate a sample std-dev
  	// while boost calculates a population std-dev
  	BOOST_CHECK_EQUAL(sqrt((bacc::variance(acc)) * vec.size() / (vec.size() - 1)),snap.std_dev());	
}
#include <thread>
#include <boost/test/unit_test.hpp>
#include "snapshot.hpp"

BOOST_AUTO_TEST_CASE(snapshot_test){
	std::vector<int> vec(9);
	for(int i = 0; i < vec.size(); ++i){
		vec[i] = i*i + 2;
	}
	snapshot snap(vec);
}
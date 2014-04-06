#include "common.hpp"

BOOST_AUTO_TEST_CASE(snapshot_test){
	std::vector<int> vec(17);
	for(int i = 0; i < vec.size(); ++i){
		vec[i] = (32-i)*(i+18) + 2;
	}

	snapshot snap(vec);

	test_snapshot(vec,snap);
}
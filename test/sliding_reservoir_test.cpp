#include "sliding_window_reservoir.hpp"
#include "common.hpp"

BOOST_AUTO_TEST_CASE(swr_test){
	sliding_window_reservoir res(5);

	std::vector<int> vec(21);

	// Non-deterministic test - always a bad thing ?
	// probably ~ should refactor this
	// but it's a good test without that

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

	for(int i = 0; i < vec.size(); ++i){
		vec[i] = dis(gen);
	}

	res.mark(vec[0]);

	for(int i = 1; i < vec.size() ; ++i){
		int start = std::max(0,i-5);
		int end = i;
		std::vector<int> test_vec(end-start);
		for(int j = 0; j < test_vec.size(); ++j){
			test_vec[j] = vec[(start + j) % vec.size()];
		}
		test_snapshot(test_vec,res.get_snapshot());
		res.mark(vec[i]);
	}

}

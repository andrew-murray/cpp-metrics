#include <boost/test/unit_test.hpp>
#include "common.hpp"
#include "instruments/timer.hpp"

BOOST_AUTO_TEST_CASE(timer_test){
	mock::clock::set_time(0);
	metrics::instruments::clocked_timer<mock::clock> test_timer;
	test_timer.time([&](){mock::clock::set_time(5);});

	BOOST_CHECK_EQUAL(test_timer.count(),1);
	test_snapshot(std::vector<int>{5},test_timer.get_snapshot());

	test_timer.time([](){mock::clock::set_time(15);});
	test_snapshot(std::vector<int>{5,10},test_timer.get_snapshot());

	test_timer.time([](){mock::clock::set_time(115);});
	test_snapshot(std::vector<int>{5,10,100},test_timer.get_snapshot());
}

BOOST_AUTO_TEST_CASE(scoped_timer_test){
	mock::clock::duration dur(0);
	mock::clock::set_time(0);
	{
		auto preserve_result = [&](mock::clock::duration d){dur = d;};
		metrics::instruments::scoped_timer<mock::clock> timer(preserve_result);
		mock::clock::set_time(100);
	}
	BOOST_CHECK_EQUAL(dur.count(),100);
}


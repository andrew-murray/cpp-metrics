#include <boost/test/unit_test.hpp>
#include "instruments/histogram.hpp"
#include "instruments/timer.hpp"
#include <chrono>

BOOST_AUTO_TEST_CASE(histogram_test){
	metrics::instruments::histogram h;
	using metrics::instruments::scoped_timer;
	scoped_timer<>::duration local_duration;
	scoped_timer<>([&](const scoped_timer<>::duration& time){local_duration = time;});
}

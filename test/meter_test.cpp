#include <chrono>
#include <boost/test/unit_test.hpp>
#include "meter.hpp"
namespace mock{
	static int time = 0;
	class mock_clock {
	public:
		typedef std::chrono::seconds duration ;
		typedef std::chrono::time_point<mock_clock> time_point;
		time_point now(){
			return time_point(duration(time));
		}
	};
}

BOOST_AUTO_TEST_CASE(meter_counting_test){
	meter m;
	BOOST_CHECK_EQUAL(m.count(),0);

	// test mark by 1, by n
	m.mark();
	BOOST_CHECK_EQUAL(m.count(),1);

	// test increment-by-delta
	m.mark(7);
	BOOST_CHECK_EQUAL(m.count(),8);
}

BOOST_AUTO_TEST_CASE(meter_rate_test){
	clocked_meter<mock::mock_clock> m;
	mock::time = 1;
	BOOST_CHECK_EQUAL(m.mean_rate(),0);

	mock::time = 5;
	m.mark();
	BOOST_CHECK_CLOSE(m.mean_rate(),0.2,1e-8);


	mock::time = 100;
	m.mark(99);
	BOOST_CHECK_CLOSE(m.mean_rate(),1,1e-8);
}
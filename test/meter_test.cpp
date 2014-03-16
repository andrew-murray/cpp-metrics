#include <chrono>
#include <boost/test/unit_test.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/thread/thread.hpp>
#include "meter.hpp"

BOOST_AUTO_TEST_CASE(meter_counting_test){
	meter<> m;
	BOOST_CHECK_EQUAL(m.count(),0);

	// test mark by 1, by n
	m.mark();
	BOOST_CHECK_EQUAL(m.count(),1);

	// test increment-by-delta
	m.mark(7);
	BOOST_CHECK_EQUAL(m.count(),8);
}

BOOST_AUTO_TEST_CASE(meter_rate_test){
	// TODO: redo this to take a mock clock
	// this was fun but obviously nuts
	meter<> m;
	BOOST_CHECK_EQUAL(m.mean_rate(),0);

	boost::this_thread::sleep_for(boost::chrono::duration_cast<boost::chrono::nanoseconds>(boost::chrono::milliseconds(20)));
	m.mark();
	BOOST_CHECK_CLOSE(m.mean_rate(),50,1);


	boost::this_thread::sleep_for(boost::chrono::duration_cast<boost::chrono::nanoseconds>(boost::chrono::milliseconds(20)));
	m.mark(50);
	BOOST_CHECK_CLOSE(m.mean_rate(),51*(1.0f/0.04f),1*(1.0f/0.04f));


}
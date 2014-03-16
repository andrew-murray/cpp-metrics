#include <boost/test/unit_test.hpp>
#include "counter.hpp"

BOOST_AUTO_TEST_CASE(counter_test){
	counter c;
	BOOST_CHECK_EQUAL(c.count(),0);

	// test default increment
	c.increment();
	BOOST_CHECK_EQUAL(c.count(),1);
	c.decrement();
	BOOST_CHECK_EQUAL(c.count(),0);

	// test increment-by-delta
	c.increment(7);
	BOOST_CHECK_EQUAL(c.count(),7);
	c.decrement(3);
	BOOST_CHECK_EQUAL(c.count(),4);
	
}

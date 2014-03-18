#include <boost/test/unit_test.hpp>
#include "registry.hpp"

BOOST_AUTO_TEST_CASE(registry_counter_test){
	metrics::registry reg;

	// test the counter registry supplies works
	auto& simple_counter = reg.counter("simple.counter.test");
	BOOST_CHECK_EQUAL(simple_counter.count(),0);
	simple_counter.increment(8);
	BOOST_CHECK_EQUAL(simple_counter.count(),8);

	// test you can obtain another instance of the same counter and
	// use it concurrently
	auto& counter_simple = reg.counter("simple.counter.test");
	BOOST_CHECK_EQUAL(simple_counter.count(),counter_simple.count());
	counter_simple.increment(3);
	BOOST_CHECK_EQUAL(simple_counter.count(),counter_simple.count());

	// get another counter and assure it's default initialized
	auto& default_counter = reg.counter("simple.counter.default");
	BOOST_CHECK_EQUAL(default_counter.count(),0);

}

BOOST_AUTO_TEST_CASE(registry_meter_test){
	metrics::registry reg;

	// test the counter registry supplies works
	auto& simple_meter = reg.meter("simple.meter.test");
	BOOST_CHECK_EQUAL(simple_meter.count(),0);
	simple_meter.mark(8);
	BOOST_CHECK_EQUAL(simple_meter.count(),8);

	auto& meter_simple = reg.meter("simple.meter.test");
	BOOST_CHECK_EQUAL(meter_simple.count(),simple_meter.count());
	meter_simple.mark(10);
	BOOST_CHECK_EQUAL(meter_simple.count(),simple_meter.count());

	auto& default_meter = reg.meter("simple.meter.default");
	BOOST_CHECK_EQUAL(default_meter.count(),0);

}

BOOST_AUTO_TEST_CASE(registry_gauge_test){
	metrics::registry reg;

	int value = 0;
	std::function<int(void)> callable([&value](){return value;});

	// test the counter registry supplies works
	auto& simple_gauge = reg.gauge("simple.gauge.test",callable);
	BOOST_CHECK_EQUAL(simple_gauge.get_valid(),false);
/*
	auto& gauge_3 = reg.get_gauge("simple.gauge.test");
	BOOST_CHECK_EQUAL(simple_gauge.get_valid(),true);
	BOOST_CHECK_EQUAL(gauge_2.get_valid(),true);
	BOOST_CHECK_EQUAL(gauge_3.get_valid(),true);

	BOOST_CHECK_EQUAL(simple_gauge.get_value(),value);
	BOOST_CHECK_EQUAL(gauge_2.get_value(),value);
	BOOST_CHECK_EQUAL(gauge_3.get_value(),value);

	value = 7;

	BOOST_CHECK_EQUAL(simple_gauge.get_value(),value);
	BOOST_CHECK_EQUAL(gauge_2.get_value(),value);
	BOOST_CHECK_EQUAL(gauge_3.get_value(),value);


	// test the counter registry supplies works
	auto& default_gauge = reg.get_gauge("simple.gauge.other");
	BOOST_CHECK_EQUAL(default_gauge.get_valid(),false);
*/
}





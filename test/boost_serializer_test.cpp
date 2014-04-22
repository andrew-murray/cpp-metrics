#include <iostream>
#include <thread>
#include <boost/test/unit_test.hpp>
#include "reporting/boost_serializer.hpp"
#include "reporting/graphite_serializer.hpp"
#include <boost/archive/text_iarchive.hpp>

namespace mock {
	typedef metrics::reporting::regular_boost_reporter<boost::archive::xml_oarchive,std::ostream> xml_reporter;
	typedef metrics::reporting::regular_boost_reporter<boost::archive::text_oarchive,std::ostream> text_reporter;
}

BOOST_AUTO_TEST_CASE(metrics_text_reporter_test){
	metrics::registry reg;
	// this test is seriously weak
	reg.counter("simple.counter.for_serializing").mark(10879);
	reg.counter("another.counter.for_serializing").mark(30091);
	reg.gauge("look.at.this.gauge",[&](){return 9998.0f;});
	reg.meter("another.meter.why_not").mark(1150);

	std::stringstream output_stream;
	mock::text_reporter a(output_stream,reg,std::chrono::milliseconds(20));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::string all_output(output_stream.str());

	// check for presence of all instruments
	BOOST_CHECK(all_output.find("simple.counter.for_serializing")!=std::string::npos);
	BOOST_CHECK(all_output.find("another.counter.for_serializing")!=std::string::npos);
	BOOST_CHECK(all_output.find("another.meter.why_not")!=std::string::npos);
	BOOST_CHECK(all_output.find("look.at.this.gauge")!=std::string::npos);
	
	BOOST_CHECK(all_output.find("10879")!=std::string::npos);	
	BOOST_CHECK(all_output.find("30091")!=std::string::npos);	
	BOOST_CHECK(all_output.find("9998")!=std::string::npos);
	// Need to write a full mock registry in order to test
	// the meter serialization really
}

BOOST_AUTO_TEST_CASE(metrics_xml_reporter_test){
	metrics::registry reg;
	// this test is seriously weak
	reg.counter("simple.counter.for_serializing").mark(10879);
	reg.counter("another.counter.for_serializing").mark(30091);
	reg.gauge("look.at.this.gauge",[&](){return 9998.0f;});
	reg.meter("another.meter.why_not").mark(1150);

	std::stringstream output_stream;
	mock::xml_reporter a(output_stream,reg,std::chrono::milliseconds(20));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::string all_output(output_stream.str());

	// check for presence of all instruments
	BOOST_CHECK(all_output.find("simple.counter.for_serializing")!=std::string::npos);
	BOOST_CHECK(all_output.find("another.counter.for_serializing")!=std::string::npos);
	BOOST_CHECK(all_output.find("another.meter.why_not")!=std::string::npos);
	BOOST_CHECK(all_output.find("look.at.this.gauge")!=std::string::npos);
	
	BOOST_CHECK(all_output.find("10879")!=std::string::npos);	
	BOOST_CHECK(all_output.find("30091")!=std::string::npos);	
	BOOST_CHECK(all_output.find("9998")!=std::string::npos);
	// Need to write a full mock registry in order to test
	// the meter serialization really

}

BOOST_AUTO_TEST_CASE(metrics_graphite_reporter_test){
	metrics::registry reg;
	// this test is seriously weak
	reg.counter("simple.counter.for_serializing").mark(10879);
	reg.counter("another.counter.for_serializing").mark(30091);
	reg.meter("another.meter.why_not").mark(1150);
	reg.gauge("look.at.this.gauge",[&](){return 999.0f;});
	// note : in ad-hoc prototyping this worked
	// however need to write a test using a micro boost::asio server
	// and have a similar weak test 
	//metrics::reporting::graphite_reporter<std::chrono::high_resolution_clock> a("localhost","2003",reg,std::chrono::milliseconds(20));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
#include <iostream>
#include <thread>
#include <boost/test/unit_test.hpp>
#include "reporting/boost_serializer.hpp"
#include <boost/archive/text_iarchive.hpp>

namespace mock {
	typedef metrics::reporting::regular_boost_reporter<boost::archive::xml_oarchive,std::ofstream> text_reporter;
}

BOOST_AUTO_TEST_CASE(metrics_text_reporter_test){
	std::ofstream output_stream("this_file.txt");
	{
		metrics::registry reg;
		// this test is seriously weak
		reg.counter("simple.counter.for_serializing").mark(10879);
		reg.counter("another.counter.for_serializing").mark(30091);
		mock::text_reporter a(output_stream,reg,std::chrono::milliseconds(20));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
#if 0
	std::cout << output_stream.str() << std::endl;
	std::string all = output_stream.str();
	{
		std::cout << all;
		BOOST_CHECK(all.find("simple.counter.for_serializing")!=std::string::npos);
		BOOST_CHECK(all.find("another.counter.for_serializing")!=std::string::npos);
		std::ostringstream stream;
		stream << 10879;
		BOOST_CHECK(all.find(stream.str())!=std::string::npos);
		stream.str("");
		stream << 30091;
		BOOST_CHECK(all.find(stream.str())!=std::string::npos);
	}
	#endif
}
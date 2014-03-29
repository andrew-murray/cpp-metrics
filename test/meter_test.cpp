#include <chrono>
#include <boost/test/unit_test.hpp>
#include "meter.hpp"
#include <thread>

namespace mock{
	static std::atomic<int> time;
	class clock {
	public:
		typedef std::chrono::seconds duration ;
		typedef std::chrono::time_point<clock> time_point;
		time_point now(){
			return time_point(duration(time));
		}
	};
}

BOOST_AUTO_TEST_CASE(meter_counting_test){
	metrics::instruments::meter m;
	BOOST_CHECK_EQUAL(m.count(),0);

	// test mark by 1, by n
	m.mark();
	BOOST_CHECK_EQUAL(m.count(),1);

	// test increment-by-delta
	m.mark(7);
	BOOST_CHECK_EQUAL(m.count(),8);
}

BOOST_AUTO_TEST_CASE(meter_rate_test){
	mock::time = 0;
	metrics::instruments::clocked_meter<mock::clock> m;
	mock::time = 1;
	BOOST_CHECK_EQUAL(m.mean_rate(),0);

	mock::time = 5;
	m.mark();
	BOOST_CHECK_CLOSE(m.mean_rate(),0.2,1e-8);


	mock::time = 100;
	m.mark(99);
	BOOST_CHECK_CLOSE(m.mean_rate(),1,1e-8);
}

BOOST_AUTO_TEST_CASE(meter_ewma_test){
	mock::time = 0;
	metrics::instruments::clocked_meter<mock::clock> m;
	// mean_rate returns nan
	// is this good? It's accurate.
	//BOOST_CHECK_EQUAL(m.mean_rate(),0);
	//BOOST_CHECK_EQUAL(m.one_minute_rate(),0);
	//BOOST_CHECK_EQUAL(m.five_minute_rate(),0);
	//BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),0);

	mock::time = 1;
	BOOST_CHECK_EQUAL(m.mean_rate(),0);
	BOOST_CHECK_EQUAL(m.one_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.five_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),0);

	bool terminate = false;
	auto force_count = [&terminate,&m](int& time){
		while(!terminate){
			if(time != mock::time){
				int diff = mock::time - time;
				m.mark(5 * diff);
				time = mock::time;
			}
		}
	};

	int time_0 = mock::time;
	int time_1 = mock::time;

	std::thread marker_0(force_count,std::ref(time_0));
	std::thread marker_1(force_count,std::ref(time_1));

	BOOST_CHECK_EQUAL(m.count(),0);
	BOOST_CHECK_EQUAL(m.mean_rate(),0);
	BOOST_CHECK_EQUAL(m.one_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.five_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),0);

	mock::time = 2;

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	BOOST_CHECK_EQUAL(m.count(),10);
	BOOST_CHECK_EQUAL(m.mean_rate(),10 / mock::time);
	BOOST_CHECK_EQUAL(m.one_minute_rate(),3);
	//BOOST_CHECK_EQUAL(m.five_minute_rate(),3);
	//BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),3);
#if 0
	mock::time = 5;

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	BOOST_CHECK_EQUAL(m.mean_rate(),12);
	///BOOST_CHECK_EQUAL(m.one_minute_rate(),12);
	//BOOST_CHECK_EQUAL(m.five_minute_rate(),12);
	//BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),12);
#endif
	terminate = true;

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	marker_0.join();
	marker_1.join();
	std::cout << m.count() << ',' << m.mean_rate() << std::endl;
	std::cout << "rate numbers" << std::endl;
}

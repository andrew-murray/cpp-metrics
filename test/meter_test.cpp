#include <chrono>
#include <thread>
#include <boost/test/unit_test.hpp>
#include "instruments/meter.hpp"
#include "common.hpp"

namespace mock{
	class ticker;
	static std::vector<ticker*> one_minute_tickers;
	static std::vector<ticker*> five_minute_tickers;
	static std::vector<ticker*> fifteen_minute_tickers;
	
	class ticker {
	public:
		typedef std::chrono::duration<double, std::chrono::nanoseconds::period> time_type;

		template<typename DurationAll, typename DurationTick>
		ticker(const DurationAll& footprint,const DurationTick& tick)
		: m_tick(std::chrono::duration_cast<std::chrono::nanoseconds>(tick)),
		  m_tick_count(0),
		  m_mark_count(0),
		  m_footprint(std::chrono::duration_cast<std::chrono::minutes>(footprint))
		{
			if(m_footprint == std::chrono::minutes(1)){
				one_minute_tickers.push_back(this);
			} else if(m_footprint == std::chrono::minutes(5)){
				five_minute_tickers.push_back(this);
			} else if(m_footprint == std::chrono::minutes(15)){
				fifteen_minute_tickers.push_back(this);
			} else {
				throw std::invalid_argument("mock ticker received a non-standard footprint.");
			}
		}

		void tick(){
			++m_tick_count;
		}

		void mark(unsigned int n = 1){
			m_mark_count += n;
		}

		template<typename Duration = std::chrono::minutes>
	    double rate(const Duration& unit = std::chrono::minutes(1)) {
	        return 0.0;
	    }

	    time_type interval(){
	    	return std::chrono::nanoseconds(5000);
	    }

	    int count(){
	    	return m_mark_count;
	    }

	    int ticks(){
	    	return m_tick_count;
	    }
	    std::chrono::nanoseconds m_tick;
	    std::chrono::minutes m_footprint;
	    unsigned int m_tick_count;
	    unsigned int m_mark_count;
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
	mock::clock::set_time(0);
	metrics::instruments::clocked_meter<mock::clock> m;
	mock::clock::set_time(1);
	BOOST_CHECK_EQUAL(m.mean_rate(),0);

	mock::clock::set_time(5);
	m.mark();
	BOOST_CHECK_CLOSE(m.mean_rate(),0.2,1e-8);


	mock::clock::set_time(100);
	m.mark(99);
	BOOST_CHECK_CLOSE(m.mean_rate(),1,1e-8);
}





BOOST_AUTO_TEST_CASE(meter_ticker_test){
	using mock::ticker;

	mock::clock::set_time(0);
	typedef metrics::instruments::clocked_meter<mock::clock,mock::ticker> mock_meter;
	mock_meter m;
	BOOST_ASSERT(mock::one_minute_tickers.size() == 1);
	BOOST_ASSERT(mock::five_minute_tickers.size() == 1);
	BOOST_ASSERT(mock::fifteen_minute_tickers.size() == 1);

	ticker& ticker_1 = *mock::one_minute_tickers[0];
	ticker& ticker_5 = *mock::five_minute_tickers[0];
	ticker& ticker_15 = *mock::fifteen_minute_tickers[0];

	BOOST_CHECK_EQUAL(ticker_1.count(),0);
	BOOST_CHECK_EQUAL(ticker_5.count(),0);
	BOOST_CHECK_EQUAL(ticker_15.count(),0);
	BOOST_CHECK_EQUAL(ticker_1.ticks(),0);
	BOOST_CHECK_EQUAL(ticker_5.ticks(),0);
	BOOST_CHECK_EQUAL(ticker_15.ticks(),0);


	mock::clock::set_time(4);

	BOOST_CHECK_EQUAL(ticker_1.count(),0);
	BOOST_CHECK_EQUAL(ticker_5.count(),0);
	BOOST_CHECK_EQUAL(ticker_15.count(),0);
	BOOST_CHECK_EQUAL(ticker_1.ticks(),0);
	BOOST_CHECK_EQUAL(ticker_5.ticks(),0);
	BOOST_CHECK_EQUAL(ticker_15.ticks(),0);

	m.mark(5);

	BOOST_CHECK_EQUAL(ticker_1.count(),5);
	BOOST_CHECK_EQUAL(ticker_5.count(),5);
	BOOST_CHECK_EQUAL(ticker_15.count(),5);
	BOOST_CHECK_EQUAL(ticker_1.ticks(),0);
	BOOST_CHECK_EQUAL(ticker_5.ticks(),0);
	BOOST_CHECK_EQUAL(ticker_15.ticks(),0);

	mock::clock::set_time(5);

	BOOST_CHECK_EQUAL(ticker_1.count(),5);
	BOOST_CHECK_EQUAL(ticker_5.count(),5);
	BOOST_CHECK_EQUAL(ticker_15.count(),5);
	BOOST_CHECK_EQUAL(ticker_1.ticks(),0);
	BOOST_CHECK_EQUAL(ticker_5.ticks(),0);
	BOOST_CHECK_EQUAL(ticker_15.ticks(),0);

	// regardless of implementation
	// marking & getting the latest value
	// has to flush ticks
	m.mark(5);
	double r = m.one_minute_rate();

	BOOST_CHECK_EQUAL(ticker_1.count(),10);
	BOOST_CHECK_EQUAL(ticker_5.count(),10);
	BOOST_CHECK_EQUAL(ticker_15.count(),10);
	BOOST_CHECK_EQUAL(ticker_1.ticks(),1);
	BOOST_CHECK_EQUAL(ticker_5.ticks(),1);
	BOOST_CHECK_EQUAL(ticker_15.ticks(),1);

	mock::clock::set_time(100);

	m.mark(90);
	r = m.fifteen_minute_rate();


	BOOST_CHECK_EQUAL(ticker_1.count(),100);
	BOOST_CHECK_EQUAL(ticker_5.count(),100);
	BOOST_CHECK_EQUAL(ticker_15.count(),100);
	BOOST_CHECK_EQUAL(ticker_1.ticks(),20);
	BOOST_CHECK_EQUAL(ticker_5.ticks(),20);
	BOOST_CHECK_EQUAL(ticker_15.ticks(),20);

	mock::clock::set_time(104);
	m.mark(1);
	r = m.fifteen_minute_rate();

	BOOST_CHECK_EQUAL(ticker_1.count(),101);
	BOOST_CHECK_EQUAL(ticker_5.count(),101);
	BOOST_CHECK_EQUAL(ticker_15.count(),101);
	BOOST_CHECK_EQUAL(ticker_1.ticks(),20);
	BOOST_CHECK_EQUAL(ticker_5.ticks(),20);
	BOOST_CHECK_EQUAL(ticker_15.ticks(),20);

	mock::clock::set_time(105);
	m.mark(1);
	r = m.fifteen_minute_rate();


	BOOST_CHECK_EQUAL(ticker_1.count(),102);
	BOOST_CHECK_EQUAL(ticker_5.count(),102);
	BOOST_CHECK_EQUAL(ticker_15.count(),102);
	BOOST_CHECK_EQUAL(ticker_1.ticks(),21);
	BOOST_CHECK_EQUAL(ticker_5.ticks(),21);
	BOOST_CHECK_EQUAL(ticker_15.ticks(),21);

}

void wait_for(const std::function<bool(void)>& func){
    do {
        std::this_thread::yield();
    } while (!func());
}

BOOST_AUTO_TEST_CASE(meter_ewma_test){

	mock::clock::set_time(0);
	metrics::instruments::clocked_meter<mock::clock> m;
	// mean_rate returns nan
	// is this good? It's accurate.
	//BOOST_CHECK_EQUAL(m.mean_rate(),0);
	//BOOST_CHECK_EQUAL(m.one_minute_rate(),0);
	//BOOST_CHECK_EQUAL(m.five_minute_rate(),0);
	//BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),0);

	mock::clock::set_time(1);
	BOOST_CHECK_EQUAL(m.mean_rate(),0);
	BOOST_CHECK_EQUAL(m.one_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.five_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),0);

	bool terminate = false;
	auto force_count = [&terminate,&m](std::atomic<int>& time){
		while(!terminate){
			if(time != mock::clock::time()){
				int diff = mock::clock::time() - time;
				m.mark(5 * diff);
				time = (int)mock::clock::time();
			}
		}
	};

	std::atomic<int> time_0((int)mock::clock::time());
	std::atomic<int> time_1((int)mock::clock::time());

	std::thread marker_0(force_count,std::ref(time_0));
	std::thread marker_1(force_count,std::ref(time_1));

	BOOST_CHECK_EQUAL(m.count(),0);
	BOOST_CHECK_EQUAL(m.mean_rate(),0);
	BOOST_CHECK_EQUAL(m.one_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.five_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),0);

	mock::clock::set_time(2);

	auto verify_2= [&time_0,&time_1]()->bool{
			return (time_0 == 2) && (time_1 == 2);
	};

	auto verify_4 = [&time_0,&time_1]()->bool{
			return (time_0 == 4) && (time_1 == 4);
	};

	wait_for(verify_2);

	BOOST_CHECK_EQUAL(m.count(),10);
	BOOST_CHECK_EQUAL(m.mean_rate(),10 / mock::clock::time());
	BOOST_CHECK_EQUAL(m.one_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.five_minute_rate(),0);
	BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),0);


	mock::clock::set_time(4);

	wait_for(verify_4);

	int count_before_interval = m.count();
	BOOST_CHECK_EQUAL(count_before_interval,30);
	terminate = true;


	marker_0.join();
	marker_1.join();
	
	mock::clock::set_time(5);


	BOOST_CHECK_EQUAL(m.one_minute_rate(),(count_before_interval / mock::clock::time()) * 60);
	BOOST_CHECK_EQUAL(m.five_minute_rate(),(count_before_interval / mock::clock::time()) * 60);
	BOOST_CHECK_EQUAL(m.fifteen_minute_rate(),(count_before_interval / mock::clock::time()) * 60);


}

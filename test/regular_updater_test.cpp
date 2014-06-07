#include <iostream>
#include <chrono>
#include <thread>
#include <boost/test/unit_test.hpp>
#include "utils/regular_updater.hpp"	
#include "common.hpp"
#if 0
	// VC13's implementation of sleep_for / sleep_until
	// are non-compliant in that they
	// don't pay attention to custom clocks
	// therefore regular_updater is currently unmockable

	// TODO: Switch to boost thread ?
namespace mock {
	class update_logger {
	public:
		update_logger()
		: m_interval(std::chrono::milliseconds(100))
		, m_count(0)
		, m_updater(m_interval,[this](){this->update();})
		{
			lockfree_flag.test_and_set();
			m_updater.begin_updates();
		}

		~update_logger(){
			m_updater.halt_updates();
		}

		void stop(){
			m_updater.halt_updates();
		}

		int count(){
			return m_count;
		}

		void update(){
			lockfree_flag.clear();
			++m_count;
			m_time = mock::clock::time();
		}

		unsigned int time(){
			return m_time;
		}

		std::chrono::nanoseconds interval(){
			return m_interval;
		}
		unsigned int m_count;
		std::chrono::nanoseconds m_interval;
		unsigned int m_time;
		std::atomic_flag lockfree_flag;
		metrics::utils::regular_updater<mock::clock> m_updater;
	};
}

template<typename T>
long long ns_count(const T& in){
	typedef std::chrono::nanoseconds ns;
	return std::chrono::duration_cast<ns>(in).count();
}

BOOST_AUTO_TEST_CASE(regular_updater_test){

	mock::clock::set_time(0);
	mock::update_logger logger;
	// spin until thread has launched
	std::cout << "waiting" << std::endl;
	while(logger.lockfree_flag.test_and_set(std::memory_order_acquire)){}
	std::cout << "safe" << std::endl;
	mock::clock::set_time(1);
	wait_for(
		[&]()->bool{
			return (logger.count() == ns_count(std::chrono::seconds(1)) / ns_count(logger.interval()));
		}
	);

	typedef std::chrono::nanoseconds ns;

	BOOST_CHECK_EQUAL(logger.time(), 1);
	BOOST_CHECK_EQUAL(logger.count(), ns_count(std::chrono::seconds(1)) / ns_count(logger.interval()));

	std::this_thread::sleep_for(std::chrono::seconds(1));

	BOOST_CHECK_EQUAL(logger.time(), 1);
	BOOST_CHECK_EQUAL(logger.count(), ns_count(std::chrono::seconds(1)) / ns_count(logger.interval()));

	mock::clock::set_time(2);
	wait_for(
		[&]()->bool{
			return (logger.count() == ns_count(std::chrono::seconds(2)) / ns_count(logger.interval()));
		}
	);

	BOOST_CHECK_EQUAL(logger.time(), 2);
	BOOST_CHECK_EQUAL(logger.count(), ns_count(std::chrono::seconds(2)) / ns_count(logger.interval()));

	boost::this_thread::sleep_until(boost::chrono::steady_clock::now());

	logger.stop();

}

#endif


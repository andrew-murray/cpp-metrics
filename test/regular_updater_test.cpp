#include <iostream>
#include <chrono>
#include <thread>
#include <boost/test/unit_test.hpp>
#include "utils/regular_updater_mixin.hpp"	
#include "common.hpp"


namespace mock {
	class update_logger : public metrics::utils::regular_updater_mixin<std::chrono::high_resolution_clock> {
	public:
		update_logger()
		: regular_updater_mixin(std::chrono::milliseconds(100))
		, m_interval(std::chrono::milliseconds(100))
		, m_count(0)
		, lockfree_flag(true)
		{
			begin_updates();
		}

		~update_logger(){
			halt_updates();
		}

		void stop(){
			halt_updates();
		}

		int count(){
			return m_count;
		}

		void update(){
			lockfree_flag.clear();
			++m_count;
		}

		std::chrono::nanoseconds interval(){
			return m_interval;
		}
		unsigned int m_count;
		std::chrono::nanoseconds m_interval;
		std::atomic_flag lockfree_flag;
	};
}


BOOST_AUTO_TEST_CASE(regular_updater_test){
	mock::update_logger logger;
	// spin until thread has launched
	while(logger.lockfree_flag.test_and_set(std::memory_order_acquire)){}
	auto&& interval = std::chrono::seconds(2);
	std::this_thread::sleep_for(interval);
	logger.stop();
	BOOST_CHECK_CLOSE((float)logger.count(),(float)(interval / logger.interval()),1.0);
}

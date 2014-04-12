#include <iostream>
#include <chrono>
#include <thread>
#include <boost/test/unit_test.hpp>
#include "utils/regular_updater_mixin.hpp"	
#include "common.hpp"


namespace mock {
	class update_logger : public metrics::utils::regular_updater_mixin<mock::clock> {
	public:
		update_logger()
		: regular_updater_mixin(std::chrono::milliseconds(1))
		{
			begin_updates();
		}

		~update_logger(){
			halt_updates();
		}
	private:
		void update(){
		}
	};
}


BOOST_AUTO_TEST_CASE(regular_updater_test){
	mock::update_logger logger;
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

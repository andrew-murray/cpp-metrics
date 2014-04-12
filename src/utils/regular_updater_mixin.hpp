#include <chrono>
#include <functional>
#include <type_traits>

namespace metrics {
	namespace utils {
		/*
			A mixin that spawns a thread to periodically call a function
			update() 
			in a derived class.

			I have a really bad feeling I'm trying to do something impossible
			and this pattern will inevitably cause a virtual function call
			exception ... however it's really neat if it'll work
		*/
		template<typename ClockType = std::chrono::high_resolution_clock>
		class regular_updater_mixin {
		private:
			void thread_main(){
				try {
					do {
						auto&& tick_time(m_clock.now());
						std::cout << "tick" << std::endl;
						update();
						std::this_thread::sleep_until(tick_time + m_tick_ns);
					} while (!m_killed);
				} catch(...) {
					handle_update_exception(std::current_exception());
				}
			}
		public:

			typedef std::chrono::nanoseconds ns;

			template<typename T>
			regular_updater_mixin(const T& interval)
			: m_tick_ns(std::chrono::duration_cast<ns>(interval))
			, m_killed(false)
			, m_thread(&regular_updater_mixin::thread_main, this)
			{

			}
			virtual ~regular_updater_mixin(){
				std::cout << "starting to destruct " << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				m_killed = true;
				if(m_thread.joinable()){
					m_thread.join();
				}
			}
			ns m_tick_ns;
			std::thread m_thread;
			bool m_killed;
		private:

			virtual void update() = 0;
			virtual void handle_update_exception(const std::exception_ptr eptr){}
			ClockType m_clock;
		};
	}
}
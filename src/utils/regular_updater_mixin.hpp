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


			Unfortunately ~ you MUST call begin_updates and halt_updates
			~ expected to be at the end of your constructor and the beginning 
			of your destructor... there may need to be some sort of synchronization
			in this class later.	
		*/
		template<typename ClockType = std::chrono::high_resolution_clock>
		class regular_updater_mixin {
		private:
			void thread_main(){
				try {
					do {
						auto&& current_time(m_clock.now());
						update();
						std::this_thread::sleep_until(current_time + m_tick_ns);
					} while (m_active);
				} catch(...) {
					handle_update_exception(std::current_exception());
				}
			}
		public:

			typedef std::chrono::nanoseconds ns;

			template<typename T>
			regular_updater_mixin(const T& interval)
			: m_tick_ns(std::chrono::duration_cast<ns>(interval))
			, m_active(false)
			{

			}
			virtual ~regular_updater_mixin(){}

			void begin_updates(){
				m_active = true;
				m_thread = std::thread(&regular_updater_mixin::thread_main, this);
			}

			void halt_updates(){
				m_active = false;
				if(m_thread.joinable()){
					m_thread.join();
				}			
			}
			ns m_tick_ns;
			std::thread m_thread;
			bool m_active;
		private:

			virtual void update() = 0;
			virtual void handle_update_exception(const std::exception_ptr eptr){}
			ClockType m_clock;
		};
	}
}
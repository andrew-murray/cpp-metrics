#include <chrono>
#include <functional>
#include <type_traits>
#include <thread>

namespace metrics {
	namespace utils {
		/*
			 This is a utility class to allow periodic updates to be
			 queued.

			 Note: the passed callback cannot throw 
		*/
		template<typename ClockType = std::chrono::high_resolution_clock>
		class regular_updater{
		private:
			void thread_main(){
				do {
					auto&& current_time(m_clock.now());
					m_update();
					std::this_thread::sleep_until(current_time + m_tick_ns);
				} while (m_active);
			}
		public:

			typedef std::chrono::nanoseconds ns;

			template<typename T>
			regular_updater(const T& interval,const std::function<void()>& update)
			: m_tick_ns(std::chrono::duration_cast<ns>(interval))
			, m_active(false)
			, m_update(update)
			{

			}

			virtual ~regular_updater(){}

			void begin_updates(){
				m_active = true;
				m_thread = std::thread(&regular_updater::thread_main, this);
			}

			void halt_updates(){
				m_active = false;
				if(m_thread.joinable()){
					m_thread.join();
				}			
			}

		private:
			ns m_tick_ns;
			std::thread m_thread;
			bool m_active;
			std::function<void()> m_update;
			ClockType m_clock;
		};
	}
}
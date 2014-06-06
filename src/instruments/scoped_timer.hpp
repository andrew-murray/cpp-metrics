#include <chrono>
#include <thread>
namespace metrics{
	namespace instruments {
#ifndef _WIN32
		#define metrics_hint_likely(x)	__builtin_expect(!!(x), 1)
#else
		#define metrics_hint_likely(x) x
#endif
		template<typename ClockType = std::chrono::high_resolution_clock>
		class scoped_timer{
		public:

			typedef typename ClockType::time_point time_point;
			typedef typename ClockType::duration duration;

			scoped_timer(std::function<void(const duration&)> recorder
							= std::function<void(const duration&)>(
								[&](const duration& dur){
									std::cout << dur.count() << std::endl;
								})
						)
			: m_clock()
			, m_running(true)
			, m_record(recorder)
			, m_start(m_clock.now())
			{
			}


			inline void start(){
				m_start = m_clock.now();
				m_running = true;
			}

			inline void stop(){
				m_period = std::chrono::duration_cast<duration>(m_clock.now() - m_start);
				m_running = false;
			}

			inline ~scoped_timer(){
				if(metrics_hint_likely(m_running)){
					stop();
				}
				m_record(m_period);
			}

			ClockType m_clock;
			duration m_period;
			std::function<void(const duration&)> m_record;
			bool m_running;
			// important to initialize this last !
			time_point m_start;
		};
	}
}
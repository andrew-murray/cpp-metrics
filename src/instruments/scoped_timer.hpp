#include <chrono>
#include <thread>
namespace metrics{
	namespace instruments {
		#define hint_likely(x)	__builtin_expect(!!(x), 1)

		template<typename ClockType = std::chrono::high_resolution_clock>
		class scoped_timer{
		public:

			typedef typename ClockType::time_point time_point;
			typedef typename ClockType::duration duration;

			template<typename... ArgTypes >
			scoped_timer(std::function<void(const duration&)> recorder
							= std::function<void(const duration&)>(
								[&](const duration& dur){
									std::cout << dur << std::endl;
								})
						) __attribute__((always_inline))
			: m_clock()
			, m_running(true)
			, m_record(recorder)
			, m_start(m_clock.now())
			{

			}


			void start() __attribute__((always_inline)){
				m_start = m_clock.now();
				m_running = true;
			}

			void stop() __attribute__((always_inline)){
				m_time = std::chrono::duration_cast<duration>(m_clock.now() - m_start);
			}

			~scoped_timer() __attribute__((always_inline)){
				if(hint_likely(m_running)){
					stop();
				}
				m_record(m_time);
			}

			ClockType m_clock;
			duration m_time;
			std::function<void(const duration&)> m_record;
			bool m_running;
			// important to initialize this last !
			time_point m_start;
		};
	}
}
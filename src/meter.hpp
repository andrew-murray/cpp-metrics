#include <atomic>
#include <chrono>
#include "moving_average.hpp"

namespace metrics {
	namespace instruments {

		template<typename ClockType>
		class clocked_meter {
		public:
			typedef typename ClockType::time_point time_point;
			typedef typename ClockType::duration duration;

			clocked_meter()
			: m_count(std::make_shared<std::atomic<unsigned int>>(0)),
			  m_start_time(m_timer.now())
			{

			}

			unsigned int count(){
				return (unsigned int) *m_count;
			}

			void mark(const int& val = 1){
				*m_count += val;
			}

			double mean_rate(){
				typedef std::chrono::duration<double> fp_seconds;
				duration diff(m_timer.now() - m_start_time);
				fp_seconds s(std::chrono::duration_cast<fp_seconds>(diff));
				return *m_count / s.count();
			}
		private:
			std::shared_ptr<std::atomic<unsigned int>> m_count;
			ClockType m_timer;
			time_point m_start_time;
		};

		typedef clocked_meter<std::chrono::high_resolution_clock> meter;

	}
}
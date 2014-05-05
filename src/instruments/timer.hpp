#include <chrono>
#include "scoped_timer.hpp"
#include "meter.hpp"
#include "histogram.hpp"

namespace metrics{
	namespace instruments{
		template<typename ClockType = std::chrono::high_resolution_clock>
		class clocked_timer {
		public:
			clocked_timer()
			{

			}

			scoped_timer<ClockType> time_scope(){
				return scoped_timer<ClockType>([&](const typename scoped_timer<ClockType>::duration& dur){this->update(dur);});
			}

			template<typename T>
			void time(T&& func){
				scoped_timer<ClockType> local_timer(time_scope());
				func();
			}

			void update(const typename ClockType::duration& dur){
				m_histogram.mark(dur.count());
				m_meter.mark();
			}

			snapshot get_snapshot(){
				m_histogram.get_snapshot();
			}

		public:
			
			ClockType m_clock;
			clocked_meter<ClockType> m_meter;
			histogram m_histogram;
		};

		typedef clocked_timer<> timer;
	}
}
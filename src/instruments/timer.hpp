#pragma once
#include <chrono>
#include "scoped_timer.hpp"
#include "meter.hpp"
#include "histogram.hpp"
namespace metrics{
	namespace instruments{

		template<typename ClockType = std::chrono::high_resolution_clock>
		class clocked_timer {
		public:

			scoped_timer<ClockType> time_scope(){
				return scoped_timer<ClockType>([&](const typename scoped_timer<ClockType>::duration& dur){this->update(dur);});
			}

			template<typename T>
			void time(T&& func){
				// Kind've counting on the copy being elided
				auto local_timer = time_scope();
				// explicitly starting means that we don't include time
				// the move constructor in the call
				// this should be documented on the class
				local_timer.start();
				func();
			}

			size_t count() const {
				return m_meter.count();
			}


			void update(const typename ClockType::duration& dur){
				m_histogram.mark((int)dur.count());
				m_meter.mark();
			}

			snapshot get_snapshot() const {
				return m_histogram.get_snapshot();
			}

		public:
			
			ClockType m_clock;
			clocked_meter<ClockType> m_meter;
			histogram m_histogram;
		};

		typedef clocked_timer<> timer;
	}
}

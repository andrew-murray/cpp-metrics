#pragma once
#include <atomic>
#include <chrono>
#include <iostream>
#include "moving_average.hpp"

namespace metrics {
	namespace instruments {

		template<typename ClockType,typename AverageType = moving_average>
		class clocked_meter {
		public:
			typedef typename ClockType::time_point time_point;
			typedef typename ClockType::duration clock_duration;
			typedef std::chrono::nanoseconds ns;

			template<typename IntervalType = std::chrono::seconds>
			clocked_meter(const IntervalType& interval = (IntervalType)std::chrono::seconds(5))
			: m_count(std::make_shared<std::atomic<unsigned int>>(0))
			, m_counted(std::make_shared<std::atomic<unsigned int>>(0))
			, m_interval(interval)
			, m_start_time(m_timer.now())
			, m_last_tick(std::make_shared<std::atomic<ns>>(std::chrono::duration_cast<ns>(m_start_time.time_since_epoch())))
			, m_one_minute_tracker(std::make_shared<AverageType>(std::chrono::minutes(1),m_interval))
			, m_five_minute_tracker(std::make_shared<AverageType>(std::chrono::minutes(5),m_interval))
			, m_fifteen_minute_tracker(std::make_shared<AverageType>(std::chrono::minutes(15),m_interval))
			{

			}

			unsigned int count() const {
				return (unsigned int) *m_count;
			}

			void mark(const int& val = 1){
				tick_if_needed();
				*m_count += val;
			}

			double mean_rate() const {
				typedef std::chrono::duration<double> fp_seconds;
				clock_duration diff(m_timer.now() - m_start_time);
				fp_seconds s(std::chrono::duration_cast<fp_seconds>(diff));
				return *m_count / s.count();
			}

			double one_minute_rate() const {
				tick_if_needed();
				return m_one_minute_tracker->rate();
			}

			double five_minute_rate() const{
				tick_if_needed();
				return m_five_minute_tracker->rate();
			}

			double fifteen_minute_rate() const {
				tick_if_needed();
				return m_fifteen_minute_tracker->rate();
			}

		private:
			void tick_if_needed() const {
				// this function would perhaps be easier if we were
				// just counting ticks
				ns previous = *m_last_tick;
				ns now = std::chrono::duration_cast<ns>(m_timer.now().time_since_epoch());
				ns age = now - previous;
				if(age >= std::chrono::duration_cast<ns>(m_interval)){
					ns::rep last_tick = (previous + age).count() - (age.count() % std::chrono::duration_cast<ns>(m_interval).count());
					// vc13 chrono seems very unhappy, reverting to integer arithmetic
					// ns last_tick = previous + age - (age % m_interval);
					
					if(m_last_tick->compare_exchange_strong(previous,ns(last_tick))){
						// this implementation assumes this brace is not re-entrant
						unsigned int count = *m_count - *m_counted;
						*m_counted += count;
						// vc13 chrono seems very unhappy, reverting to integer arithmetic
						// auto ticks = (last_tick - previous) / m_interval;
						auto ticks = (unsigned int) ((last_tick - previous.count()) / std::chrono::duration_cast<ns>(m_interval).count());
						for(int i = 0; i < (int)ticks -1 ;++i){
							m_one_minute_tracker->tick(count  / ticks);
							m_five_minute_tracker->tick(count / ticks);
							m_fifteen_minute_tracker->tick(count / ticks);
						}
						
						m_one_minute_tracker->tick(count/ticks + count % ticks);
						m_five_minute_tracker->tick(count / ticks + count % ticks);
						m_fifteen_minute_tracker->tick(count / ticks + count % ticks);
					}
				}
			}

			std::chrono::seconds m_interval;
			std::shared_ptr<std::atomic<unsigned int>> m_count;
			ClockType m_timer;
			time_point m_start_time;
			std::shared_ptr<std::atomic<ns>> m_last_tick;
			std::shared_ptr<std::atomic<unsigned int>> m_counted;
			std::shared_ptr<AverageType> m_one_minute_tracker;
			std::shared_ptr<AverageType> m_five_minute_tracker;
			std::shared_ptr<AverageType> m_fifteen_minute_tracker;
		};

		typedef clocked_meter<std::chrono::high_resolution_clock> meter;

	}
}
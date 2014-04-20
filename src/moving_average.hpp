#pragma once
#include <chrono>
#include <atomic>
#include <cmath>
/*
 * This class is naieve to threading issues
 * it expects to be called safely
 */

class moving_average {
public:
	typedef std::chrono::duration<double, std::chrono::nanoseconds::period> time_type;

	typedef std::chrono::duration<double, std::chrono::seconds::period> fp_seconds_type;

	template<typename Duration>
	moving_average(const double& alpha,const Duration tick) 
		: m_alpha(alpha),
		  m_rate(0.0),
		  m_first(true),
		  m_uncounted(0),
		  m_interval(std::chrono::duration_cast<time_type>(tick))
	{

	}

	template<typename DurationAll, typename DurationTick, typename = typename std::enable_if<!std::is_same<DurationAll,double>::value>::type>
	moving_average(const DurationAll& footprint,const DurationTick& tick)
	: moving_average(1.0 - exp(-fp_seconds_type(tick).count()/fp_seconds_type(footprint).count()),tick)
	{
	}

	void tick(){
		unsigned int count = m_uncounted.exchange(0);
		double recent_rate = count / (double)m_interval.count();
		bool lvalue_true = true;
		if(m_first.compare_exchange_strong(lvalue_true,false)){
			m_rate = recent_rate;
		} else {
			//m_rate = alpha * recent_rate + (1.0 - alpha) * m_rate;
			m_rate += m_alpha * (recent_rate - m_rate);
		}
	}

	void mark(unsigned int n = 1){
		m_uncounted+=n;
	}

	template<typename Duration = std::chrono::minutes>
    double rate(const Duration& unit = std::chrono::minutes(1)) {
        return m_rate * (double) std::chrono::duration_cast<time_type>(unit).count();
    }

    time_type interval(){
    	return m_interval;
    }
private:
	double m_alpha;
	std::atomic<bool> m_first;
	double m_rate;
	std::atomic<unsigned int> m_uncounted;
	time_type m_interval;
};




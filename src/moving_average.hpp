#include <chrono>
#include <atomic>
#include <cmath>

#include <iostream>
/*
 * This class is naieve to threading issues
 * it expects to be called safely
 */

class moving_average {
public:
	typedef std::chrono::duration<double, std::chrono::nanoseconds::period> time_type;

	typedef std::chrono::duration<double, std::chrono::minutes::period> fp_minutes_type;

	template<typename Duration>
	moving_average(const double& alpha,const Duration dur) 
		: m_alpha(alpha),
		  m_rate(0.0),
		  m_first(true),
		  m_uncounted(0),
		  m_interval(std::chrono::duration_cast<time_type>(dur))
	{

	}

	template<typename DurationAll, typename DurationTick, typename = typename std::enable_if<!std::is_same<DurationAll,double>::value>::type>
	moving_average(const DurationAll& footprint,const DurationTick& tick) 
		: m_alpha(1.0 - exp(-fp_minutes_type(tick).count())),
		  m_rate(0.0),
		  m_first(true),
		  m_uncounted(0),
		  m_interval(std::chrono::duration_cast<time_type>(tick))
	{
		std::cout << fp_minutes_type(tick).count() << "time conv" << std::endl;
	}

	void tick(){
		unsigned int count = m_uncounted.exchange(0);
		double recent_rate = count / (double)m_interval.count();
		if(m_first){
			m_first = false;
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

	double m_alpha;
	bool m_first;
	double m_rate;
	std::atomic<unsigned int> m_uncounted;
	time_type m_interval;
};




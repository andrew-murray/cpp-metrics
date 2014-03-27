#include <chrono>

/*
 * This class is naieve to threading issues
 * it expects to be called safely
 */

class moving_average {

	typedef std::chrono::duration<double, std::chrono::nanoseconds::period> time_type;

	template<typename Duration>
	moving_average(const double& alpha,const Duration dur) 
		: m_alpha(alpha),
		  m_rate(0.0),
		  m_first(true),
		  m_uncounted(0),
		  m_interval(std::chrono::duration_cast<time_type>(dur))
	{

	}

	void tick(){
		unsigned int count = m_uncounted;
		m_uncounted = 0;
		double recent_rate = count / (double)m_interval.count();
		if(m_first){
			m_first = false;
			m_rate = recent_rate;
		} else {
			//m_rate = alpha * recent_rate + (1.0 - alpha) * m_rate;
			m_rate += m_alpha * (recent_rate - m_rate);
		}
	}

	double m_alpha;
	bool m_first;
	double m_rate;
	unsigned int m_uncounted;
	time_type m_interval;
};



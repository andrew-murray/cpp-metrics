#include <atomic>
#include <chrono>
#include <iostream>

class meter {
public:
	typedef std::chrono::high_resolution_clock::time_point time_point;
	typedef std::chrono::high_resolution_clock::duration duration;

	meter()
	: m_count(0),
	  m_start_time(m_timer.now())
	{

	}

	unsigned int count(){
		return (unsigned int) m_count;
	}

	void mark(const int& val = 1){
		m_count += val;
	}

	double mean_rate(){
		typedef std::chrono::duration<double> fp_seconds;
		duration diff(m_timer.now() - m_start_time);
		fp_seconds s(std::chrono::duration_cast<fp_seconds>(diff));
		return m_count / s.count();
	}
private:
	std::atomic<unsigned int> m_count;
	std::chrono::high_resolution_clock m_timer;
	time_point m_start_time;
};
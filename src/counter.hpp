#include <atomic>

class counter {
public:
	counter() 
		: m_count(0)
	{

	}

	void increment(const int& val = 1){
		m_count+= val;
	}

	void decrement(const int& val = 1){
		m_count-= val;		
	}

	unsigned int count(){
		return (unsigned int) m_count;
	}
private:
	std::atomic<unsigned int> m_count ;
};
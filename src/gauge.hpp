#include <functional>

class gauge {
public:
	gauge() : m_callable(false){};

	template<typename T> 
	void initialize(const std::function<T(void)> measure){
		if(!m_callable){
			m_measure = [&](){return (double) measure();};
			m_callable = true;
		} else {
			throw std::runtime_error("Multiple initialization.");
		}
	}

	template<typename T>
	gauge(const std::function<T(void)> measure)
		: m_measure([&](){return (double) measure();})
	{
	}

	virtual double get_value(){
		if(m_callable){
			return m_measure();
		} else {
			throw std::runtime_error("Gauge not initialized.");
		}
	}

	virtual bool get_valid(){
		return m_callable;
	}

	std::function<double(void)> m_measure;
	bool m_callable;
};
#include <functional>

class gauge {
	template<typename T>
	gauge(const std::function<T(void)> measure)
		: m_measure([&](){return (double) measure();})
	{
		// register self somewhere
		// want to implement flexible reporters
	}
	std::function<double(void)> m_measure;
};
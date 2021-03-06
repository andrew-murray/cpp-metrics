#pragma once
#include <functional>

namespace metrics {
	namespace instruments {

		class gauge {
		public:

			template<typename T>
			gauge(const std::function<T(void)>& measure)
				: m_measure([measure](){return (double) measure();})
			{
			}

			virtual double get_value() const {
					return m_measure();
			}

			std::function<double(void)> m_measure;
		};
		
	}
}
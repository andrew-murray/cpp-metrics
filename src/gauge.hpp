#include <functional>


namespace metrics {
	namespace instruments {
		class gauge {
		public:
			//gauge() : m_callable(std::make_shared<bool>(false)){};

			template<typename T> 
			void initialize(const std::function<T(void)> measure){
				if(!m_callable){
					m_measure = [&](){return (double) measure();};
					*m_callable = true;
				} else {
					throw std::runtime_error("Multiple initialization.");
				}
			}

			template<typename T>
			gauge(const std::function<T(void)>& measure)
				: m_measure([&](){return (double) measure();}),
				  m_callable(std::make_shared<bool>(true))
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
				return *m_callable;
			}

			std::function<double(void)> m_measure;
			std::shared_ptr<bool> m_callable;
		};
	}
}
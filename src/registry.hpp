#include "counter.hpp"
#include "gauge.hpp"
#include "meter.hpp"
#include <map>

namespace metrics {

	class registry {
	public:
		registry(const registry&) = delete;
		registry(const registry&&) = delete;
		registry& operator = (const registry&) = delete;
		registry& operator = (const registry&&) = delete;

		registry(){}
		template<typename T>
		T& get_or_add_object(const std::string& name,std::map<const std::string, T> objects){
			auto it = objects.find(name);
			if(it!=objects.end()){
				return it->second;
			} else {
				objects[name] = T();
				return objects[name];
			}
		}

		instruments::counter& counter(const std::string& name){
			return get_or_add_object(name,m_counters);
		}

		instruments::gauge& gauge(const std::string& name){
			return get_or_add_object(name,m_gauges);
		}

		instruments::meter& meter(const std::string& name){
			return get_or_add_object(name,m_meters);
		}

	private:
		std::map<const std::string, instruments::gauge> m_gauges;
		std::map<const std::string, instruments::counter> m_counters;
		std::map<const std::string, instruments::meter> m_meters;
	};
}
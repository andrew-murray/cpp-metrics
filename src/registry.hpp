#include "counter.hpp"
#include "gauge.hpp"
#include "meter.hpp"
#include <map>

class registry {
public:
	registry(const registry&) = delete;
	registry(const registry&&) = delete;
	registry& operator = (const registry&) = delete;
	registry& operator = (const registry&&) = delete;

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

	counter& get_counter(const std::string& name){
		return get_or_add_object(name,m_counters);
	}

	gauge& get_gauge(const std::string& name){
		return get_or_add_object(name,m_gauges);
	}

	meter& get_meter(const std::string& name){
		return get_or_add_object(name,m_meters);
	}

private:
	std::map<const std::string, gauge> m_gauges;
	std::map<const std::string, counter> m_counters;
	std::map<const std::string, meter> m_meters;
};
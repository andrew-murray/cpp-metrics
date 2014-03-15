#include "counter.hpp"
#include "timer.hpp"
#include "gauge.hpp"
#include <map>

class registry {
	counter& counter(const std::string& name){
		auto it = m_counters.find(name);
		if(it!=m_counters.end()){
			return it->second;
		} else {
			m_counters[name] = 0;
			return m_counters[name].second;
		}
	}

	void increment_count(const std::string& name){
		auto it = m_counters.find(name);
		if(it!=m_counters.end()){
			++(it->second);
		} else {
			m_counters[name] = 1;
		}
	}

	void decrement_count(const std::string& name){
		auto it = m_counters.find(name);
		if(it!=m_counters.end()){
			++(it->second);
		} else {
			throw std::invalid_argument("name not found.")
		}
	}

	std::map<std::string, timer> m_timers;
	std::map<std::string, gauge> m_gauges;
	std::map<std::string,counter> m_counters;
};
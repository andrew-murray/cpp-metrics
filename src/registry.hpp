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

	counter& get_counter(const std::string& name){
		auto it = m_counters.find(name);
		if(it!=m_counters.end()){
			return it->second;
		} else {
			m_counters[name] = counter();
			return m_counters[name];
		}
	}

	void increment_count(const std::string& name,const int& val = 1){
		auto& current_counter = get_counter(name);
		current_counter.increment(val);
	}

	void decrement_count(const std::string& name,const int& val = 1){
		auto& current_counter = get_counter(name);
		current_counter.decrement(val);
	}
private:
	std::map<const std::string, gauge> m_gauges;
	std::map<const std::string, counter> m_counters;
	std::map<const std::string, meter> m_meters;
};
#pragma once
#include "instruments/counter.hpp"
#include "instruments/gauge.hpp"
#include "instruments/meter.hpp"
#include <map>

namespace metrics {

	class registry {
	public:
		registry(const registry&) = delete;
		registry(const registry&&) = delete;
		registry& operator = (const registry&) = delete;
		registry& operator = (const registry&&) = delete;

		registry(){}

		instruments::counter& counter(const std::string& name){
			return m_counters[name];
		}

		template<typename T, typename = typename std::enable_if<!std::is_same<T,double>::value >::type>
		instruments::gauge& gauge(const std::string& name,const std::function<T(void)>& func){
			return m_gauges.emplace(name,func).first->second;
		}

		instruments::gauge& gauge(const std::string& name,const std::function<double(void)>& func){
			return m_gauges.emplace(name,func).first->second;
		}

		instruments::meter& meter(const std::string& name){
			return m_meters[name];
		}

	private:
		std::map<const std::string, instruments::gauge> m_gauges;
		std::map<const std::string, instruments::counter> m_counters;
		std::map<const std::string, instruments::meter> m_meters;
	public:

		auto gauges() -> decltype((m_gauges)){
			return m_gauges;
		}

		auto counters() -> decltype((m_counters)){
			return m_counters;
		}


		auto meters() -> decltype((m_meters)){
			return m_meters;
		}
	};
}
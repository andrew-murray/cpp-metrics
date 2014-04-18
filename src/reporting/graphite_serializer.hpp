#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <fstream>
#include <memory>
#include "boost_serializing.hpp"
#include "utils/regular_updater_mixin.hpp"
#include "registry.hpp"
#include "boost_serializer.hpp"
#include <deque>
#include <boost/network/include/http/client.hpp>

namespace network = boost::network;

namespace metrics {
	namespace reporting {

		template<typename ClockType,typename StreamType = std::ofstream>
		class graphite_reporter {
		public:
			template <typename T = std::chrono::seconds, typename = typename std::enable_if<std::is_same<StreamType,std::ofstream>::value>::type>
		  	graphite_reporter(const std::string& filename, metrics::registry& reg,const T& interval = std::chrono::seconds(20))
		  	: m_registry(reg)
		  	, m_get_updater(interval,[this](){this->queue_strings();})
		  	, m_post_updater(interval,[this](){this->post_strings();})
		  	{
		  		m_get_updater.begin_updates();
		  		m_post_updater.begin_updates();
		  	}

			template <typename T = std::chrono::seconds>
		  	graphite_reporter(StreamType& stream, metrics::registry& reg,const T& interval = std::chrono::seconds(20))
		  	: m_registry(reg)
		  	, m_get_updater(interval,[this](){this->queue_strings();})
		  	, m_post_updater(interval,[this](){this->post_strings();})
		  	{
		  		m_get_updater.begin_updates();
		  		m_post_updater.begin_updates();
		  	}

		  	void queue_strings(){
		  		// note: this function should probably capture
		  		// state up front - then do the work to serialize it
		  		std::stringstream output;
		  		auto&& tp = std::chrono::duration_cast<std::chrono::seconds>(m_clock.now().time_since_epoch());

		  		for(auto&& result : m_registry.counters()){ 
		  			output << result.first << " " << result.second.count() << " " << tp.count();
		  			m_string_queue.push_back(output.str());
		  			output.str("");
		  		}

		  		for(auto&& result : m_registry.gauges()){
		  			output << result.first << " " << result.second.get_value() << " " << tp.count();
		  			m_string_queue.push_back(output.str());
		  			output.str(" ");
		  		}
		  		for(auto&& result : m_registry.meters()){
		  			output << result.first << " " << result.second.mean_rate() << " " << tp.count();
		  			m_string_queue.push_back(output.str());
		  			output.str(" ");
		  		}
		  	}

		  	void post_strings(){
		  		// on each update empty the queue
		  		// really want a "wake-on-message" system
	  			while(!m_string_queue.empty()){
	  				std::cout << "doing a thing" << std::endl;
	  				std::cout << m_string_queue[0] << std::endl;
	  				//network::http::client::request req("http://localhost:2003");
	  				//m_client.post(req,m_string_queue[0]);
	  				system((std::string("echo ") + m_string_queue[0]+std::string("| nc -q0 localhost 2003")).c_str());
	  				m_string_queue.pop_front();
	  				std::cout << "done a thing" << std::endl;
	  			}
		  	}

		  	virtual ~graphite_reporter(){
		  		std::cout << "time to die" << std::endl;
		  		m_get_updater.halt_updates();
		  		m_post_updater.halt_updates();
		  	}
		 private:
		  	metrics::registry& m_registry;
		  	std::deque<std::string> m_string_queue;
		  	metrics::utils::regular_updater<> m_get_updater;
		  	metrics::utils::regular_updater<> m_post_updater;
		  	ClockType m_clock;
		  	network::http::client m_client;
		  };
	}
}
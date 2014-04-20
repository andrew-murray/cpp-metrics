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
#include <boost/asio.hpp>

namespace metrics {
	namespace reporting {

		template<typename ClockType>
		class graphite_reporter {
		public:
			template <typename T = std::chrono::seconds>
		  	graphite_reporter(const std::string& host, const std::string& port,metrics::registry& reg,const T& interval = std::chrono::seconds(20))
		  	: m_registry(reg)
		  	, m_get_updater(interval,[this](){this->queue_strings();})
		  	, m_post_updater(interval,[this](){this->post_strings();})
		  	, m_socket(m_io_service)
		  	{
		  		using boost::asio::ip::tcp;
	  			tcp::resolver::query query(host,port,boost::asio::ip::resolver_query_base::numeric_service);
	  			tcp::resolver resolver(m_io_service);
	  			auto&& endpoint_iterator = resolver.resolve(query);
    			boost::asio::connect(m_socket, endpoint_iterator);
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
	  			while(!m_string_queue.empty()){
	  				boost::asio::streambuf req;
	  				std::ostream stream(&req);
	  				stream << m_string_queue[0] << std::endl;
    				boost::asio::write(m_socket,req);
	  				m_string_queue.pop_front();
	  			}
		  	}

		  	virtual ~graphite_reporter(){
		  		m_get_updater.halt_updates();
		  		m_post_updater.halt_updates();
		  	}
		 private:
		  	metrics::registry& m_registry;
		  	std::deque<std::string> m_string_queue;
		  	metrics::utils::regular_updater<> m_get_updater;
		  	metrics::utils::regular_updater<> m_post_updater;
		  	ClockType m_clock;
		  	boost::asio::io_service m_io_service;
		  	boost::asio::ip::tcp::socket m_socket;
		  };
	}
}
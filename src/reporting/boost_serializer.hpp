#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <fstream>
#include <memory>
#include "boost_serializing.hpp"
#include "utils/regular_updater.hpp"
#include "registry.hpp"

namespace metrics {
	namespace reporting {
		
		namespace impl {
			template <typename T, typename... Args>
			inline auto make_unique(Args&&... args) -> std::unique_ptr<T>
			{
			    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
			}
		}


		template<typename ArchiveType,typename StreamType = std::ofstream>
		class regular_boost_reporter {
		public:
			template <typename T = std::chrono::seconds, typename = typename std::enable_if<std::is_same<StreamType,std::ofstream>::value>::type>
		  	regular_boost_reporter(const std::string& filename, metrics::registry& reg,const T& interval = std::chrono::seconds(20))
		  	: m_registry(reg)
		  	, m_stream(impl::make_unique<std::ofstream>(filename))
		  	, m_archive(*m_stream.get())
		  	, m_updater(interval,[this](){this->serialize();})
		  	{
		  		m_updater.begin_updates();
		  	}

			template <typename T = std::chrono::seconds>
		  	regular_boost_reporter(StreamType& stream, metrics::registry& reg,const T& interval = std::chrono::seconds(20))
		  	: m_registry(reg)
		  	, m_archive(stream)
		  	, m_updater(interval,[this](){this->serialize();})
		  	{
		  		m_updater.begin_updates();
		  	}

		  	void serialize(){
		  		// note: this function should probably capture
		  		// state up front - then do the work to serialize it
		  		m_archive << boost::serialization::make_nvp("counters",m_registry.counters());
		  		m_archive << boost::serialization::make_nvp("gauges",m_registry.gauges());
		  		m_archive << boost::serialization::make_nvp("meters",m_registry.meters());

		  		/*
		  		for(auto&& string_gauge_pair : m_registry.gauges()){
		  			m_archive & string_gauge_pair.first;
		  			m_archive & string_gauge_pair.second;
		  		}
		  		*/
		  		/*
		  		for(auto&& string_counter_pair : m_registry.counters()){
		  			m_archive << string_counter_pair.first;
		  			m_archive << string_counter_pair.second;
		  		}
		  		*/
		  	}

		  	virtual ~regular_boost_reporter(){
		  		m_updater.halt_updates();
		  	}
		 private:
		  	metrics::registry& m_registry;
		  	std::unique_ptr<StreamType> m_stream;
		  	ArchiveType m_archive;
		  	metrics::utils::regular_updater<> m_updater;
		  };
	}

	typedef reporting::regular_boost_reporter<boost::archive::text_oarchive> text_reporter;
}
#include "instruments/counter.hpp"
#include "instruments/gauge.hpp"
#include "instruments/histogram.hpp"
#include "instruments/meter.hpp"
#include "boost/serialization/split_free.hpp"
#include <boost/archive/xml_oarchive.hpp>
#include <type_traits>

BOOST_SERIALIZATION_SPLIT_FREE(metrics::instruments::counter)
BOOST_SERIALIZATION_SPLIT_FREE(metrics::instruments::gauge)
BOOST_SERIALIZATION_SPLIT_FREE(metrics::instruments::meter)
BOOST_SERIALIZATION_SPLIT_FREE(metrics::snapshot)

namespace boost {
	namespace serialization {

	template<typename Archive,typename T>
	void  save_impl(Archive& archive,const std::string& name,T&& val){
		archive & boost::serialization::make_nvp(name.c_str(),val);
	}

	template<class Archive>
	void save(Archive& archive, const metrics::instruments::counter & counter, const unsigned int /*version*/)
	{
		save_impl(archive,"count",counter.count());
	}
	
	template<class Archive>
	void load(Archive& archive, metrics::instruments::counter& counter, const unsigned int /*version*/)
	{
	    throw std::invalid_argument("metrics::instruments::counter is not loadable.");
	}



	template<class Archive>
	void save(Archive& archive, const metrics::instruments::gauge & gauge, const unsigned int /*version*/)
	{
		save_impl(archive,"value",gauge.get_value());
	}
	
	template<class Archive>
	void load(Archive& archive, metrics::instruments::gauge & gauge, const unsigned int /*version*/)
	{
	    throw std::invalid_argument("metrics::instruments::gauge is not loadable.");
	}

#if 0
	template<class Archive>
	void save(Archive& archive, const metrics::instruments::histogram& histogram, const unsigned int /*version*/)
	{
	    archive & gauge.get_value();
	}
	
	template<class Archive>
	void load(Archive& archive, metrics::instruments::histogram& histogram, const unsigned int /*version*/)
	{
	    static_assert(false,"metrics::instruments::histogram is not loadable.");
	}


	BOOST_SERIALIZATION_SPLIT_FREE(metrics::instruments::histogram)
#endif

	template<class Archive>
	void save(Archive& archive, const metrics::instruments::meter& meter, const unsigned int /*version*/)
	{
		save_impl(archive,std::string("mean_rate"),meter.mean_rate());
	    save_impl(archive,std::string("one_minute_rate"),meter.one_minute_rate());
	    save_impl(archive,std::string("five_minute_rate"),meter.five_minute_rate());
	    save_impl(archive,std::string("fifteen_minute_rate"),meter.fifteen_minute_rate());
	}
	
	template<class Archive>
	void load(Archive& archive, metrics::instruments::meter& meter, const unsigned int /*version*/)
	{
	    throw std::invalid_argument("metrics::instruments::meter is not loadable.");
	}



	template<class Archive>
	void save(Archive& archive, const metrics::snapshot& snapshot, const unsigned int /*version*/)
	{
		/*
	    archive & std::string("min");
	    archive & snapshot.min();
	    archive & std::string("max");
	    archive & snapshot.max();
	    archive & std::string("mean");
	    archive & snapshot.mean();
	    archive & std::string("95th_quantile");
	    archive & snapshot.quantile(0.95);
	    archive & std::string("sample_size");
	    archive & snapshot.size();
	    archive & std::string("standard_deviation");
	    archive & snapshot.std_dev();
	    */
	}
	
	template<class Archive>
	void load(Archive& archive, metrics::snapshot& snapshot, const unsigned int /*version*/)
	{
	    throw std::invalid_argument("metrics::snapshot is not loadable.");
	}

	} // namespace serialization
} // namespace boost
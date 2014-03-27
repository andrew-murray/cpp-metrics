#include <boost/test/unit_test.hpp>
#include "moving_average.hpp"

template<typename ewma>
void mock_minute(ewma& avg){
	typedef std::chrono::duration<double, std::chrono::minutes::period> fp_minutes_type;
	fp_minutes_type interval_in_mins(avg.interval());
	double ticks = 1.0 / interval_in_mins.count();
	BOOST_ASSERT((ticks - (int) ticks) < 0.01);
	for(int i = 0; i < (int)ticks; ++i){
		avg.tick();
	}
}

// test cases from java-metrics to ensure consistency
BOOST_AUTO_TEST_CASE(one_minute_ewma_test_from_java){
	moving_average ewma(std::chrono::minutes(1),std::chrono::seconds(5));
    ewma.mark(3);
    ewma.tick();

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.6,1e-6);

    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.22072766,1e-3);

    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.08120117,1e-3);

    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.02987224,1e-3);

    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.01098938,1e-3);

    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00404277,1e-3);

    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00148725,1e-3);

    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00054713,1e-3);

    mock_minute(ewma);
    

    // TODO: Generate more precise data than is provided by the java tests

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00020128,1);

    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00007405,1);

	mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00002724,1);
    
    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00001002,1);
    
    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00000369,1);
    
    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00000136,1);
    
    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00000050,1);
            
    mock_minute(ewma);
    
    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.00000018,2);

}

// test cases from java-metrics to ensure consistency
BOOST_AUTO_TEST_CASE(five_minute_ewma_test_from_java){
	moving_average ewma(std::chrono::minutes(5),std::chrono::seconds(5));
    ewma.mark(3);
    ewma.tick();

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.6, 1e-6);

    mock_minute(ewma);	

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.49123845, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.40219203, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.32928698, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.26959738, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.22072766, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.18071653, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.14795818, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.12113791, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.09917933, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.08120117, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.06648190, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.05443077, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.04456415, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.03648604, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.02987224, 1e-3);

}

// test cases from java-metrics to ensure consistency
BOOST_AUTO_TEST_CASE(fifteen_minute_ewma_test_from_java){
	moving_average ewma(std::chrono::minutes(15),std::chrono::seconds(5));
    ewma.mark(3);
    ewma.tick();

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.6, 1e-6);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.56130419, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.52510399, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.49123845, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.45955700, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.42991879, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.40219203, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.37625345, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.35198773, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.32928698, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.30805027, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.28818318, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.26959738, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.25221023, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.23594443, 1e-3);

    mock_minute(ewma);

    BOOST_CHECK_CLOSE(ewma.rate(std::chrono::seconds(1)),0.22072766, 1e-3);

}
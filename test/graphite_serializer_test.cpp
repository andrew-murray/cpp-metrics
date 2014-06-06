#include <iostream>
#include <thread>
#include <future>
#include <boost/test/unit_test.hpp>
#include "reporting/graphite_serializer.hpp"
#include <boost/archive/text_iarchive.hpp>

namespace mock {

	std::vector<std::string> recorded_messages;

	using boost::asio::ip::tcp;


	// Code example mutilated from boost::asio tutorials

	class session
	  : public std::enable_shared_from_this<session>
	{
	public:
	
	  session(tcp::socket socket)
	    : socket_(std::move(socket))
	  {
	  }

	  void start()
	  {
	    do_read();
	  }

	private:
	  void do_read()
	  {
	    auto self(shared_from_this());
	    socket_.async_read_some(boost::asio::buffer(data_, max_length),
	        [this, self](boost::system::error_code ec, std::size_t length)
	        {
	          if (!ec)
	          {
	          	std::string received(data_,length);
	            recorded_messages.push_back(received);
	            do_read();
	          }
	        });
	  }

	  tcp::socket socket_;
	  enum { max_length = 1024 };
	  char data_[max_length];
	};

	class server
	{
	public:
	  server(boost::asio::io_service& io_service, short port)
	    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
	      socket_(io_service)
	  {
	    do_accept();
	  }

	private:
	  void do_accept()
	  {
	    acceptor_.async_accept(socket_,
	        [this](boost::system::error_code ec)
	        {
	          if (!ec)
	          {
	            std::make_shared<session>(std::move(socket_))->start();
	          }

	          do_accept();
	        });
	  }

	  tcp::acceptor acceptor_;
	  tcp::socket socket_;
	};
}

int count_in_strings(const std::vector<std::string> strings, const std::string& term){
	int count = 0;
	for(auto& str : strings){
		if(str.find(term) != std::string::npos){
			++count;
		}
	}
	return count;
}

BOOST_AUTO_TEST_CASE(metrics_graphite_reporter_test){
	metrics::registry reg;
	reg.counter("simple.counter.for_serializing").mark(10879);
	reg.counter("another.counter.for_serializing").mark(30091);
	reg.meter("another.meter.why_not").mark(1150);
	reg.gauge("look.at.this.gauge",[&](){return 999.0f;});

    boost::asio::io_service io_service;
    mock::server s(io_service, 8902);

    const size_t recorded_messages_required = 20;

    auto server_thread = [&](){
    	while(mock::recorded_messages.size() < recorded_messages_required){
    		io_service.poll(); 
    	}
    };

    auto server = std::async(std::launch::async,server_thread);

    typedef metrics::reporting::graphite_reporter<std::chrono::high_resolution_clock> g_reporter;
	g_reporter a("localhost","8902",reg,std::chrono::milliseconds(20));
	
	// wait for enough messages 
	server.get();

	BOOST_ASSERT(mock::recorded_messages.size() >= recorded_messages_required);
	BOOST_CHECK_GT(count_in_strings(mock::recorded_messages,"simple.counter.for_serializing"),0);
	BOOST_CHECK_GT(count_in_strings(mock::recorded_messages,"another.counter.for_serializing"),0);
	BOOST_CHECK_GT(count_in_strings(mock::recorded_messages,"another.meter.why_not"),0);
	BOOST_CHECK_GT(count_in_strings(mock::recorded_messages,"look.at.this.gauge"),0);

	BOOST_CHECK_GT(count_in_strings(mock::recorded_messages,"10879"),0);
	BOOST_CHECK_GT(count_in_strings(mock::recorded_messages,"30091"),0);
	BOOST_CHECK_GT(count_in_strings(mock::recorded_messages,"999"),0);
}
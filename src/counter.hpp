#include <atomic>

class counter {
public:
	counter() 
		: count(std::make_shared<std::atomic<unsigned int>>(0))
	{

	}

	void increment(const int& val = 1){
		(*count)+= val;
	}

	void decrement(const int& val = 1){
		(*count)-= val;		
	}

	unsigned int value(){
		return (unsigned int) *count;
	}
private:
	std::shared_ptr<std::atomic<unsigned int>> count ;
};
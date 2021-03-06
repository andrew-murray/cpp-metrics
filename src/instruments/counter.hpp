#pragma once
#include <atomic>
#include <memory>

namespace metrics {
	namespace instruments {

		class counter {
		public:
			counter() 
				: m_count(std::make_shared<std::atomic<unsigned int>>(0))
			{

			}

			void increment(const int& val = 1){
				(*m_count)+= val;
			}

			void decrement(const int& val = 1){
				(*m_count)-= val;		
			}

			void mark(const int& val = 1){
				increment(val);
			}

			unsigned int count() const {
				return (unsigned int) *m_count;
			}
		private:
			std::shared_ptr<std::atomic<unsigned int>> m_count ;
		};
		
	}
}
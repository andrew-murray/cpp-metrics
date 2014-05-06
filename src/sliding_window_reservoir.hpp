#pragma once
#include <atomic>
#include "reservoir.hpp"

namespace metrics{
	class sliding_window_reservoir : public reservoir {
	public:
		sliding_window_reservoir(const int& sz)
		: m_values(sz),
		  m_count(0)
		{
		}

		size_t size() const {
			return std::min(m_values.size(),(size_t)m_count);
		}

		virtual void mark(const int& n){
			size_t local_count = m_count++;
			if(local_count < m_values.size()){
				m_values[local_count] = n;
			} else {
				m_values[local_count % m_values.size()] = n;
			}
		}

		virtual snapshot get_snapshot() const {
			int sz = size();
			std::vector<int> vec(m_values.begin(), m_values.begin() + sz);
			return snapshot(vec);
		}

	private:

		std::atomic<size_t> m_count;
		std::vector<int> m_values;
	};
}
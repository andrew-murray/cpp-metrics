#pragma once
#include <atomic>
#include "reservoir.hpp"
#include <cstdlib>

namespace metrics{
	class random_reservoir : public reservoir {
	public:
		random_reservoir(const int& size)
		: m_values(size),
		  m_count(0)
		{
		}

		size_t size() const {
			return std::min(m_values.size(),(size_t)m_count);
		}

		virtual void mark(const int& n){
			size_t local_count = m_count;
			if(local_count < m_values.size()){
				m_values[local_count] = n;
				m_count = std::min(((size_t)m_count)+1,m_values.size());
			} else {
				const int to_replace = (int)((std::rand() / (double)RAND_MAX)+0.5);
				m_values[to_replace] = n;
			}
		}

		virtual snapshot get_snapshot() const {
			std::vector<int> snap(m_values.begin(), m_values.begin()+m_count);
			return snapshot(snap);
		}

	private:
		std::atomic<size_t> m_count;
		std::vector<int> m_values;
	};
}
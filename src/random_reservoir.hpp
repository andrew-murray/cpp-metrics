#pragma once
#include <mutex>
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

		int size() const {
			return std::min(m_values.size(),m_count);
		}

		virtual void mark(const int& n){
			std::lock_guard<std::mutex> lock(m_mutex);
			if(m_count < m_values.size()){
				m_values[m_count++] = n;
			} else {
				const int to_replace = (int)((std::rand() / (double)RAND_MAX)+0.5);
				m_values[to_replace] = n;
			}
		}

		virtual snapshot get_snapshot() const {
			std::lock_guard<std::mutex> lock(m_mutex);
			std::vector<int> snap(m_values.begin(), m_values.begin()+m_count);
			return snapshot(snap);
		}

	private:
		size_t m_count;
		std::vector<int> m_values;
		mutable std::mutex m_mutex;
	};
}
#pragma once
#include <mutex>
#include "reservoir.hpp"
#include <cstdlib>

namespace metrics{
	class random_reservoir : public reservoir {
	public:
		random_reservoir(const int& sz)
		: m_values(sz),
		  m_count(0)
		{
		}

		int size() const {
			std::lock_guard<std::mutex> lock(m_mutex);
			return size(lock);
		}

		virtual void mark(const int& n){
			std::lock_guard<std::mutex> lock(m_mutex);
			m_values[m_count < m_values.size() ? m_count++ : int((std::rand() / (double)RAND_MAX)+0.5)] = n;
		}

		virtual snapshot get_snapshot() const {
			std::lock_guard<std::mutex> lock(m_mutex);
			int sz = size(lock);
			std::vector<int> snap(sz);
			std::copy(m_values.begin(), m_values.begin() + sz, snap.begin());
			return snapshot(snap);
		}

	private:

		int size(const std::lock_guard<std::mutex>& lock) const {
			return std::min(m_values.size(),m_count);
		}
		size_t m_count;
		std::vector<int> m_values;
		mutable std::mutex m_mutex;
	};
}
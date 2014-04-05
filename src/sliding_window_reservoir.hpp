#include <mutex>
#include "reservoir.hpp"

class sliding_window_reservoir : public reservoir {
public:
	sliding_window_reservoir(const int& sz)
	: m_values(sz),
	  m_count(0)
	{
	}

	int size() const {
		std::lock_guard<std::mutex> lock(m_mutex);
		return std::min(m_values.size(),m_count);
	}

	virtual void mark(const int& n){
		std::lock_guard<std::mutex> lock(m_mutex);
		m_values[m_count++ % m_values.size()] = n;
	}

	virtual snapshot get_snapshot() const {
		std::lock_guard<std::mutex> lock(m_mutex);
		int sz = size();
		std::vector<int> snap(sz);
		std::copy(m_values.begin(), m_values.begin() + sz, snap.begin());
		return snapshot(snap);
	}

private:
	size_t m_count;
	std::vector<int> m_values;
	mutable std::mutex m_mutex;
};
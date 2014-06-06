#pragma once
#include <vector>
#include <algorithm>
#include <numeric>

namespace metrics {
    class snapshot {
    public:
        template<typename T>
        snapshot(T&& container)
        : m_values(container.size())
        {
        	std::copy(container.begin(),container.end(),m_values.begin());
        	std::sort(m_values.begin(),m_values.end());
        }

        double quantile(const double& y) const {
            if (m_values.size() == 0) {
                return 0.0;
            }

            /*

            const double pos = quantile * (m_values.size() + 1);

            I am entirely unconvinced by this logic
            What's the grounding for it at all?

            ruby-metrics provides a reference

    		this one

    		http://www.itl.nist.gov/div898/handbook/prc/section2/prc252.htm

    		claiming Excel and R calculate quantiles in this way.

    		I am more less convinced than before I found this link.

            if (pos < 1) {
                return values[0];
            }

            if (pos >= m_values.size()) {
                return values[m_values.size() - 1];
            }
            */

            // Note : this method ~ if without bugs
            // just does linear interp to attempt to find the right threshold

            const double pos = y  * (m_values.size()-1);
            if(pos >= m_values.size()-1){return double(m_values.back());}
            const double pos_floor = floor(pos);

            const int& lower = m_values[(int)pos_floor];
            const int& upper = m_values[(int)pos_floor + 1];
            return lower + (pos - pos_floor) * (upper - lower);
        }

        size_t size() const {
        	return m_values.size();
        }

        double getMedian() const {
            return quantile(0.5);
        }

        double get75thPercentile() const {
            return quantile(0.75);
        }

        double get95thPercentile() const {
            return quantile(0.95);
        }

        double get98thPercentile() const {
            return quantile(0.98);
        }

        double get99thPercentile() const {
            return quantile(0.99);
        }

        double get999thPercentile() const {
            return quantile(0.999);
        }

        std::vector<int> values() const {
            return m_values;
        }

        int max() const {
            if (m_values.size() == 0) {
                return 0;
            }
            return m_values.back();
        }

        int min() const {
            if (m_values.size() == 0) {
                return 0;
            }
            return m_values.front();
        }

        /**
         * Returns the arithmetic mean of the values in the snapshot.
         *
         * @return the arithmetic mean
         */
        double mean() const {
        	return double(std::accumulate(m_values.begin(),m_values.end(),0)) / m_values.size();
        }

        double std_dev() const {
            // two-pass algorithm for variance, avoids numeric overflow

            if (m_values.size() <= 1) {
                return 0;
            }

            double local_mean = mean();
            double sum = 0;

            for (auto value : m_values) {
                double diff = value - local_mean;
                sum += diff * diff;
            }

            // -1 ? Orly? the old n-1 gag?
            double variance = sum / (m_values.size() - 1);
            return sqrt(variance);
        }

        std::vector<int> m_values;
    };
}
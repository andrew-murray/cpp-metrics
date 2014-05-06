#pragma once
#include <atomic>
#include "sliding_window_reservoir.hpp"

namespace metrics {
    namespace instruments {

        class histogram {
        public:
            histogram() 
            : m_count(std::make_shared<std::atomic<size_t>>(0))
            , m_reservoir(std::dynamic_pointer_cast<reservoir>(std::make_shared<sliding_window_reservoir>(100)))
            {

            }

            int count() const {
                return (int) *m_count;
            }

            void mark(const int& value){
                m_reservoir->mark(value);
                (*m_count)++;
            }

            snapshot get_snapshot() const {
                return m_reservoir->get_snapshot((size_t)*m_count);
            }

        private:
            std::shared_ptr<reservoir> m_reservoir;
            std::shared_ptr<std::atomic<size_t>> m_count;
        };
    
    }
}

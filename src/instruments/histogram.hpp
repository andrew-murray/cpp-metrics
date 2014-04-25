#pragma once
#include <atomic>
#include "sliding_window_reservoir.hpp"

namespace metrics {
    namespace instruments {

        class histogram {
        public:
            histogram() 
            : m_count(std::make_shared<std::atomic<int>>(0))
            , m_reservoir(std::dynamic_pointer_cast<reservoir>(std::make_shared<sliding_window_reservoir>(100)))
            {

            }

            int count() const {
                return (int) *m_count;
            }

            void mark(const int& value){
                (*m_count)++;
                m_reservoir->mark(value);
            }

            snapshot get_snapshot() const {
                return m_reservoir->get_snapshot();
            }

        private:
            std::shared_ptr<reservoir> m_reservoir;
            std::shared_ptr<std::atomic<int>> m_count;
        };
    
    }
}

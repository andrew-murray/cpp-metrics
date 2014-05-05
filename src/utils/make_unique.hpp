#pragma once
#include <memory>
#include <utility>

namespace metrics{	
	namespace utils {

		template <typename T, typename... Args>
		inline auto make_unique(Args&&... args) -> std::unique_ptr<T>
		{
		    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
		}
		
	}
}

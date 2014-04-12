#pragma once
#include "snapshot.hpp"

class reservoir {
public:
	virtual void mark(const int&) = 0;
	virtual snapshot get_snapshot() const = 0;
};

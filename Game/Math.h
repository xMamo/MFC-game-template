#pragma once

#include <cmath>

template<typename T>
T round(T x) {
	return x >= static_cast<T>(0.0L) ? floor(x + static_cast<T>(0.5L)) : ceil(x - static_cast<T>(0.5L));
}

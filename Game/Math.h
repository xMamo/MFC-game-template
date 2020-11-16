#pragma once

#include <d2d1helper.h>
#include <cmath>

template<typename T>
T round(T x) {
	return x >= static_cast<T>(0.0L) ? floor(x + static_cast<T>(0.5L)) : ceil(x - static_cast<T>(0.5L));
}

D2D1::Matrix3x2F rotation(FLOAT angle, D2D1_POINT_2F center = D2D1::Point2F());

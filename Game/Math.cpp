#include "Math.h"

D2D1::Matrix3x2F rotation(FLOAT angle, D2D1_POINT_2F center) {
	angle *= atanf(1.0F) / 45.0F;

	auto sin = sinf(angle);
	auto cos = cosf(angle);
	auto x = center.x;
	auto y = center.y;

	return D2D1::Matrix3x2F(
		cos, sin,
		-sin, cos,
		x * (1.0F - cos) + y * sin, y * (1.0F - cos) - x * sin
	);
}

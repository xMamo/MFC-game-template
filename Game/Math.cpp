#include "Math.h"

D2D1::Matrix3x2F rotation(FLOAT angle, D2D1_POINT_2F center) {
	// Conversion to radians. Mathematically,
	// rad = deg * (PI / 180)
	// rad = deg * ((4 * tan(1)) / 180)
	// rad = deg * (tan(1) / 45)

	angle *= atanf(1.0F) / 45.0F;

	// Rotation matrix: calculated as the composition of three transformations:
	// 1. Translation by (-x, -y): ((1, 0, 0), (0, 1, 0), (-x, -y, 1));
	// 2. Rotation by angle: ((cos, sin, 0), (-sin, cos, 0), (0, 0, 1));
	// 3. Translation by (x, y): ((1, 0, 0), (0, 1, 0), (x, y, 1)).

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

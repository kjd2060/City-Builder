#include "KMath.hpp"

namespace KMath
{
	double lerp(float value, float start, float end)
	{
		return start + (end - start) * value;
	}
}

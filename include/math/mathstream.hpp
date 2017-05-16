#ifndef MATHSTREAM_HPP

#include <iostream>

#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include "math/mat2.hpp"
#include "math/mat3.hpp"
#include "math/mat4.hpp"

std::ostream& operator<<(std::ostream& os, const vec2 &_v);
std::ostream& operator<<(std::ostream& os, const vec3 &_v);
std::ostream& operator<<(std::ostream& os, const mat2 &_v);
std::ostream& operator<<(std::ostream& os, const mat3 &_v);
//std::ostream& operator<<(std::ostream& os, const mat4 &_v);

#endif

#ifndef MATHSTREAM_HPP

#include <iostream>

#include "math/ang3.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include "math/mat2.hpp"
#include "math/mat3.hpp"
#include "math/mat4.hpp"

std::ostream& operator<<(std::ostream& os, const vec2 &_v);
std::ostream& operator<<(std::ostream& os, const vec3 &_v);
std::ostream& operator<<(std::ostream& os, const mat2 &_v);
std::ostream& operator<<(std::ostream& os, const mat3 &_v);
std::ostream& operator<<(std::ostream& os, const ang3 &_v);

#endif

#include <cmath>
#include <iostream>

#include "vector2.hpp"

Vector2::Vector2(const std::array<float, 2> &vector)
{
    this->vec = vector;
}

Vector2::Vector2(const float x, const float y)
{
    this->vec = {x, y};
}

const float* Vector2::data(void) const noexcept
{
    return this->vec.data();
}

const float Vector2::x() const
{
    return vec[0];
}

const float Vector2::y() const
{
    return vec[1];
}

Vector2 Vector2::normalize(Vector2 vec)
{
    float unit = 0;
    for(const float &f : vec.vec) {
        unit += f*f;
    }
    unit = std::sqrt(unit);
    for (size_t i = 0; i < vec.vec.size(); ++i) {
        vec.vec[i] /= unit;
    }
    return vec;
}

Vector2& Vector2::operator*=(const float f)
{
    for (size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] *= f;
    }
    return *this;
}

Vector2& Vector2::operator+=(const Vector2 &vec2)
{
    for (size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] += vec2.vec[i];
    }
    return *this;
}

Vector2& Vector2::operator-=(const Vector2 &vec2)
{
    for (size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] -= vec2.vec[i];
    }
    return *this;
}

float Vector2::operator[](const std::size_t i) const
{
    if (i > this->vec.size()) {
        throw std::range_error("Vector2 has only 2 elements");
    }
    return this->vec[i];
}

Vector2 operator*(Vector2 lhs, const float rhs)
{
    lhs *= rhs;
    return lhs;
}

Vector2 operator*(const float lhs, Vector2 rhs)
{
    rhs *= lhs;
    return rhs;
}

Vector2 operator+(Vector2 lhs, const Vector2 &rhs)
{
    lhs += rhs;
    return lhs;
}

Vector2 operator-(Vector2 lhs, const Vector2 &rhs)
{
    lhs -= rhs;
    return lhs;
}

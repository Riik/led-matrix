#include <iostream>

#include "vector3.hpp"

Vector3::Vector3(float x, float y, float z)
{
    this->vec = {x, y, z};
}

bool Vector3::operator==(const Vector3& other) const
{
    for (std::size_t i = 0; i < this->vec.size(); ++i) {
        if (this->vec[i] != other.vec[i]) {
            return false;
        }
    }
    return true;
}

bool Vector3::operator!=(const Vector3& other) const
{
    return !this->operator==(other);
}

Vector3& Vector3::operator*=(float f)
{
    for (std::size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] *= f;
    }
    return *this;
}

Vector3& Vector3::operator+=(const Vector3 &other)
{
    for (std::size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] += other.vec[i];
    }
    return *this;
}

Vector3& Vector3::operator-=(const Vector3 &other)
{
    for (std::size_t i = 0; i < this->vec.size(); ++i) {
        this->vec[i] -= other.vec[i];
    }
    return *this;
}

float Vector3::operator[](const std::size_t i) const
{
    if (i > this->vec.size()) {
        throw std::range_error("Index out of range");
    }
    return this->vec[i];
}

Vector3 operator*(Vector3 lhs, float rhs)
{
    lhs *= rhs;
    return lhs;
}

Vector3 operator*(float lhs, Vector3 rhs)
{
    rhs *= lhs;
    return rhs;
}

Vector3 operator+(Vector3 lhs, const Vector3 &rhs)
{
    lhs += rhs;
    return lhs;
}

Vector3 operator-(Vector3 lhs, const Vector3 &rhs)
{
    lhs -= rhs;
    return lhs;
}

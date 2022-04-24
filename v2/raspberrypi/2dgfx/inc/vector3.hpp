#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <array>

/**Represents a 3x1 vector of type float.*/
class Vector3 {
    protected:
        std::array<float, 3> vec;
    public:
        /**Construct a Vector3 from up to four values. Also serves as the default constructor.*/
        Vector3(float x = 0, float y = 0, float z = 0);

        bool operator==(const Vector3& other) const;
        bool operator!=(const Vector3& other) const;

        /**@return The value at position i.
         * @warning Throws range_error if i > 3*/
        float operator[](std::size_t i) const;

        /**Multiply every element of this Vector3 with f.*/
        Vector3& operator*=(float f);
        /**Add another Vector3 to this one.*/
        Vector3& operator+=(const Vector3 &other);
        /**Subtract another Vector3 from this one.*/
        Vector3& operator-=(const Vector3 &other);
};
/**Create a new Vector3 from multiplication. See also: Vector3::operator*=.*/
Vector3 operator*(Vector3 lhs, float rhs);
/**Create a new Vector3 from multiplication. See also: Vector3::operator*=.*/
Vector3 operator*(float lhs, Vector3 rhs);
/**Create a new Vector3 from addition. See also: Vector3::operator+=.*/
Vector3 operator+(Vector3 lhs, const Vector3 &rhs);
/**Create a new Vector3 from subtraction. See also: Vector3::operator-=.*/
Vector3 operator-(Vector3 lhs, const Vector3 &rhs);

#endif //VECTOR3_HPP

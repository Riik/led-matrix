#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <array>

/**Represents a 2x1 vector of type float.*/
class Vector2 {
    protected:
        std::array<float, 2> vec;
    public:
        /**Construct a Vector2 from up to four values. Also serves as the default constructor.*/
        Vector2(const float x = 0, const float y = 0);

        bool operator==(const Vector2& other) const;
        bool operator!=(const Vector2& other) const;
        /**Return a pointer to a c-style array containing the current data in this vector.
         * See also: std::array::data.*/
        const float* data(void) const noexcept;

        /**@return vec[0]*/
        const float x() const;
        /**@return vec[1]*/
        const float y() const;
        /**@return The value at position i.
         * @warning Throws range_error if i > 2*/
        float operator[](const std::size_t i) const;

        /**Normalize the vector. A normalized vector has a length of 1 (sqrt(a^2 + b^2 + c^2 + ..) = 1).*/
        static Vector2 normalize(Vector2 vec);
        /**Multiply every element of this Vector2 with f.*/
        Vector2& operator*=(const float f);
        /**Add another Vector2 to this one.*/
        Vector2& operator+=(const Vector2 &vec4);
        /**Subtract another Vector2 from this one.*/
        Vector2& operator-=(const Vector2 &vec4);
};
/**Create a new Vector2 from multiplication. See also: Vector2::operator*=.*/
Vector2 operator*(Vector2 lhs, const float rhs);
/**Create a new Vector2 from multiplication. See also: Vector2::operator*=.*/
Vector2 operator*(const float lhs, Vector2 rhs);
/**Create a new Vector2 from addition. See also: Vector2::operator+=.*/
Vector2 operator+(Vector2 lhs, const Vector2 &rhs);
/**Create a new Vector2 from subtraction. See also: Vector2::operator-=.*/
Vector2 operator-(Vector2 lhs, const Vector2 &rhs);

#endif //VECTOR2_HPP

#pragma once

#include <cmath>

struct Vector2
{
    float x{0.0f};
    float y{0.0f};

    constexpr Vector2() = default;
    constexpr Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2 &other) const
    {
        return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2 &other) const
    {
        return {x - other.x, y - other.y};
    }

    Vector2 operator*(float scalar) const
    {
        return {x * scalar, y * scalar};
    }

    float length_squared() const
    {
        return x * x + y * y;
    }

    float length() const
    {
        return std::sqrt(length_squared());
    }

    Vector2 normalized() const
    {
        float len = length();
        if (len > 0.0001f)
        {
            return {x / len, y / len};
        }
        return {0.0f, 0.0f};
    }
    
    Vector2 clamp(const Vector2 &min, const Vector2 &max) const
    {
        Vector2 result;
        result.x = x < min.x ? min.x : (x > max.x ? max.x : x);
        result.y = y < min.y ? min.y : (y > max.y ? max.y : y);
        return result;
    }
};
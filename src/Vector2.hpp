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
};
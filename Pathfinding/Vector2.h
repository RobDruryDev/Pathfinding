#pragma once

#include <cmath>
#include <limits>

template<typename T>
class Vector2
{
public: 
    T x, y;
    
    Vector2<T>() {};
    Vector2(T x, T y) : x(x), y(y) {};
    template<typename inT>
    Vector2(const Vector2<inT>& other)
    {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
    }

    Vector2(const Vector2<T>& other)
    {
        x = other.x;
        y = other.y;
    }


    friend Vector2<T> operator+(const Vector2<T>& lhs, T rhs) { return Vector2<T>(); }
    friend Vector2<T> operator+(T rhs, const Vector2<T>& lhs) { return Vector2<T>(); }

    friend Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y); }
    friend Vector2<T> operator+(const Vector2<T>&& lhs, const Vector2<T>&& rhs) { return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs); }
    friend Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs.x - rhs, lhs.y - rhs); }
    friend Vector2<T> operator-(const Vector2<T>&& lhs, const Vector2<T>&& rhs) { return Vector2<T>(lhs.x - rhs, lhs.y - rhs); }
    friend Vector2<T> operator*(const Vector2<T>& lhs, const T rhs) { return Vector2<T>(lhs.x * rhs, lhs.y * rhs); }
    friend Vector2<T> operator*(const Vector2<T>&& lhs, const T rhs) { return Vector2<T>(lhs.x * rhs, lhs.y * rhs); }
    friend Vector2<T> operator/(const Vector2<T>& lhs, const T rhs) { return Vector2<T>(lhs.x / rhs, lhs.y / rhs); }
    friend Vector2<T> operator/(const Vector2<T>&& lhs, const T rhs) { return Vector2<T>(lhs.x / rhs, lhs.y / rhs); }
};

template<typename T>
inline void operator+=(const Vector2<T>& lhs, const Vector2<T>& rhs) { lhs.x += rhs; lhs.y += rhs; }

template<typename T>
inline void operator-=(const Vector2<T>& lhs, const Vector2<T>& rhs) { lhs.x -= rhs; lhs.y -= rhs; }

template<typename T>
inline void operator*=(const Vector2<T>& lhs, const T rhs) { lhs.x *= rhs; lhs.y *= rhs; }

template<typename T>
inline void operator/=(const Vector2<T>& lhs, const T rhs) { lhs.x /= rhs; lhs.y /= rhs; }

template<typename T>
inline bool operator== (const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return lhs.x - rhs.x <= std::numeric_limits<T>::epsilon() &&
           lhs.y - rhs.y <= std::numeric_limits<T>::epsilon();
}

template<typename T>
inline bool operator== (const Vector2<T>& lhs, const Vector2<T>&& rhs)
{
    return lhs.x - rhs.x <= std::numeric_limits<T>::epsilon() &&
           lhs.y - rhs.y <= std::numeric_limits<T>::epsilon();
}

template<typename T>
inline bool operator!= (const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return !(lhs == rhs);
}

template<typename T>
inline bool operator!= (const Vector2<T>& lhs, const Vector2<T>&& rhs)
{
    return !(lhs == rhs);
}

template<typename T>
inline T Dot(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
};

template<typename T>
inline T DistanceSq(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return Dot(lhs, rhs);
};

template<typename T>
inline T Distance(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return static_cast<T>(sqrt(DistanceSq(lhs, rhs)));
};

template<typename T>
inline T LengthSq(const Vector2<T>& vec)
{
    return DistanceSq(vec, vec);
};

template<typename T>
inline T Length(const Vector2<T>& vec)
{
    return Distance(vec, vec);
};

template<typename T>
inline Vector2<T> Normalized(const Vector2<T>& vec)
{
    return vec / Length(vec);
};

template<typename T>
inline void Normalize(const Vector2<T>& vec)
{
    vec /= Length(vec);
};

typedef Vector2<int> Vector2I;
typedef Vector2<float> Vector2F;
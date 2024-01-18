#pragma once

#include <limits>

template<class T>
class Vector2
{
public: 
    static const Vector2<T> Zero() { static const Vector2<T> v(0, 0); return v; }
    static const Vector2<T> One() { static const Vector2<T> v(1, 1); return v; }
    static const Vector2<T> Right() { static const Vector2<T> v(1, 0); return v; }
    static const Vector2<T> Up() { static const Vector2<T> v(0, 1); return v; }

    T x, y;
    
    Vector2<T>() {};
    Vector2(T x, T y) : x(x), y(y) {};

    template<class inT>
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
};

template<class T>
inline Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y); }

template<class T>
inline Vector2<T> operator+(const Vector2<T>&& lhs, const Vector2<T>&& rhs) { return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y); }

template<class T>
inline Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y); }

template<class T>
inline Vector2<T> operator-(const Vector2<T>&& lhs, const Vector2<T>&& rhs) { return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y); }

template<class T>
inline Vector2<T> operator*(const Vector2<T>& lhs, const T rhs) { return Vector2<T>(lhs.x * rhs, lhs.y * rhs); }

template<class T>
inline Vector2<T> operator*(const Vector2<T>&& lhs, const T rhs) { return Vector2<T>(lhs.x * rhs, lhs.y * rhs); }

template<class T>
inline Vector2<T> operator/(const Vector2<T>& lhs, const T rhs) { return Vector2<T>(lhs.x / rhs, lhs.y / rhs); }

template<class T>
inline Vector2<T> operator/(const Vector2<T>&& lhs, const T rhs) { return Vector2<T>(lhs.x / rhs, lhs.y / rhs); }

template<class T>
inline Vector2<T>& operator+=(Vector2<T>& lhs, const Vector2<T>& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }

template<class T>
inline Vector2<T>& operator-=(Vector2<T>& lhs, const Vector2<T>& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }

template<class T>
inline Vector2<T>& operator*=(Vector2<T>& lhs, const T rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }

template<class T>
inline Vector2<T>& operator/=(Vector2<T>& lhs, const T rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }

template<class T>
inline bool operator== (const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return abs(lhs.x - rhs.x) <= std::numeric_limits<T>::epsilon() &&
           abs(lhs.y - rhs.y) <= std::numeric_limits<T>::epsilon();
}

template<class T>
inline bool operator== (const Vector2<T>& lhs, const Vector2<T>&& rhs)
{
    return abs(lhs.x - rhs.x) <= std::numeric_limits<T>::epsilon() &&
           abs(lhs.y - rhs.y) <= std::numeric_limits<T>::epsilon();
}

template<class T>
inline bool operator!= (const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return !(lhs == rhs);
}

template<class T>
inline bool operator!= (const Vector2<T>& lhs, const Vector2<T>&& rhs)
{
    return !(lhs == rhs);
}

template<class T>
inline T Dot(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
};

template<class T>
inline T DistanceSq(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    Vector2<T> toRhs = rhs - lhs;
    return Dot(toRhs, toRhs);
};

template<class T>
inline T Distance(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return static_cast<T>(sqrt(DistanceSq(lhs, rhs)));
};

template<class T>
inline T LengthSq(const Vector2<T>& vec)
{
    return Dot(vec, vec);
};

template<class T>
inline T Length(const Vector2<T>& vec)
{
    return static_cast<T>(sqrt(LengthSq(vec)));
};

template<class T>
inline Vector2<T> Normalized(const Vector2<T>& vec)
{
    return vec / Length(vec);
};

template<class T>
inline void Normalize(const Vector2<T>& vec)
{
    vec /= Length(vec);
};

typedef Vector2<int> Vector2I;
typedef Vector2<float> Vector2F;
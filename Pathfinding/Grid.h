#pragma once

#include <vector>
#include <assert.h>

#include "Vector2.h"

template<class T>
class Grid
{
public:
    Grid(int width, int height) : _width(width), _height(height) { _data.resize(width * height); };

    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
    int GetSize() const { return _width * _height; }

    template<class vecT>
    bool IsValid(const Vector2<vecT>& pos) const { return IsValid(pos.x, pos.y); }

    template<class vecT>
    bool IsValid(const Vector2<vecT>&& pos) const { return IsValid(pos.x, pos.y); }

    bool IsValid(int x, int y) const { return x >= 0 && y >= 0 && x < _width && y < _height; }

    template<class vecT>
    const T& GetData(const Vector2<vecT>& pos) { return GetData(pos.x, pos.y); }

    template<class vecT>
    T& GetData(const Vector2<vecT>&& pos) { return GetData(pos.x, pos.y); }

    T& GetData(int x, int y) { assert(IsValid(x, y)); return _data[x + y * _width]; }

    template<class vecT>
    void SetData(const Vector2<vecT>& pos, T& data) { SetData(pos.x, pos.y, data); }

    template<class vecT>
    void SetData(const Vector2<vecT>&& pos, T&& data) { SetData(pos.x, pos.y, data); }

    void SetData(int x, int y, T& data) { assert(IsValid(x, y)); _data[x + y * _width] = data; }

    void SetData(int x, int y, T&& data) { assert(IsValid(x, y)); _data[x + y * _width] = data; }

protected:
    int _width;
    int _height;

    std::vector<T> _data;
};


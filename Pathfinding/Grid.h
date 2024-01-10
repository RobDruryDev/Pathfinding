#pragma once

#include <iostream>
#include <string>
#include <fstream>
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

    virtual bool IsBlocked(const Vector2I& src, const Vector2I& dst) const { return GetData(dst.x, dst.y) == 1; }

    bool IsBlocked(const Vector2I&& src, const Vector2I&& dst) const { return IsBlocked(src, dst); }

    template<class vecT>
    const T& GetData(const Vector2<vecT>& pos) const { return GetData(pos.x, pos.y); }

    template<class vecT>
    const T& GetData(const Vector2<vecT>&& pos) const { return GetData(pos.x, pos.y); }

    const T& GetData(int x, int y) const { assert(IsValid(x, y)); return _data[x + y * _width]; }

    template<class vecT>
    void SetData(const Vector2<vecT>& pos, T& data) { SetData(pos.x, pos.y, data); }

    template<class vecT>
    void SetData(const Vector2<vecT>&& pos, T&& data) { SetData(pos.x, pos.y, data); }

    void SetData(int x, int y, T& data) { assert(IsValid(x, y)); _data[x + y * _width] = data; }

    void SetData(int x, int y, T&& data) { assert(IsValid(x, y)); _data[x + y * _width] = data; }

    friend _STD ofstream& operator<<(_STD ofstream& stream, Grid<T>& g)
    {
        stream.write(reinterpret_cast<char*>(&g._width), sizeof(g._width));
        stream.write(reinterpret_cast<char*>(&g._height), sizeof(g._height));
        stream.write(reinterpret_cast<char*>(g._data.data()), sizeof(T) * g._data.size());

        return stream;
    }

    friend _STD istream& operator>>(_STD istream& stream, Grid<T>& g)
    {
        stream.read(reinterpret_cast<char*>(&g._width), sizeof(g._width));
        stream.read(reinterpret_cast<char*>(&g._height), sizeof(g._height));
        
        g._data = _STD vector<T>(g._width * g._height);
        stream.read(reinterpret_cast<char*>(g._data.data()), sizeof(T) * g._data.size());

        return stream;
    }

protected:
    int _width;
    int _height;

    _STD vector<T> _data;
};


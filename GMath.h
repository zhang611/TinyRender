#ifndef MATH_H_
#define MATH_H_

#include <vector>
#include <iostream>

template<class T>
static T min(T x, T y, T z) { return std::min<T>(std::min<T>(x, y), z); }

template<class T>
static T max(T x, T y, T z) { return std::max<T>(std::max<T>(x, y), z); }




template <class t>
struct Vec2 {
    t x, y;
    Vec2<t>() : x(t()), y(t()) {}
    Vec2<t>(t _x, t _y) : x(_x), y(_y) {}
    Vec2<t>(const Vec2<t> &v) : x(t()), y(t()) { *this = v; }

    Vec2<t> &operator=(const Vec2<t> &v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
        }
        return *this;
    }

    Vec2<t> operator+(const Vec2<t> &V) const { return Vec2<t>(x + V.x, y + V.y); }

    Vec2<t> operator-(const Vec2<t> &V) const { return Vec2<t>(x - V.x, y - V.y); }

    Vec2<t> operator*(float f) const { return Vec2<t>(x * f, y * f); }

    t &operator[](const int i) { if (i <= 0) return x; else return y; }

    template<class>
    friend std::ostream &operator<<(std::ostream &s, Vec2<t> &v);
};

template <class t>
struct Vec3 {
    t x, y, z;
    Vec3<t>() : x(t()), y(t()), z(t()) {}
    Vec3<t>(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
    Vec3<t>(const Vec3<t> &v) : x(t()), y(t()), z(t()) { *this = v; }
    Vec3<t> &operator=(const Vec3<t> &v) {
        if (this != &v) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
        }
        return *this;
    }

    Vec3<t> operator+(const Vec3<t> &v) const { return Vec3<t>(this->x + v.x, this->y + v.y, this->z + v.z); }

    Vec3<t> operator-(const Vec3<t> &v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }

    Vec3<t> operator*(float f) const { return Vec3<t>(x * f, y * f, z * f); }

    t operator*(const Vec3<t> &v) const { return x * v.x + y * v.y + z * v.z; }

    Vec3<t> operator^(const Vec3<t> &v) const {
        return Vec3<t>(this->y * v.z - this->z * v.y,
                       this->z * v.x - this->x * v.z,
                       this->x * v.y - this->y * v.x);
    }


    [[nodiscard]] float norm() const { return std::sqrt(x * x + y * y + z * z); }

    Vec3<t> &normalize(t l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }

    t &operator[](const int i) { if (i <= 0) return x; else if (i == 1) return y; else return z; }

    template<class>
    friend std::ostream &operator<<(std::ostream &s, Vec3<t> &v);
};

template <class t>
struct Vec4 {
    t x, y, z, w;
    Vec4<t>() : x(t()), y(t()), z(t()), w(t()) {}
    Vec4<t>(t _x, t _y, t _z, t _w) : x(_x), y(_y), z(_z), w(_w) {}
    Vec4<t>(const Vec4<t> &v) : x(t()), y(t()), z(t()), w(t()) { *this = v; }
    Vec4<t> &operator=(const Vec4<t> &v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }
        return *this;
    }

    Vec4<t> operator+(const Vec4<t> &v) const { return Vec4<t>(x + v.x, y + v.y, z + v.z, w + v.w); }

    Vec4<t> operator-(const Vec4<t> &v) const { return Vec4<t>(x - v.x, y - v.y, z - v.z, w - v.w); }

    Vec4<t> operator*(float f) const { return Vec3<t>(x * f, y * f, z * f, w * f); }

    t operator*(const Vec4<t> &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

    [[nodiscard]] float norm() const { return std::sqrt(x * x + y * y + z * z + w * w); }

    Vec4<t> &normalize(t l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }

    t &operator[](const int i) {
        if (i <= 0) return x;
        else if (i == 1) return y;
        else if (i == 2) return z;
        else return w;
    }

    template<class>
    friend std::ostream &operator<<(std::ostream &s, Vec4<t> &v);
};


template <class t>
std::ostream &operator<<(std::ostream &s, Vec2<t> &v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, Vec3<t> &v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, Vec4<t> &v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
    return s;
}


template <typename T>
Vec3<T> cross(Vec3<T> a, Vec3<T> b) {
    return Vec3<T>(a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x);
}

template <typename T>
Vec3<T> dot(Vec3<T> a, Vec3<T> b) { return Vec3<T>(a.x * b.x, a.y * b.y, a.z * b.z); }

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec4<float> Vec4f;


class Matrix {
private:
    std::vector<std::vector<float>> m;
    int rows, cols;

public:
    explicit Matrix(int r = 4, int c = 4);

    explicit Matrix(std::vector<float> nums);

    explicit Matrix(Vec3f v);

    [[nodiscard]] inline int nRows() const { return this->rows; };
    [[nodiscard]] inline int nCols() const { return this->cols; };

    static Matrix identity(int dimensions);

    std::vector<float> &operator[](int i);

    Matrix operator*(float b);

    Matrix operator*(const Matrix &a);

    Matrix operator+(const Matrix &a);

    Matrix transpose();

    Matrix adjugate();

    float algebraicComplement(int x, int y);

    Matrix inverse();

    friend std::ostream &operator<<(std::ostream &s, Matrix &m);

    friend Matrix operator*(float b, Matrix &m);

    static float det(Matrix& m);
};


#endif //MATH_H_


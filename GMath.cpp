#include "GMath.h"

#include <cassert>
#include <iostream>
#include <vector>


Matrix::Matrix(int _row, int _col)
    : m(std::vector<std::vector<float> >(_row, std::vector<float>(_col, 0.f))),
      rows(_row),
      cols(_col) {}


Matrix::Matrix(std::vector<float> nums) {
    assert(nums.size() == 16 || nums.size() == 9);

    rows = 3;
    if (nums.size() == 16) rows = 4;
    cols = rows;

    m = std::vector<std::vector<float>>(rows, std::vector<float>(cols, 0.f));

    for (int i = 0; i < nums.size(); i++) {
        m[i / rows][i % rows] = nums[i];
    }
}


Matrix::Matrix(Vec3f v) {
    rows = 4;
    cols = 1;
    m = std::vector<std::vector<float>> (rows, std::vector<float>(1, 1.f));
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
}

Matrix vertex2matrix(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}


float Matrix::det(Matrix& m) {
    assert(m.nRows() == m.nCols());
    if (m.nRows() == 1) { return m[0][0]; }
    if (m.nRows() == 2) { return m[0][0] * m[1][1] - m[0][1] * m[1][0]; }

    float result = 0;
    for (int i = 0; i < m.nRows(); i++) {
        Matrix subMatrix(m.nRows() - 1, m.nCols() - 1);
        for (int j = 1; j < m.nRows(); j++) {
            for (int k = 0; k < m.nCols(); k++) {
                if (k < i) {
                    subMatrix[j - 1][k] = m[j][k];
                } else if (k > i) {
                    subMatrix[j - 1][k - 1] = m[j][k];
                }
            }
        }
        result += m[0][i] * det(subMatrix) * ((i % 2 == 0) ? 1 : -1);
    }
    return result;
}


Matrix Matrix::identity(int dim) {
    Matrix I(dim, dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            I[i][j] = (i == j ? 1.f : 0.f);
        }
    }
    return I;
}


std::vector<float>& Matrix::operator[](const int i) {
    assert(i >= 0 && i < rows);
    return m[i];
}

Matrix Matrix::operator*(const Matrix &a) {
    assert(cols == a.nRows());
    Matrix result(rows, a.nCols());
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result.m[i][j] = 0.f;
            for (int k = 0; k < cols; k++) {
                result.m[i][j] += m[i][k] * a.m[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix &a) {
    assert(cols == a.nCols() && rows == a.nRows());
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.m[i][j] = m[i][j] + a.m[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const float b) {
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.m[i][j] = m[i][j] * b;
        }
    }
    return result;
}

Matrix Matrix::transpose() {
    Matrix res(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            res[j][i] = m[i][j];
    return res;
}


/// 求x，y位置的代数余子式
/// \param x row
/// \param y col
/// \return
float Matrix::algebraicComplement(int x, int y) {
    std::vector<float> temp;
    for (int i = 0; i < rows; i++) {
        if (i == x) continue;
        for (int j = 0; j < cols; j++) {
            if (j == y) continue;
            temp.push_back(m[i][j]);
        }
    }
    Matrix sub(temp);
    return (float)((x + y) % 2 ? -1 : 1) * det(sub);
}


/// 伴随矩阵是代数余子式矩阵的转置
/// \return
Matrix Matrix::adjugate() {
    assert(rows == cols);
    Matrix res(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res[i][j] = algebraicComplement(i, j);
        }
    }
    return res.transpose();
}


Matrix Matrix::inverse() {
    assert(rows == cols);
    float d = det(*this);
    if (std::abs(d) < 1e-6) throw std::runtime_error("error inverse");

    Matrix adj = this->adjugate();  // 伴随矩阵
    return adj * (1.f / d);
}

std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i = 0; i < m.nRows(); i++) {
        for (int j = 0; j < m.nCols(); j++) {
            s << m[i][j];
            if (j < m.nCols() - 1) s << "\t";
        }
        s << "\n";
    }
    return s;
}


Matrix operator*(float b, Matrix& m) {
    Matrix result(m.nRows(), m.nCols());
    for (int i = 0; i < result.nRows(); i++) {
        for (int j = 0; j < result.nCols(); j++) {
            result.m[i][j] = m[i][j] * b;
        }
    }
    return result;
}





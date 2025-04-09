#include "mvp.h"

constexpr float MY_PI = 3.1415926f;
const int depth = 255;


Matrix modelMatrix(float rotation_angle, Vec3f axis) {
    axis.normalize();
    rotation_angle = rotation_angle * MY_PI / 180.f;

    Matrix nnT = Matrix::identity(4), crossN(4, 4);
    float cosAlpha = cos(rotation_angle);
    float sinAlpha = sin(rotation_angle);
    float x = axis.x, y = axis.y, z = axis.z;

    // nnT <<  x*x, x*y, x*z,0,
    //         y*x, y*y, y*z,0,
    //         z*x, z*y, z*z,0,
    //         0,0,0,1;
    nnT[0][0] = x * x, nnT[0][1] = x * y, nnT[0][2] = x * z;
    nnT[1][0] = y * x, nnT[1][1] = y * y, nnT[1][2] = y * z;
    nnT[2][0] = z * x, nnT[2][1] = z * y, nnT[2][2] = z * z;

    // crossn<<    0,-z, y,0,
    //             z, 0,-x,0,
    //             -y,x, 0,0,
    //             0,0,0,1;
    crossN[0][1] = -z, crossN[0][2] = y,
    crossN[1][0] = z, crossN[1][2] = -x,
    crossN[2][0] = -y, crossN[2][1] = x,
    crossN[3][3] = 1;

    Matrix rotate = Matrix::identity(4) * cosAlpha + (1 - cosAlpha) * nnT + sinAlpha * crossN;
    rotate[3][3] = 1;
    return rotate;
}



/// 相机运动 == 物体做逆运动
/// 让相机坐标和世界坐标重合，其他点做相同变换
/// \param eyePos
/// \param targetPos
/// \param up
/// \return
Matrix lookAt(Vec3f eyePos, Vec3f &targetPos, Vec3f &up) {
    Vec3f z = (eyePos - targetPos).normalize();
    Vec3f x = cross(up, z).normalize();
    Vec3f y = cross(z, x).normalize();
    Matrix invM = Matrix::identity(4);
    Matrix translate = Matrix::identity(4);
    for (int i = 0; i < 3; i++) {
        invM[0][i] = x[i];
        invM[1][i] = y[i];
        invM[2][i] = z[i];
        translate[i][3] = -eyePos[i];
    }
    return invM * translate;   // 先平移再缩放
}


/// 坐标映射入[-1,1]*[-1,1]*[-1,1]的NDC空间
/// \param eye_fov 垂直可视角度
/// \param aspect_ratio 宽高比
/// \param zNear 近平面
/// \param zFar 远平面
/// \return
Matrix projection(float eye_fov, float aspect_ratio,
                  float zNear, float zFar) {
    Matrix res;
    Matrix orth, orthTranslate, orthScale, pers2orth;
    float n = -zNear, f = -zFar;
    float t = tan(eye_fov / 2) * (-n), b = -t;
    float r = aspect_ratio * t, l = -r;

    std::vector<float> p2o = {
            n, 0, 0, 0,
            0, n, 0, 0,
            0, 0, n + f, -n * f,
            0, 0, 1, 0
    };
    pers2orth = Matrix(p2o);

    std::vector<float> scale = {
            2.f/(r - l), 0, 0, 0,
            0, 2.f/(t - b), 0, 0,
            0, 0, 2.f/(n - f), 0,
            0, 0, 0, 1
    };
    orthScale = Matrix(scale);

    std::vector<float> trans = {
            1, 0, 0, -(r + l) / 2.f,
            0, 1, 0, -(t + b) / 2.f,
            0, 0, 1, -(n + f) / 2.f,
            0, 0, 0, 1
    };
    orthTranslate = Matrix(trans);

    orth = orthScale * orthTranslate;  // 先平移，再缩放
    res = orth * pers2orth;  // 先透视，再正交
    return res;
}



Matrix &projdivision(Matrix &clip) {
    Matrix &ndc = clip;
    ndc[0][0] = clip[0][0] / clip[3][0];
    ndc[1][0] = clip[1][0] / clip[3][0];
    ndc[2][0] = clip[2][0] / clip[3][0];
    ndc[3][0] = 1.0f;
    return ndc;
}


Matrix viewport(int x, int y, int w, int h) {
    // [-1,1]*[-1,1]*[-1,1]平移到[0,2]*[0,2]*[0,2]
    std::vector<float> nums1 = {
            1, 0, 0, 1,
            0, 1, 0, 1,
            0, 0, 1, 1,
            0, 0, 0, 1
    };

    // [0,2]*[0,2]*[0,2]缩放到[0,1]*[0,1]*[0,1]
    std::vector<float> nums2 = {
            0.5f, 0, 0, 0,
            0, 0.5f, 0, 0,
            0, 0, 0.5f, 0,
            0, 0, 0, 1
    };

//    [0,1]*[0,1]*[0,1]缩放到[0,w]*[0,h]*[0,d]
    std::vector<float> nums3 = {
            static_cast<float>(w), 0, 0, 0,
            0, static_cast<float>(h), 0, 0,
            0, 0, depth, 0,
            0, 0, 0, 1
    };

//    [0,w]*[0,h]*[0,d]移动到[x,x+w]*[y,y+h]*[0,d]
    std::vector<float> nums4 = {
            1, 0, 0, static_cast<float>(x),
            0, 1, 0, static_cast<float>(y),
            0, 0, 1, 0,
            0, 0, 0, 1
    };

    Matrix res = Matrix(nums4) * Matrix(nums3) * Matrix(nums2) * Matrix(nums1);
//    std::cout << res << std::endl;
    return res;
}

IShader::~IShader() = default;

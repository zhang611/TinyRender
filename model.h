#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "GMath.h"
#include "tgaimage.h"
#include <opencv2/opencv.hpp>

struct ids {
    int vIdx, uvIdx, normIdx;

    ids() : vIdx(), uvIdx(), normIdx() {}
    ids(int v, int uv, int other) : vIdx(v), uvIdx(uv), normIdx(other) {}
};

class Model {
private:
    std::vector<Vec3f> vs_;
    std::vector<Vec2f> uvs_;
    std::vector<Vec3f> norms_;
    std::vector<std::vector<ids> > faces_;

    TGAImage diffuseMap;
    Vec2i diffuseMapSize;

public:
    Model(const char *filename, const char *diffuseFilename);

    ~Model() = default;

    int nVert() { return (int) vs_.size(); }

    int nUvs() { return (int) uvs_.size(); }

    int nFaces() { return (int) faces_.size(); }

    int nNormals() { return (int) norms_.size(); }


    Vec3f vert(int idx) { return vs_[idx]; };

    Vec2f uv(int idx) { return uvs_[idx]; };

    Vec3f normal(int idx) { return norms_[idx]; };

    std::vector<ids> face(int idx) { return faces_[idx]; };


    Vec3f vert(int iface, int nthVert) ;

    Vec2f uv(int iface, int nthVert);

    Vec3f normal(int iface, int nthVert);


    TGAColor diffuse(float u, float v) {
        return diffuseMap.get(int(diffuseMapSize.x * u), int(diffuseMapSize.y * v));
    }
};

#endif //__MODEL_H__
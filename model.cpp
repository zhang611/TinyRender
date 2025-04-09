#include "model.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Model::Model(const char *filename, const char *diffuseFilename) : vs_(), uvs_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            vs_.push_back(v);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec3f uv;
            for (int i = 0; i < 3; i++) iss >> uv[i];
            uvs_.emplace_back(uv.x, uv.y);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f nor;
            for (int i = 0; i < 3; i++) iss >> nor[i];
            norms_.push_back(nor);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<ids> f;
            int vIdx, uvIdx, nmIdx;
            iss >> trash;
            while (iss >> vIdx >> trash >> uvIdx >> trash >> nmIdx) {
                vIdx--;  // in wavefront obj all indices start at 1, not zero
                uvIdx--;
                nmIdx--;
                f.emplace_back(vIdx, uvIdx, nmIdx);
            }
            faces_.push_back(f);
        }
    }

    if (diffuseFilename != nullptr) {
        diffuseMap.read_tga_file(diffuseFilename);
        diffuseMap.flip_vertically();
        diffuseMapSize = Vec2i(diffuseMap.get_width(), diffuseMap.get_height());
    }

    std::cerr << "# v# " << vs_.size() << " uv# " << uvs_.size() << " f# " << faces_.size() << std::endl;
}

Vec3f Model::vert(int iface, int nthVert) {
    auto idx = faces_[iface][nthVert].vIdx;
    return vs_[idx];
}

Vec2f Model::uv(int iface, int nthVert) {
    auto idx = faces_[iface][nthVert].uvIdx;
    return uvs_[idx];
}

Vec3f Model::normal(int iface, int nthVert) {
    auto idx = faces_[iface][nthVert].normIdx;
    return norms_[idx];
}

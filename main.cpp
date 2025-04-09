#include <cmath>
#include <opencv2/opencv.hpp>
#include <vector>

#include "draw.h"
#include "model.h"
#include "tgaimage.h"
#include "mvp.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

Model *model = nullptr;
float *zBuffer = nullptr;

Vec3f camera(0, 0, 3);
Vec3f target(0, 0, 0);
Vec3f up(0, 1, 0);

const int width = 800;
const int height = 800;
Vec3f light_dir(0, 0, -1);


void drawModel(TGAImage &image, Matrix &modelM, Matrix &viewM, Matrix &projM, Matrix &viewportM) {
    Matrix worldSpace, viewSpace, clipSpace, viewPortSpace;
    for (int i = 0; i < model->nFaces(); i++) {
        std::vector<ids> face = model->face(i);
        Vec3f pts[3], modelPos[3];
        Vec2f coords[3];
        for (int j = 0; j < 3; j++) {
            modelPos[j] = model->vert(face[j].vIdx);
            coords[j] = model->uv(face[j].uvIdx);

            worldSpace = modelM * Matrix(modelPos[j]);
            viewSpace = viewM * worldSpace;
            clipSpace = projM * viewSpace;
            viewPortSpace = viewportM * projdivision(clipSpace);
            pts[j] = Vec3f(viewPortSpace[0][0], viewPortSpace[1][0], viewPortSpace[2][0]);
        }
        Vec3f n = cross(pts[2] - pts[0], pts[1] - pts[0]);
        n.normalize();
        float intensity = n * light_dir;

        triangle(image, model, zBuffer, pts, coords, std::max(intensity, 0.1f));
    }
}

void renderModel() {
    model = new Model("../assets/obj/african_head.obj",
                      "../assets/obj/african_head_diffuse.tga");

    zBuffer = new float[width * height];
    TGAImage image(width, height, TGAImage::RGB);
    std::string mTitle = "image";
    cv::Mat img(height, width, CV_8UC3);  // 8 bit unsigned, 3 channels
    cv::namedWindow(mTitle, cv::WINDOW_AUTOSIZE);

    float angle = 0.0f;
    float step = 1.;

    Matrix viewportM = viewport(0, 0, width, height);
    float radius = 3.0f;
    float time = 0.0f;

    int key = -1;
    while (key != 27) {
        for (int i = width * height - 1; i >= 0; i--) zBuffer[i] = -std::numeric_limits<float>::infinity();
        image.clear();
        radius = 0.1f * sin(time * 2.0f) + radius;
        float camX = sin(time) * radius;
        float camZ = cos(time) * radius;
//        Matrix modelM = Matrix::identity(4);
        Matrix modelM = modelMatrix(angle, {0,1,0});
//        Matrix viewM = lookAt({camX, 0, camZ}, target, up);
        Matrix viewM = lookAt(camera, target, up);
        Matrix projM = projection(45, 1, 0.1f, 50.0f);

        // shader.setModel(modelM); shader.setLookAt(viewM); shader.setProj(projM); shader.setViewPort(viewportM);
        drawModel(image, modelM, viewM, projM, viewportM);
        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image

        img.data = image.buffer();
        cv::imshow(mTitle, img);
        if (cv::getWindowProperty(mTitle, cv::WND_PROP_AUTOSIZE) < 1) break;
        key = cv::waitKey(10);
        angle += step;
        // time += 0.1f;
        // angle = std::fmodf(angle, 360.0f);  // 浮点数取余
    }

    image.write_tga_file("../image/output.tga");
    cv::imwrite("../image/render_obj.png", img);

    delete model;
    delete[] zBuffer;
}


void drawLine() {
    TGAImage image(width, height, TGAImage::RGB);
    std::string mTitle = "image";
    cv::namedWindow(mTitle, cv::WINDOW_AUTOSIZE);
    cv::Mat img(height, width, CV_8UC3);
    int key = -1;
    while (key != 27) {
        key = cv::waitKey(10);
        image.clear();

        line(image, 0, 0, 500, 500, TGAColor(0, 0, 255, 255));

        img.data = image.buffer();
        cv::imshow(mTitle, img);
    }
    cv::imwrite("../image/line.png", img);
}


void drawTriangle() {
    TGAImage image(width, height, TGAImage::RGB);
    std::string mTitle = "image";
    cv::namedWindow(mTitle, cv::WINDOW_AUTOSIZE);
    cv::Mat img(height, width, CV_8UC3);
    int key = -1;
    while (key != 27) {
        key = cv::waitKey(10);
        image.clear();

        Vec3f oneTriangle[3];
        oneTriangle[0] = {100, 100, 0};
        oneTriangle[1] = {100, 500, 0};
        oneTriangle[2] = {500, 500, 0};

        simpleTriangle(image, oneTriangle);
        img.data = image.buffer();
        cv::imshow(mTitle, img);
    }
    cv::imwrite("../image/oneTriangle.png", img);
}



int main(int argc, char **argv) {
//    renderModel();

//    drawLine();
    drawTriangle();
    return 0;
}
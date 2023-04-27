#pragma once
#include<string>
#include<vector>
#include <iostream>
#include <algorithm> 

class Image {

    int width;
    int height;
    std::vector<unsigned char*>image;
    int dim;

    void LoadImage(unsigned char* src) {


        for (int i = 0; i <= (height * dim) - dim; i += dim) {
            for (int j = 0; j <= (width * dim) - dim; j += dim) {
                unsigned char* color = new unsigned char[3];

                for (int k = 0; k < 3; k++) {
                    color[k] = src[(width * i + j) + k % dim];
                }
                image.push_back(color);
            }
        }

        dim = std::clamp(dim, 0, 3);
    }

public:
    Image(int w, int h, int dim, unsigned char* i) :width(w), height(h), dim(dim) {

        LoadImage(i);
    }

    Image(int w, int h, int dim) :width(w), height(h), dim(dim) {

        for (int i = 0; i <= (height * dim) - dim; i += dim) {
            for (int j = 0; j <= (width * dim) - dim; j += dim) {
                unsigned char* color = new unsigned char[3];

                for (int k = 0; k < 3; k++) {
                    color[k] = 0;
                }
                image.push_back(color);
            }
        }

        dim = std::clamp(dim, 0, 3);
    }

    Image() :width(0), height(0) {
    }

    ~Image() {
        image.clear();
    }

    Image(int w, int h, int dim, std::vector<unsigned char*>img) :width(w), height(h), dim(dim), image(img) {

    }


    Image(const Image& im) :width(im.getWidth()), height(im.getHeight()), dim(im.getDim()) {
        this->image = std::vector<unsigned char*>();

        std::vector<unsigned char*> tmp = im.getImage();

        for (int i = 0; i < im.getWidth(); i++) {
            for (int j = 0; j < im.getHeight(); j++) {
                unsigned char* color = new unsigned char[3];

                for (int k = 0; k < 3; k++) {
                    *(color + k) = *(tmp[i * height + j] + k % dim);
                }

                image.push_back(color);
            }
        }

    }


    unsigned char* getFlatArray() const {
        unsigned char* temp = new unsigned char[width * height * dim];
        int idx = 0;
        for (int i = 0; i < image.size(); i++) {

            for (int k = 0; k < dim; k++) {

                temp[idx++] = image[i][k % dim];
            }
        }
        return temp;
    }


    int getWidth() const { return this->width; };

    int getHeight() const { return this->height; };

    int getDim() const { return this->dim; };

    std::vector<unsigned char*> getImage() const { return this->image; };




};
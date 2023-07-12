#pragma once
#include<string>
#include<vector>
#include <iostream>
#include <algorithm> 

#include "stb_image.h"
#include "stb_image_write.h"


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

    Image() :width(0), height(0), dim(0) {
    };

    Image(int w, int h, int dim, unsigned char* i) :width(w), height(h), dim(dim) {

        LoadImage(i);
    }

    Image(const char* path) {
       LoadImage( stbi_load(path, &width, &height, &dim, 0));
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

    ~Image() {
        image.clear();
    }

    Image operator=(Image im) {
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

        return (*this);
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

    void WriteImage(const char* path) {
        stbi_write_png(path, width, height, dim,&getFlatArray()[0], width * dim);
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


    int getWidth() const {
        
        return this->width; 
    
    };

    int getHeight() const { return this->height; };

    int getDim() const { return this->dim; };

    std::vector<unsigned char*> getImage() const { return this->image; };

    unsigned char* getColor(int i, int j) {
        if (height <= 0 || width <= 0 || j * width + i >= height*width ) {
            return new unsigned char[3] {0, 0, 0};
        }

        return image[j * width + i];
    }


    Image blur(Image im) {

        std::cout << "start basic AA done" << std::endl;
        std::vector<unsigned char*> arr = im.getImage();
        Image copy(im);
        int width = im.getWidth();
        int height = im.getHeight();
        int radius = 5;
        std::vector<unsigned char*> arrC = copy.getImage();
        float* sum = new float[3] {0, 0, 0};
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {

                int a = std::clamp(i - radius, 0, width);
                int b = std::clamp(i + radius + 1, 0, width);

                int c = std::clamp(j - radius, 0, height);
                int d = std::clamp(j + radius + 1, 0, height);

                int k = 0;
                sum = new float[3] { 0, 0, 0 };

                for (int x = a; x < b; x++) {
                    for (int y = c; y < d; y++) {


                        for (size_t n = 0; n < 3; n++)
                        {
                            sum[n] += arr[width * y + x][n];
                        }

                        k++;
                    }
                }

                arrC[(width * j + i)][0] = (float)sum[0] / k;
                arrC[(width * j + i)][1] = (float)sum[1] / k;
                arrC[(width * j + i)][2] = (float)sum[2] / k;

            }
        }

        std::cout << "Basic AA done" << std::endl;

        return copy;
    }


};

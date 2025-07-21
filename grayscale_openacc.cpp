#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <cstdio>
#include <vector>
#include <chrono>

int main() {
    int width, height, channels;

    std::cout << "======= RGB TO GRAYSCALE WITH OPENACC =======" << std::endl;

    // Delete old grayscale image
    if (std::remove("output/picture_grayscale.png") == 0) {
        std::cout << "Old \"picture_grayscale.png\" deleted\n";
    } else {
        std::cout << "No existing \"picture_grayscale.png\" to delete\n";
    }

    // Load raw image
    unsigned char* input_raw = stbi_load("output/picture_color.jpg", &width, &height, &channels, 3);
    if (!input_raw) {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
        return 1;
    }

    // Move to std::vector for OpenACC compatibility
    std::vector<unsigned char> input(input_raw, input_raw + width * height * 3);
    std::vector<unsigned char> gray(width * height);
    stbi_image_free(input_raw);  // Free raw image

    unsigned char* input_ptr = input.data();
    unsigned char* gray_ptr = gray.data();



    // Defines the data region where OpenACC can manage the memory explicitly
    #pragma acc data create(input_ptr[0:width*height*3], gray_ptr[0:width*height])
    {
        // Copies the input data (RGB picture) from CPU to GPU
        #pragma acc update device(input_ptr[0:width*height*3])

        // Start timer before loop
        auto start = std::chrono::high_resolution_clock::now();

        // RGB to grayscale conversion but parallel process it using GPU
        // collapse(2): basically combines two nested loops into a single loop
        // vector_length(128): basically states how many threads per vector lane
        #pragma acc parallel loop collapse(2) vector_length(128)
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = y * width + x;
                int r = input_ptr[3 * idx + 0];
                int g = input_ptr[3 * idx + 1];
                int b = input_ptr[3 * idx + 2];
                gray_ptr[idx] = static_cast<unsigned char>(0.299f * r + 0.587f * g + 0.114f * b);
            }
        }

        // Stop timer after loop
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double, std::milli>(end - start);
        std::cout << "Time duration: " << duration.count() << " ms (WITH OPENACC GPU)\n\n";

        // Copies the output data (grayscale picture) from GPU to CPU
        #pragma acc update self(gray_ptr[0:width*height])
    }

    // Save grayscale image as png
    std::cout << "Saving image...\n";
    stbi_write_png("output/picture_grayscale.png", width, height, 1, gray.data(), width);
    std::cout << "Grayscale image saved as \"picture_grayscale.png\" \n";

    return 0;
}

// compiled with:
// nvc++ -acc -gpu=cc86 -fast --diag_suppress=set_but_not_used grayscale_openacc.cpp -o grayscale_openacc
// OR
// nvc++ -acc -gpu=cc86 -fast grayscale_openacc.cpp -o grayscale_openacc
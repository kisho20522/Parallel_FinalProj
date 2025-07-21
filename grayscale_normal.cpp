// Enables us to load image into memory as raw pixel arrays
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Enables us to save image from raw pixel data
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream> // input output
#include <cstdio> // file operations
#include <vector> // use as dynamic array for pixel data
#include <chrono> // measure time

int main() {
    // height & width - image dimensions   |   channels - channel count (or 3 for RGB)
    int width, height, channels;

    std::cout << "======= RGB TO GRAYSCALE NORMAL =======" << std::endl;

    // Deletes the old existing output, if it exists
    if (std::remove("output/picture_grayscale.png") == 0) {
        std::cout << "Old \"picture_grayscale.png\" deleted\n";
    } else {
        std::cout << "No existing \"picture_grayscale.png\" to delete\n";
    }
    
    // Load input image
    unsigned char* input = stbi_load("output/picture_color.jpg", &width, &height, &channels, 3);
    if (!input) {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
        return 1;
    }

    // initialize vector output for grayscale
    std::vector<unsigned char> gray(width * height);

    // Start timer before loop
    auto start = std::chrono::high_resolution_clock::now();

    // RGB to Grayscale conversion
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = y * width + x;
            int r = input[3 * idx + 0];
            int g = input[3 * idx + 1];
            int b = input[3 * idx + 2];
            gray[idx] = static_cast<unsigned char>(0.299f * r + 0.587f * g + 0.114f * b);
        }
    }

    // Stop timer after loop
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

    // Displays time duration where the program starts the grayscale process
    std::cout << "Time duration: " << duration.count() << " ms (NORMAL) \n\n";

    // Save grayscale image as PNG
    std::cout << "Saving image...\n";
    stbi_write_png("output/picture_grayscale.png", width, height, 1, gray.data(), width);
    stbi_image_free(input); // frees the memory allocated by stbi_load
    std::cout << "Grayscale image saved as \"picture_grayscale.png\" \n";

    return 0;
}

// compiled with:
// g++ grayscale_normal.cpp -o grayscale_normal
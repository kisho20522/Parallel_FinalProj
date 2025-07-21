# CS0051 - Parallel Computing Final Project

This project demonstrates the performance difference between sequential and parallel image processing by converting RGB images to grayscale using two approaches:

- 🖥️ **Sequential C++ Implementation**
- 🚀 **Parallel C++ Implementation with OpenACC (GPU acceleration)**

---

## 📌 Project Overview

The objective is to compare the execution time and efficiency between CPU and GPU-based grayscale image conversion.

### ✅ Sequential Code
Standard C++ image processing using loops.

### ⚡ Parallel Code
Utilizes **OpenACC** directives via **NVIDIA HPC SDK** to accelerate the grayscale conversion on the GPU.

---

## 🛠️ Prerequisites

To run the parallel version, ensure the following:

- NVIDIA GPU with proper drivers installed
- Stable GPU utilization
- **WSL** (Windows Subsystem for Linux) or any compatible Linux environment
- NVIDIA HPC SDK installed and configured
- `stb_image.h` and `stb_image_write.h` from [nothings/stb](https://github.com/nothings/stb)

---

## 📦 Dependencies

This project uses custom single-header libraries for image handling:

- `stb_image.h` - Load images into pixel data
- `stb_image_write.h` - Write grayscale images to output file

---

## 🚀 How to Compile and Run

### Sequential Version
```bash
g++ grayscale_normal.cpp -o grayscale_normal
./grayscale_normal
```

### Sequential Version
```bash
nvc++ -acc -gpu=cc86 -fast --diag_suppress=set_but_not_used grayscale_openacc.cpp -o grayscale_openacc
./grayscale_openacc
```

# QR Code Generator

A simple and clean desktop application built with **Qt 6** that generates QR codes from any text or URL.
The project includes features for generating, previewing, copying, and saving QR codes in multiple formats.

---

## Features

* Generate QR codes from any text or link
* Copy QR code to clipboard
* Save QR images as **PNG** or **JPEG**
* Lightweight and fast

---

## Technologies Used

* **C++**
* **Qt 6 (Widgets)**
* **CMake**
* **qrcodegen** by https://github.com/nayuki

---

## Build Steps

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The executable will appear in the `build` directory.

---

## Deployment (Windows)

To deploy a standalone `.exe`, use:

```bash
windeployqt path/to/your.exe
```

This will copy all required Qt libraries into the executable folder.

# Welcome to Color++ library

The cross-platform Color++ library allows you to perform color conversions between HSV, HSL, RGB, XYZ and Lab models.

Commonly used RGB color spaces are supported:
- Adobe RGB (1998)
- Apple RGB
- Best RGB
- Beta RGB
- Bruce RGB
- CIE RGB
- Color Match RGB
- Don RGB 4
- ECI RGB v2
- Ekta Space PS5
- NTSC RGB
- PAL/SECAM RGB
- ProPhoto RGB
- SMPTE-C RGB
- sRGB
- Wide Gamut RGB

In addition, you can set the gamma, choose the methods of chromatic adaptation and the white point.

Part of the code is ported to C++ from brucelindblum.com

Requirements:
C++11
STL

Unit tests are implemented through gtest. Installed with the git command:
git clone https://github.com/google/googletest.git -b release-1.11.0

/*!
\file hsl.cpp
\brief This file contains the source code of HSL conversion as a part of
	Color++ library
\authors Konstantin A. Pankov, explorus@mail.ru
\copyright MIT License
\version 1.0
\date 05/03/2022 (DD/MM/YYYY)
\warning Unstable code. Under development.

The MIT License

Copyright(c) 2020 Konstantin Pankov, explorus@mail.ru

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <algorithm>
//#include <cmath>

namespace colorpp 
{

/*!
    \brief Conversion from RGB to HSL color model
    \param[in] r - red channel in 0..1.0 range
	\param[in] g - green channel in 0..1.0 range
 	\param[in] b - blue channel in 0..1.0 range
 	\param[out] h - hue channel in 0..1.0 range
  	\param[out] s - saturation channel in 0..1.0 range
  	\param[out] l - lightness channel in 0..1.0 range
*/
void rgb_to_hsl(double r, double g, double b,
	double& h, double& s, double& l)
{
    auto max_channel = std::max(r, std::max(g, b));
    auto min_channel = std::min(r, std::min(g, b));
    auto chroma = max_channel - min_channel;
	auto lightness = (max_channel + min_channel) / 2.;
    
    // hue
    if (chroma != 0)
    {
        if (r == max_channel)
            h = (g - b) / chroma;
        else if (g == max_channel)
            h = 2. + (b - r) / chroma;
        else
            h = 4. + (r - g) / chroma;
        h /= 6.;
        if (h < 0) h += 1.;
    }
    else
        h = 0;

    // saturation
    if (lightness > 0. && lightness < 1.)
        s = chroma / (1. - std::abs(2. * lightness - 1.));
    else
        s = 0;

    // lightness
    l = lightness;
}

namespace
{
    double mod2(double v)
    {
        while (v > 2.)
            v -= 2.;
        return v;
    }
}

/*!
    \brief Conversion from HSL to RGB color model
	\param[in] h - hue channel in 0..1.0 range
  	\param[in] s - saturation channel in 0..1.0 range
  	\param[in] l - lightness channel in 0..1.0 range
    \param[out] r - red channel in 0..1.0 range
	\param[out] g - green channel in 0..1.0 range
 	\param[out] b - blue channel in 0..1.0 range
*/
void hsl_to_rgb(double h, double s, double l,
	double& r, double& g, double& b)
{
    h *= 6.;
    auto c = (1. - std::abs(2. * l - 1.)) * s;
	//auto x = c * (1. - std::abs(std::fmod(h, 2.) - 1.));
    auto x = c * (1. - std::abs(mod2(h) - 1.));
    auto m = l - c / 2.;

    if (h >= 6.)
        h -= 6.;
    if (h < 1.)
    {
        r = c + m;
        g = x + m;
        b = m;
    }
    else if (h < 2.)
    {
        r = x + m;
        g = c + m;
        b = m;
    }
    else if (h < 3.)
    {
        r = m;
        g = c + m;
        b = x + m;
    }
    else if (h < 4.)
    {
        r = m;
        g = x + m;
        b = c + m;
    }
    else if (h < 5.)
    {
        r = x + m;
        g = m;
        b = c + m;
    }
    else if (h < 6.)
    {
        r = c + m;
        g = m;
        b = x + m;
    }
    else
    {
        r = 0;
        g = 0;
        b = 0;
    }

}

}


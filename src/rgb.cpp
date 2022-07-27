/*!
\file rgb.cpp
\brief This file contains the source code of RGB conversion	as a part of
	Color++ library
\authors Konstantin A. Pankov, explorus@mail.ru
\copyright MIT License
\version 1.0
\date 25/07/2022 (DD/MM/YYYY)
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

#include "rgb.h"
#include <cmath>

namespace colorpp 
{

// brucelindblum.com CIE Color Calculator C++ porting

const Mtx3x3 Adaptations[3][2] = {
		{
			{{0.8951, -0.7502, 0.0389}, {0.2664, 1.7135, -0.0685}, {-0.1614, 0.0367, 1.0296}},
			{{0.9869929, 0.4323053, -0.0085287}, {-0.1470543, 0.5183603, 0.0400428}, {0.1599627, 0.0492912, 0.9684867}}
		},
		{
			{{0.40024, -0.2263, 0}, {0.7076, 1.16532, 0}, {-0.08081, 0.0457, 0.91822}},
			{{1.8599364, 0.3611914, 0}, {-1.1293816, 0.6388125, 0}, {0.2198974, -0.0000064, 1.0890636}}
		},
		{
			{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
			{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
		}
};

static double Compand(double linear, const double gamma)
{
	double companded;
	if (gamma > 0.0)
	{
		companded = (linear >= 0.0) ? pow(linear, 1.0 / gamma) : -pow(-linear, 1.0 / gamma);
	}
	else if (gamma < 0.0)
	{
		/* sRGB */
		double sign = 1.0;
		if (linear < 0.0)
		{
			sign = -1.0;
			linear = -linear;
		}
		companded = (linear <= 0.0031308) ? (linear * 12.92) : (1.055 * pow(linear, 1.0 / 2.4) - 0.055);
		companded *= sign;
	}
	else
	{
		/* L* */
		double sign = 1.0;
		if (linear < 0.0)
		{
			sign = -1.0;
			linear = -linear;
		}
		companded = (linear <= (216.0 / 24389.0)) ? (linear * 24389.0 / 2700.0) : (1.16 * pow(linear, 1.0 / 3.0) - 0.16);
		companded *= sign;
	}
	return companded;
}

static double InvCompand(double companded, const double gamma)
{
	double linear;
	if (gamma > 0.0)
	{
		linear = (companded >= 0.0) ? pow(companded, gamma) : -pow(-companded, gamma);
	}
	else if (gamma < 0.0)
	{
		/* sRGB */
		double sign = 1.0;
		if (companded < 0.0)
		{
			sign = -1.0;
			companded = -companded;
		}
		linear = (companded <= 0.04045) ? (companded / 12.92) : pow((companded + 0.055) / 1.055, 2.4);
		linear *= sign;
	}
	else
	{
		/* L* */
		double sign = 1.0;
		if (companded < 0.0)
		{
			sign = -1.0;
			companded = -companded;
		}
		linear = (companded <= 0.08) ? (2700.0 * companded / 24389.0) : ((((1000000.0 * companded + 480000.0) * companded + 76800.0) * companded + 4096.0) / 1560896.0);
		linear *= sign;
	}
	return linear;
}

static double Determinant3x3(Mtx3x3 m)
{
	double det = m[0][0] * (m[2][2] * m[1][1] - m[2][1] * m[1][2]) -
		m[1][0] * (m[2][2] * m[0][1] - m[2][1] * m[0][2]) +
		m[2][0] * (m[1][2] * m[0][1] - m[1][1] * m[0][2]);

	return det;
}

static void MtxInvert3x3(Mtx3x3 m, Mtx3x3 i)
{
	double scale = 1.0 / Determinant3x3(m);

	i[0][0] = scale * (m[2][2] * m[1][1] - m[2][1] * m[1][2]);
	i[0][1] = -scale * (m[2][2] * m[0][1] - m[2][1] * m[0][2]);
	i[0][2] = scale * (m[1][2] * m[0][1] - m[1][1] * m[0][2]);

	i[1][0] = -scale * (m[2][2] * m[1][0] - m[2][0] * m[1][2]);
	i[1][1] = scale * (m[2][2] * m[0][0] - m[2][0] * m[0][2]);
	i[1][2] = -scale * (m[1][2] * m[0][0] - m[1][0] * m[0][2]);

	i[2][0] = scale * (m[2][1] * m[1][0] - m[2][0] * m[1][1]);
	i[2][1] = -scale * (m[2][1] * m[0][0] - m[2][0] * m[0][1]);
	i[2][2] = scale * (m[1][1] * m[0][0] - m[1][0] * m[0][1]);
}

static void MtxTranspose3x3(Mtx3x3 m)
{
	double v = m[0][1];
	m[0][1] = m[1][0];
	m[1][0] = v;

	v = m[0][2];
	m[0][2] = m[2][0];
	m[2][0] = v;

	v = m[1][2];
	m[1][2] = m[2][1];
	m[2][1] = v;
}

static void GetRefWhite(double* RefWhite, IlluminantEnum illuminant = IlluminantEnum::D50)
{
	RefWhite[1] = 1.0;
	switch (illuminant)
	{
	case IlluminantEnum::A:	// A (ASTM E308-01)
		RefWhite[0] = 1.09850;
		RefWhite[2] = 0.35585;
		break;
	case IlluminantEnum::B:	// B (Wyszecki & Stiles, p. 769)
		RefWhite[0] = 0.99072;
		RefWhite[2] = 0.85223;
		break;
	case IlluminantEnum::C:	// C (ASTM E308-01)
		RefWhite[0] = 0.98074;
		RefWhite[2] = 1.18232;
		break;
	case IlluminantEnum::D50:	// D50 (ASTM E308-01)
		RefWhite[0] = 0.96422;
		RefWhite[2] = 0.82521;
		break;
	case IlluminantEnum::D55:	// D55 (ASTM E308-01)
		RefWhite[0] = 0.95682;
		RefWhite[2] = 0.92149;
		break;
	case IlluminantEnum::D65:	// D65 (ASTM E308-01)
		RefWhite[0] = 0.95047;
		RefWhite[2] = 1.08883;
		break;
	case IlluminantEnum::D75:	// D75 (ASTM E308-01)
		RefWhite[0] = 0.94972;
		RefWhite[2] = 1.22638;
		break;
	default:
	case IlluminantEnum::E:	// E (ASTM E308-01)
		RefWhite[0] = 1.00000;
		RefWhite[2] = 1.00000;
		break;
	case IlluminantEnum::F2:	// F2 (ASTM E308-01)
		RefWhite[0] = 0.99186;
		RefWhite[2] = 0.67393;
		break;
	case IlluminantEnum::F7:	// F7 (ASTM E308-01)
		RefWhite[0] = 0.95041;
		RefWhite[2] = 1.08747;
		break;
	case IlluminantEnum::F11:	// F11 (ASTM E308-01)
		RefWhite[0] = 1.00962;
		RefWhite[2] = 0.64350;
		break;
	}
}

/*!
	\brief Create parameters for RGB color space
	\param[in] color_space - Color space (see RgbEnum)
	\param[in] adaptation - Chromatic adaptation method (see AdaptationEnum)
	\param[in] illuminant - Illuminant (see IlluminantEnum)
	\return RGB ColorSpace parameters
*/
RgbParams get_rgb_params(RgbEnum color_space, AdaptationEnum adaptation, IlluminantEnum illuminant)
{
	RgbParams result;
	result.RefWhiteRGB[1] = 1.00000;
	double xr, yr, xg, yg, xb, yb;

	switch (color_space)
	{
	case RgbEnum::AdobeRgb:	/* Adobe RGB (1998) */
		xr = 0.64;
		yr = 0.33;
		xg = 0.21;
		yg = 0.71;
		xb = 0.15;
		yb = 0.06;

		result.RefWhiteRGB[0] = 0.95047;
		result.RefWhiteRGB[2] = 1.08883;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::AppleRgb:	/* AppleRGB */
		xr = 0.625;
		yr = 0.340;
		xg = 0.280;
		yg = 0.595;
		xb = 0.155;
		yb = 0.070;

		result.RefWhiteRGB[0] = 0.95047;
		result.RefWhiteRGB[2] = 1.08883;

		result.GammaRGB = 1.8;
		break;
	case RgbEnum::BestRgb:	/* Best RGB */
		xr = 0.7347;
		yr = 0.2653;
		xg = 0.2150;
		yg = 0.7750;
		xb = 0.1300;
		yb = 0.0350;

		result.RefWhiteRGB[0] = 0.96422;
		result.RefWhiteRGB[2] = 0.82521;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::BetaRgb:	/* Beta RGB */
		xr = 0.6888;
		yr = 0.3112;
		xg = 0.1986;
		yg = 0.7551;
		xb = 0.1265;
		yb = 0.0352;

		result.RefWhiteRGB[0] = 0.96422;
		result.RefWhiteRGB[2] = 0.82521;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::BruceRgb:	/* Bruce RGB */
		xr = 0.64;
		yr = 0.33;
		xg = 0.28;
		yg = 0.65;
		xb = 0.15;
		yb = 0.06;

		result.RefWhiteRGB[0] = 0.95047;
		result.RefWhiteRGB[2] = 1.08883;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::CieRgb:	/* CIE RGB */
		xr = 0.735;
		yr = 0.265;
		xg = 0.274;
		yg = 0.717;
		xb = 0.167;
		yb = 0.009;

		result.RefWhiteRGB[0] = 1.00000;
		result.RefWhiteRGB[2] = 1.00000;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::ColorMatchRgb:	/* ColorMatch RGB */
		xr = 0.630;
		yr = 0.340;
		xg = 0.295;
		yg = 0.605;
		xb = 0.150;
		yb = 0.075;

		result.RefWhiteRGB[0] = 0.96422;
		result.RefWhiteRGB[2] = 0.82521;

		result.GammaRGB = 1.8;
		break;
	case RgbEnum::DonRgb4:	/* Don RGB 4 */
		xr = 0.696;
		yr = 0.300;
		xg = 0.215;
		yg = 0.765;
		xb = 0.130;
		yb = 0.035;

		result.RefWhiteRGB[0] = 0.96422;
		result.RefWhiteRGB[2] = 0.82521;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::EciRgb2:	/* ECI RGB v2 */
		xr = 0.67;
		yr = 0.33;
		xg = 0.21;
		yg = 0.71;
		xb = 0.14;
		yb = 0.08;

		result.RefWhiteRGB[0] = 0.96422;
		result.RefWhiteRGB[2] = 0.82521;

		result.GammaRGB = 0.0;
		break;
	case RgbEnum::EktaSpacePS5:	/* Ekta Space PS5 */
		xr = 0.695;
		yr = 0.305;
		xg = 0.260;
		yg = 0.700;
		xb = 0.110;
		yb = 0.005;

		result.RefWhiteRGB[0] = 0.96422;
		result.RefWhiteRGB[2] = 0.82521;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::NtscRgb:	/* NTSC RGB */
		xr = 0.67;
		yr = 0.33;
		xg = 0.21;
		yg = 0.71;
		xb = 0.14;
		yb = 0.08;

		result.RefWhiteRGB[0] = 0.98074;
		result.RefWhiteRGB[2] = 1.18232;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::PalSecamRgb:	/* PAL/SECAM RGB */
		xr = 0.64;
		yr = 0.33;
		xg = 0.29;
		yg = 0.60;
		xb = 0.15;
		yb = 0.06;

		result.RefWhiteRGB[0] = 0.95047;
		result.RefWhiteRGB[2] = 1.08883;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::ProPhotoRgb:	/* ProPhoto RGB */
		xr = 0.7347;
		yr = 0.2653;
		xg = 0.1596;
		yg = 0.8404;
		xb = 0.0366;
		yb = 0.0001;

		result.RefWhiteRGB[0] = 0.96422;
		result.RefWhiteRGB[2] = 0.82521;

		result.GammaRGB = 1.8;
		break;
	case RgbEnum::SmpteCRgb:	/* SMPTE-C RGB */
		xr = 0.630;
		yr = 0.340;
		xg = 0.310;
		yg = 0.595;
		xb = 0.155;
		yb = 0.070;

		result.RefWhiteRGB[0] = 0.95047;
		result.RefWhiteRGB[2] = 1.08883;

		result.GammaRGB = 2.2;
		break;
	case RgbEnum::sRGB:	/* sRGB */
		xr = 0.64;
		yr = 0.33;
		xg = 0.30;
		yg = 0.60;
		xb = 0.15;
		yb = 0.06;

		result.RefWhiteRGB[0] = 0.95047;
		result.RefWhiteRGB[2] = 1.08883;

		result.GammaRGB = -2.2;
		break;
	case RgbEnum::WideGamutRgb:	/* Wide Gamut RGB */
		xr = 0.735;
		yr = 0.265;
		xg = 0.115;
		yg = 0.826;
		xb = 0.157;
		yb = 0.018;

		result.RefWhiteRGB[0] = 0.96422;
		result.RefWhiteRGB[2] = 0.82521;

		result.GammaRGB = 2.2;
		break;
	}

	Mtx3x3 m = { {xr / yr, xg / yg, xb / yb}, {1.0, 1.0, 1.0}, {(1.0 - xr - yr) / yr, (1.0 - xg - yg) / yg, (1.0 - xb - yb) / yb} };
	Mtx3x3 mi = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
	MtxInvert3x3(m, mi);

	double sr = result.RefWhiteRGB[0] * mi[0][0] + result.RefWhiteRGB[1] * mi[0][1] + result.RefWhiteRGB[2] * mi[0][2];
	double sg = result.RefWhiteRGB[0] * mi[1][0] + result.RefWhiteRGB[1] * mi[1][1] + result.RefWhiteRGB[2] * mi[1][2];
	double sb = result.RefWhiteRGB[0] * mi[2][0] + result.RefWhiteRGB[1] * mi[2][1] + result.RefWhiteRGB[2] * mi[2][2];

	result.MtxRGB2XYZ[0][0] = sr * m[0][0];
	result.MtxRGB2XYZ[0][1] = sg * m[0][1];
	result.MtxRGB2XYZ[0][2] = sb * m[0][2];
	result.MtxRGB2XYZ[1][0] = sr * m[1][0];
	result.MtxRGB2XYZ[1][1] = sg * m[1][1];
	result.MtxRGB2XYZ[1][2] = sb * m[1][2];
	result.MtxRGB2XYZ[2][0] = sr * m[2][0];
	result.MtxRGB2XYZ[2][1] = sg * m[2][1];
	result.MtxRGB2XYZ[2][2] = sb * m[2][2];

	MtxTranspose3x3(result.MtxRGB2XYZ);

	MtxInvert3x3(result.MtxRGB2XYZ, result.MtxXYZ2RGB);

	// Adaptation
	result.AdaptationMethod = adaptation;
	
	GetRefWhite(result.RefWhite, illuminant);

	return result;
}

/*!
    \brief Conversion from RGB to XYZ color model
    \param[in] r - red channel in 0..1.0 range
	\param[in] g - green channel in 0..1.0 range
 	\param[in] b - blue channel in 0..1.0 range
 	\param[out] x - x channel in 0..1.0 range
  	\param[out] y - y channel in 0..1.0 range
  	\param[out] z - z channel in 0..1.0 range
	\param[in] params - RGB ColorSpace parameters
*/
void rgb_to_xyz(double r, double g, double b,
	double& x, double& y, double& z, const RgbParams& params)
{
	r = InvCompand(r, params.GammaRGB);
	g = InvCompand(g, params.GammaRGB);
	b = InvCompand(b, params.GammaRGB);
	
	x = r * params.MtxRGB2XYZ[0][0] + g * params.MtxRGB2XYZ[1][0] + b * params.MtxRGB2XYZ[2][0];
	y = r * params.MtxRGB2XYZ[0][1] + g * params.MtxRGB2XYZ[1][1] + b * params.MtxRGB2XYZ[2][1];
	z = r * params.MtxRGB2XYZ[0][2] + g * params.MtxRGB2XYZ[1][2] + b * params.MtxRGB2XYZ[2][2];

	if (params.AdaptationMethod != AdaptationEnum::amNone)
	{
		auto& MtxAdaptMa = Adaptations[static_cast<size_t>(params.AdaptationMethod)][0];
		auto& MtxAdaptMaI = Adaptations[static_cast<size_t>(params.AdaptationMethod)][1];
		auto& RefWhite = params.RefWhite;
		auto& RefWhiteRGB = params.RefWhiteRGB;

		auto Ad = RefWhite[0] * MtxAdaptMa[0][0] + RefWhite[1] * MtxAdaptMa[1][0] + RefWhite[2] * MtxAdaptMa[2][0];
		auto Bd = RefWhite[0] * MtxAdaptMa[0][1] + RefWhite[1] * MtxAdaptMa[1][1] + RefWhite[2] * MtxAdaptMa[2][1];
		auto Cd = RefWhite[0] * MtxAdaptMa[0][2] + RefWhite[1] * MtxAdaptMa[1][2] + RefWhite[2] * MtxAdaptMa[2][2];

		auto As = RefWhiteRGB[0] * MtxAdaptMa[0][0] + RefWhiteRGB[1] * MtxAdaptMa[1][0] + RefWhiteRGB[2] * MtxAdaptMa[2][0];
		auto Bs = RefWhiteRGB[0] * MtxAdaptMa[0][1] + RefWhiteRGB[1] * MtxAdaptMa[1][1] + RefWhiteRGB[2] * MtxAdaptMa[2][1];
		auto Cs = RefWhiteRGB[0] * MtxAdaptMa[0][2] + RefWhiteRGB[1] * MtxAdaptMa[1][2] + RefWhiteRGB[2] * MtxAdaptMa[2][2];

		auto X = x * MtxAdaptMa[0][0] + y * MtxAdaptMa[1][0] + z * MtxAdaptMa[2][0];
		auto Y = x * MtxAdaptMa[0][1] + y * MtxAdaptMa[1][1] + z * MtxAdaptMa[2][1];
		auto Z = x * MtxAdaptMa[0][2] + y * MtxAdaptMa[1][2] + z * MtxAdaptMa[2][2];

		X *= (Ad / As);
		Y *= (Bd / Bs);
		Z *= (Cd / Cs);

		x = X * MtxAdaptMaI[0][0] + Y * MtxAdaptMaI[1][0] + Z * MtxAdaptMaI[2][0];
		y = X * MtxAdaptMaI[0][1] + Y * MtxAdaptMaI[1][1] + Z * MtxAdaptMaI[2][1];
		z = X * MtxAdaptMaI[0][2] + Y * MtxAdaptMaI[1][2] + Z * MtxAdaptMaI[2][2];
	}
}

/*!
    \brief Conversion from XYZ to RGB color model
	\param[in] x - x channel in 0..1.0 range
  	\param[in] y - y channel in 0..1.0 range
  	\param[in] z - z channel in 0..1.0 range
    \param[out] r - red channel in 0..1.0 range
	\param[out] g - green channel in 0..1.0 range
 	\param[out] b - blue channel in 0..1.0 range
	\param[in] params - RGB ColorSpace parameters
*/
void xyz_to_rgb(double x, double y, double z,
	double& r, double& g, double& b, const RgbParams& params)
{
	if (params.AdaptationMethod != AdaptationEnum::amNone)
	{
		auto& MtxAdaptMa = Adaptations[static_cast<size_t>(params.AdaptationMethod)][0];
		auto& MtxAdaptMaI = Adaptations[static_cast<size_t>(params.AdaptationMethod)][1];
		auto& RefWhite = params.RefWhite;
		auto& RefWhiteRGB = params.RefWhiteRGB;

		double As = RefWhite[0] * MtxAdaptMa[0][0] + RefWhite[1] * MtxAdaptMa[1][0] + RefWhite[2] * MtxAdaptMa[2][0];
		double Bs = RefWhite[0] * MtxAdaptMa[0][1] + RefWhite[1] * MtxAdaptMa[1][1] + RefWhite[2] * MtxAdaptMa[2][1];
		double Cs = RefWhite[0] * MtxAdaptMa[0][2] + RefWhite[1] * MtxAdaptMa[1][2] + RefWhite[2] * MtxAdaptMa[2][2];

		double Ad = RefWhiteRGB[0] * MtxAdaptMa[0][0] + RefWhiteRGB[1] * MtxAdaptMa[1][0] + RefWhiteRGB[2] * MtxAdaptMa[2][0];
		double Bd = RefWhiteRGB[0] * MtxAdaptMa[0][1] + RefWhiteRGB[1] * MtxAdaptMa[1][1] + RefWhiteRGB[2] * MtxAdaptMa[2][1];
		double Cd = RefWhiteRGB[0] * MtxAdaptMa[0][2] + RefWhiteRGB[1] * MtxAdaptMa[1][2] + RefWhiteRGB[2] * MtxAdaptMa[2][2];

		double X1 = x * MtxAdaptMa[0][0] + y * MtxAdaptMa[1][0] + z * MtxAdaptMa[2][0];
		double Y1 = x * MtxAdaptMa[0][1] + y * MtxAdaptMa[1][1] + z * MtxAdaptMa[2][1];
		double Z1 = x * MtxAdaptMa[0][2] + y * MtxAdaptMa[1][2] + z * MtxAdaptMa[2][2];

		X1 *= (Ad / As);
		Y1 *= (Bd / Bs);
		Z1 *= (Cd / Cs);

		x = X1 * MtxAdaptMaI[0][0] + Y1 * MtxAdaptMaI[1][0] + Z1 * MtxAdaptMaI[2][0];
		y = X1 * MtxAdaptMaI[0][1] + Y1 * MtxAdaptMaI[1][1] + Z1 * MtxAdaptMaI[2][1];
		z = X1 * MtxAdaptMaI[0][2] + Y1 * MtxAdaptMaI[1][2] + Z1 * MtxAdaptMaI[2][2];
	}
	
	r = Compand(x * params.MtxXYZ2RGB[0][0] + y * params.MtxXYZ2RGB[1][0] + z * params.MtxXYZ2RGB[2][0], params.GammaRGB);
	g = Compand(x * params.MtxXYZ2RGB[0][1] + y * params.MtxXYZ2RGB[1][1] + z * params.MtxXYZ2RGB[2][1], params.GammaRGB);
	b = Compand(x * params.MtxXYZ2RGB[0][2] + y * params.MtxXYZ2RGB[1][2] + z * params.MtxXYZ2RGB[2][2], params.GammaRGB);
}

}


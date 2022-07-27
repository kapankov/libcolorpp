/*!
\file rgb.h
\authors Konstantin A. Pankov, explorus@mail.ru
\copyright MIT License
*/

#pragma once

namespace colorpp 
{

/*!
	\brief RGB ColorSpaces enum
*/
enum class RgbEnum
{
	AdobeRgb = 0,
	AppleRgb = 1,
	BestRgb = 2,
	BetaRgb = 3,
	BruceRgb = 4,
	CieRgb = 5,
	ColorMatchRgb = 6,
	DonRgb4 = 7,
	EciRgb2 = 8,
	EktaSpacePS5 = 9,
	NtscRgb = 10,
	PalSecamRgb = 11,
	ProPhotoRgb = 12,
	SmpteCRgb = 13,
	sRGB = 14,
	WideGamutRgb = 15
};

/*!
	\brief Chromatic adaptation methods enum
*/
enum class AdaptationEnum
{
	amBradford = 0,
	amVonKries = 1,
//	amXYZScaling = 2,
	amNone = 2
};

/*!
	\brief Illuminant enum
*/
enum class IlluminantEnum
{
	A = 0,
	B = 1,
	C = 2,
	D50 = 3,
	D55 = 4,
	D65 = 5,
	D75 = 6,
	E = 7,
	F2 = 8,
	F7 = 9,
	F11 = 10
};

// we won't create a special class for matrices and XYZ
using Mtx3x3 = double[3][3];
using XYZ = double[3];

/*!
	\brief RGB ColorSpace parameters
*/
typedef struct _RgbParams
{
	XYZ RefWhiteRGB;
	double GammaRGB;
	Mtx3x3 MtxRGB2XYZ;
	Mtx3x3 MtxXYZ2RGB;
	AdaptationEnum AdaptationMethod;
	XYZ RefWhite; // for adaptation
	
} RgbParams;

/*!
	\brief Create parameters for RGB color space
	\param[in] color_space - Color space (see RgbEnum)
	\param[in] adaptation - Chromatic adaptation method (see AdaptationEnum)
	\param[in] illuminant - Illuminant (see IlluminantEnum)
	\return RGB ColorSpace parameters
*/
RgbParams get_rgb_params(RgbEnum color_space = RgbEnum::sRGB, 
	AdaptationEnum adaptation = AdaptationEnum::amBradford,
	IlluminantEnum illuminant = IlluminantEnum::D50);

/*!
    \brief Conversion from RGB to XYZ color model
    \param[in] r - red channel in 0..1.0 range
	\param[in] g - green channel in 0..1.0 range
 	\param[in] b - blue channel in 0..1.0 range
	\param[in] params - RGB ColorSpace parameters
 	\param[out] x - x channel in 0..1.0 range
  	\param[out] y - y channel in 0..1.0 range
  	\param[out] z - z channel in 0..1.0 range
*/
void rgb_to_xyz(double r, double g, double b,
	double& x, double& y, double& z, const RgbParams& params = get_rgb_params());

/*!
    \brief Conversion from XYZ to RGB color model
	\param[in] x - x channel in 0..1.0 range
  	\param[in] y - y channel in 0..1.0 range
  	\param[in] z - z channel in 0..1.0 range
	\param[in] params - RGB ColorSpace parameters
    \param[out] r - red channel in 0..1.0 range
	\param[out] g - green channel in 0..1.0 range
 	\param[out] b - blue channel in 0..1.0 range
*/
void xyz_to_rgb(double x, double y, double z, 
	double& r, double& g, double& b, const RgbParams& params = get_rgb_params());

}


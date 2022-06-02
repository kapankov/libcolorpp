/*!
\file hsl.h
\authors Konstantin A. Pankov, explorus@mail.ru
\copyright MIT License
*/

#pragma once

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
	double& h, double& s, double& l);

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
	double& r, double& g, double& b);

}


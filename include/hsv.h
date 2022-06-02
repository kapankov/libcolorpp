/*!
\file hsv.h
\authors Konstantin A. Pankov, explorus@mail.ru
\copyright MIT License
*/

#pragma once

namespace colorpp 
{

/*!
    \brief Conversion from RGB to HSV color model
    \param[in] r - red channel in 0..1.0 range
	\param[in] g - green channel in 0..1.0 range
 	\param[in] b - blue channel in 0..1.0 range
 	\param[out] h - hue channel in 0..1.0 range
  	\param[out] s - saturation channel in 0..1.0 range
  	\param[out] v - value channel in 0..1.0 range
*/
void rgb_to_hsv(double r, double g, double b,
	double& h, double& s, double& v);

/*!
    \brief Conversion from HSV to RGB color model
	\param[in] h - hue channel in 0..1.0 range
  	\param[in] s - saturation channel in 0..1.0 range
  	\param[in] v - value channel in 0..1.0 range
    \param[out] r - red channel in 0..1.0 range
	\param[out] g - green channel in 0..1.0 range
 	\param[out] b - blue channel in 0..1.0 range
*/
void hsv_to_rgb(double h, double s, double v, 
	double& r, double& g, double& b);

}


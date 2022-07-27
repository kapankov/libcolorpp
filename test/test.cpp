#include <iostream>
#include <cmath>

#include "gtest/gtest.h"
#include "color.h"

TEST(rgb_to_hsv_to_rgb, colorpp_proc_test)
{
	const double value_epsilon = 8.882e-16;
	for (int r = 0; r < 256; ++r)
		for (int g = 0; g < 256; ++g)
			for (int b = 0; b < 256; ++b)
			{
				double R = static_cast<double>(r)/255.;
				double G = static_cast<double>(g)/255.;
				double B = static_cast<double>(b)/255.;
				double H = 0.;
				double S = 0.;
				double V = 0.;
				colorpp::rgb_to_hsv(R, G, B, H, S, V);
				double r_res = 0.;
				double g_res = 0.;
				double b_res = 0.;
				colorpp::hsv_to_rgb(H, S, V, r_res, g_res, b_res);
				ASSERT_LT(std::abs(R - r_res), value_epsilon) << 
					"rgb is: " << r << ", " << g << ", " << b;
				ASSERT_LT(std::abs(G - g_res), value_epsilon) << 
					"rgb is: " << r << ", " << g << ", " << b;
				ASSERT_LT(std::abs(B - b_res), value_epsilon) << 
					"rgb is: " << r << ", " << g << ", " << b;
			}
}

TEST(hsv_to_rgb_to_hsv, colorpp_proc_test)
{
	const double value_epsilon = 3.109e-15;
    for (int h = 0; h < 361; ++h)
        for (int s = 1; s < 101; ++s) // TODO: test for nullable saturation
            for (int v = 1; v < 101; ++v) // TODO: test for nullable value
            {
                double H = static_cast<double>(h)/360.;
                double S = static_cast<double>(s)/100.;
                double V = static_cast<double>(v)/100.;
				double R = 0.;
				double G = 0.;
				double B = 0.;
	            colorpp::hsv_to_rgb(H, S, V, R, G, B);
				double h_res = 0.;
				double s_res = 0.;
				double v_res = 0.;
                colorpp::rgb_to_hsv(R, G, B, h_res, s_res, v_res);
				if (H == 1)
					H = 0;
				ASSERT_LT(std::abs(H - h_res), value_epsilon) << 
					"hsv is: " << h << ", " << s << ", " << v;
				ASSERT_LT(std::abs(S - s_res), value_epsilon) << 
					"hsv is: " << h << ", " << s << ", " << v;
				ASSERT_LT(std::abs(V - v_res), value_epsilon) << 
					"hsv is: " << h << ", " << s << ", " << v;
			}
}

TEST(rgb_to_hsl_to_rgb, colorpp_proc_test)
{
	const double value_epsilon = 8.882e-16;
	for (int r = 0; r < 256; ++r)
		for (int g = 0; g < 256; ++g)
			for (int b = 0; b < 256; ++b)
			{
				double R = static_cast<double>(r)/255.;
				double G = static_cast<double>(g)/255.;
				double B = static_cast<double>(b)/255.;
				double H = 0.;
				double S = 0.;
				double L = 0.;
				colorpp::rgb_to_hsl(R, G, B, H, S, L);
				double r_res = 0.;
				double g_res = 0.;
				double b_res = 0.;
				colorpp::hsl_to_rgb(H, S, L, r_res, g_res, b_res);
				ASSERT_LT(std::abs(R - r_res), value_epsilon) << 
					"rgb is: " << r << ", " << g << ", " << b;
				ASSERT_LT(std::abs(G - g_res), value_epsilon) << 
					"rgb is: " << r << ", " << g << ", " << b;
				ASSERT_LT(std::abs(B - b_res), value_epsilon) << 
					"rgb is: " << r << ", " << g << ", " << b;
			}
}

TEST(hsl_to_rgb_to_hsl, colorpp_proc_test)
{
	const double value_epsilon = 5.563e-14;
    for (int h = 0; h < 361; ++h)
        for (int s = 1; s < 101; ++s) // TODO: test for nullable saturation
            for (int l = 1; l < 100; ++l) // TODO: test for nullable lightness
            {
                double H = static_cast<double>(h)/360.;
                double S = static_cast<double>(s)/100.;
                double L = static_cast<double>(l)/100.;
				double R = 0.;
				double G = 0.;
				double B = 0.;
	            colorpp::hsl_to_rgb(H, S, L, R, G, B);
				double h_res = 0.;
				double s_res = 0.;
				double l_res = 0.;
                colorpp::rgb_to_hsl(R, G, B, h_res, s_res, l_res);
				if (H == 1)
					H = 0;
				ASSERT_LT(std::abs(H - h_res), value_epsilon) << 
					"hsl is: " << h << ", " << s << ", " << l;
				ASSERT_LT(std::abs(S - s_res), value_epsilon) << 
					"hsl is: " << h << ", " << s << ", " << l;
				ASSERT_LT(std::abs(L - l_res), value_epsilon) << 
					"hsl is: " << h << ", " << s << ", " << l;
			}
}

TEST(rgb_to_hsv_to_rgb, colorpp_class_test)
{
	const double value_epsilon = 8.882e-16;
	for (int r = 0; r < 256; ++r)
		for (int g = 0; g < 256; ++g)
			for (int b = 0; b < 256; ++b)
			{
				colorpp::rgb rgb(static_cast<double>(r)/255.,
					static_cast<double>(g)/255.,
					static_cast<double>(b)/255.);
				colorpp::hsv hsv(rgb);
				colorpp::rgb result(hsv);
				ASSERT_LT(std::abs(rgb.get_red() - result.get_red()), value_epsilon) << 
					"rgb is: " << r << ", " << g << ", " << b;
				ASSERT_LT(std::abs(rgb.get_green() - result.get_green()), value_epsilon) << 
					"rgb is: " << r << ", " << g << ", " << b;
				ASSERT_LT(std::abs(rgb.get_blue() - result.get_blue()), value_epsilon) << 
					"rgb is: " << r << ", " << g << ", " << b;
			}

}

TEST(rgb_to_xyz_to_rgb, colorpp_proc_test)
{
	const double value_epsilon = 4.495e-06;
	auto params = colorpp::get_rgb_params();
	for (int r = 0; r < 256; ++r)
		for (int g = 0; g < 256; ++g)
			for (int b = 0; b < 256; ++b)
			{
				double R = static_cast<double>(r) / 255.;
				double G = static_cast<double>(g) / 255.;
				double B = static_cast<double>(b) / 255.;
				double X = 0.;
				double Y = 0.;
				double Z = 0.;
				colorpp::rgb_to_xyz(R, G, B, X, Y, Z, params);
				double r_res = 0.;
				double g_res = 0.;
				double b_res = 0.;
				colorpp::xyz_to_rgb(X, Y, Z, r_res, g_res, b_res, params);
				ASSERT_LT(std::abs(R - r_res), value_epsilon) <<
					"rgb is: " << r << ", " << g << ", " << b;
				ASSERT_LT(std::abs(G - g_res), value_epsilon) <<
					"rgb is: " << r << ", " << g << ", " << b;
				ASSERT_LT(std::abs(B - b_res), value_epsilon) <<
					"rgb is: " << r << ", " << g << ", " << b;
			}
}
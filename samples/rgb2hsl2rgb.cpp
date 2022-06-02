#include <iostream>
#include <list>
#include <tuple>
#include "hsl.h"

int main()
{
	std::list<std::tuple<double, double, double, double, double, double>> difflist;
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

				if (difflist.empty())
					difflist.push_front(std::make_tuple(R, G, B, 0, 0, 0));
				else
				{
					auto last = difflist.front();
					auto max_diff = std::max(std::max(std::get<3>(last), std::get<4>(last)), std::get<5>(last));
					auto r_diff = std::abs(R - r_res);
					auto g_diff = std::abs(G - g_res);
					auto b_diff = std::abs(B - b_res);
					auto new_max_diff = std::max(std::max(r_diff, g_diff), b_diff);
					if (new_max_diff > max_diff)
						difflist.push_front(std::make_tuple(R, G, B, r_diff, g_diff, b_diff));
					if (difflist.size() > 100)
						difflist.pop_back();
				}
			}

	for (const auto& it: difflist)
	{
		std::cout << std::get<0>(it) << "\t" << std::get<1>(it) << "\t" << std::get<2>(it) << "\t";
		std::cout << std::get<3>(it) << "\t" << std::get<4>(it) << "\t" << std::get<5>(it) << std::endl;
	}

	return 0;
}

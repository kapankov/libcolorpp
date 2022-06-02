#include <iostream>
#include <list>
#include <tuple>
#include "hsv.h"

int main()
{
	std::list<std::tuple<double, double, double, double, double, double>> difflist;
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

				if (difflist.empty())
					difflist.push_front(std::make_tuple(H, S, V, 0, 0, 0));
				else
				{
					auto last = difflist.front();
					auto max_diff = std::max(std::max(std::get<3>(last), std::get<4>(last)), std::get<4>(last));
					auto h_diff = std::abs(H - h_res);
					auto s_diff = std::abs(S - s_res);
					auto v_diff = std::abs(V - v_res);
					auto new_max_diff = std::max(std::max(h_diff, s_diff), v_diff);
					if (new_max_diff > max_diff)
						difflist.push_front(std::make_tuple(H, S, V, h_diff, s_diff, v_diff));
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

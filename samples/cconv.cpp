#include <iostream>
#include <tuple>
#include <regex>

#include "color.h"
#include "utils/clparser.h"

std::tuple<int, int, int> extract_from_tristr(std::string s)
{
    std::regex rgx("(\\d+),(\\d+),(\\d+)");
    std::smatch match;

    if (std::regex_match(s, match, rgx))
        return std::make_tuple<int, int, int>(
            std::atoi(match[1].str().c_str()),
            std::atoi(match[2].str().c_str()),
            std::atoi(match[3].str().c_str())
        );

    return std::make_tuple<int, int, int>(-1, -1, -1);
}


bool rgb_convert(std::string inp_value, std::string out_mod)
{
	if (out_mod != "hsv" && out_mod != "hsl" && out_mod != "all")
		return false;

	auto inp_rgb = extract_from_tristr(inp_value);
	if (std::get<0>(inp_rgb) < 0 ||
		std::get<1>(inp_rgb) < 0 ||
		std::get<2>(inp_rgb) < 0)
		return false;

	colorpp::rgb256 rgb{static_cast<colorpp::byte::type>(std::get<0>(inp_rgb)), 
		static_cast<colorpp::byte::type>(std::get<1>(inp_rgb)), 
		static_cast<colorpp::byte::type>(std::get<2>(inp_rgb)) };

	std::cout << "rgb: " << rgb << std::endl;

	if (out_mod == "hsv" || out_mod == "all")
	{
		colorpp::hsv360_100 hsv(rgb);
		std::cout << "hsv: " << hsv << std::endl;
	}
	if (out_mod == "hsl" || out_mod == "all")
	{
		colorpp::hsl360_100 hsl(rgb);
		std::cout << "hsl: " << hsl << std::endl;
	}
	return true;
}

bool hsv_convert(std::string inp_value, std::string out_mod)
{
	if (out_mod != "rgb" && out_mod != "hsl" && out_mod != "all")
		return false;

	auto inp_hsv = extract_from_tristr(inp_value);
	if (std::get<0>(inp_hsv) < 0 ||
		std::get<1>(inp_hsv) < 0 ||
		std::get<2>(inp_hsv) < 0)
		return false;

	colorpp::hsv360_100 hsv{static_cast<colorpp::word360::type>(std::get<0>(inp_hsv)), 
		static_cast<colorpp::byte100::type>(std::get<1>(inp_hsv)), 
		static_cast<colorpp::byte100::type>(std::get<2>(inp_hsv)) };

	std::cout << "hsv: " << hsv << std::endl;

	colorpp::rgb256 rgb(hsv);
	if (out_mod == "rgb" || out_mod == "all")
		std::cout << "rgb: " << rgb << std::endl;
	if (out_mod == "hsl" || out_mod == "all")
	{
		colorpp::hsl360_100 hsl(rgb);
		std::cout << "hsl: " << hsl << std::endl;
	}
	return true;
}

bool hsl_convert(std::string inp_value, std::string out_mod)
{
	if (out_mod != "rgb" && out_mod != "hsv" && out_mod != "all")
		return false;

	auto inp_hsl = extract_from_tristr(inp_value);
	if (std::get<0>(inp_hsl) < 0 ||
		std::get<1>(inp_hsl) < 0 ||
		std::get<2>(inp_hsl) < 0)
		return false;

	colorpp::hsl360_100 hsl{static_cast<colorpp::word360::type>(std::get<0>(inp_hsl)), 
		static_cast<colorpp::byte100::type>(std::get<1>(inp_hsl)), 
		static_cast<colorpp::byte100::type>(std::get<2>(inp_hsl)) };

	std::cout << "hsl: " << hsl << std::endl;

	colorpp::rgb256 rgb(hsl);
	if (out_mod == "rgb" || out_mod == "all")
		std::cout << "rgb: " << rgb << std::endl;
	if (out_mod == "hsv" || out_mod == "all")
	{
		colorpp::hsv360_100 hsv(rgb);
		std::cout << "hsv: " << hsv << std::endl;
	}
	return true;

}

int main(int argc, char* argv[])
{
	const char* valPtrn = "\\d+,\\d+,\\d+";
	const char* splPtrn = " :=";
	const char* prmRgb = "-rgb";
	const char* prmHsv = "-hsv";
	const char* prmHsl = "-hsl";
	const char* prmOut = "-o";
	clparser cmdline;
	cmdline.add_param(prmRgb, splPtrn, valPtrn);
	cmdline.add_param(prmHsv, splPtrn, valPtrn);
	cmdline.add_param(prmHsl, splPtrn, valPtrn);
	cmdline.add_param(prmOut, splPtrn, "\\S+");
	bool is_cl_invalid = true;
	while(1)
	{
		if (!cmdline.parse(argc, argv))
			break;
		auto options_count = cmdline.get_options_count(); 
		if (options_count < 1 || options_count > 2)
			break;
		if (!cmdline.is_exists(prmRgb) && 
			!cmdline.is_exists(prmHsv) && 
			!cmdline.is_exists(prmHsl))
			break;
		std::string out_mod = "all";
		int out_pos = 1;
		if (cmdline.is_exists(prmOut))
		{
			out_pos = cmdline.get_name(0) == prmOut ? 0 : 1;
			out_mod = cmdline.get_value(prmOut);
		}
		auto inp_name = cmdline.get_name(out_pos == 0 ? 1 : 0);
		if (inp_name == prmRgb)
			is_cl_invalid = !rgb_convert(cmdline.get_value(inp_name), out_mod);
		else if (inp_name == prmHsv)
			is_cl_invalid = !hsv_convert(cmdline.get_value(inp_name), out_mod);
		else if (inp_name == prmHsl)
			is_cl_invalid = !hsl_convert(cmdline.get_value(inp_name), out_mod);
		break;
	}
	if (is_cl_invalid)
	{
		std::cout << "Usage: cconv -mod=x,y,z -o=mod\n"
		"\twhere\n"
		"\t\tmod is rgb, hsv or hsl\n"
		"\t\tx,y,z are values from 0 to 255" << std::endl;
	}

	return 0;
}

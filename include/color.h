/*!
\file color.h
\brief This file contains the source code of Color++ library
\authors Konstantin A. Pankov, explorus@mail.ru
\copyright MIT License
\version 1.0
\date 29/05/2022 (DD/MM/YYYY)
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
#pragma once

#include "hsv.h"
#include "hsl.h"

namespace colorpp
{

template <typename T, int min_value, int max_value>
struct base_type
{
	using type = T;
	static T min() { return min_value; }
	static T max() { return max_value; }
};

using dbl = base_type<double, 0, 1>;
using word = base_type<unsigned short, 0, 65535>;
using byte = base_type<unsigned char, 0, 255>;
using byte100 = base_type<unsigned char, 0, 100>;
using word360 = base_type<unsigned short, 0, 360>;

//======================== helpers ======================== 
template<typename T>
double get_dbl(typename T::type v)
{
	if (std::is_same<T, dbl>::value)
		return v;
	double result = v;
	return result/(T::max() - T::min() + (std::is_integral<typename T::type>::value? 1: 0.));
}
template<typename T>
typename T::type from_dbl(double v)
{
	if (std::is_same<T, dbl>::value)
		return v;
	return static_cast<typename T::type>(T::min() + v * (T::max() - T::min() + (std::is_integral<typename T::type>::value? 1: 0.)));
}

template<typename Th, typename Tsv>
class hsv_base;
template<typename Th, typename Tsl>
class hsl_base;

//========================== RGB ========================== 
template<typename T>
class rgb_base
{
public: // constructors/operators
	rgb_base() = default;
	rgb_base(const rgb_base&) = default;
	rgb_base(rgb_base&&) noexcept = default;
	rgb_base(typename T::type r, typename T::type g, typename T::type b) : r_(r), g_(g), b_(b) {}
	template<typename Th, typename Tsv>
	rgb_base(const hsv_base<Th, Tsv>& hsv)
	{
		operator=(hsv);
	}
	template<typename Th, typename Tsl>
	rgb_base(const hsl_base<Th, Tsl>& hsl)
	{
		operator=(hsl);
	}

	//operators
	rgb_base& operator=(const rgb_base&) = default;
	rgb_base& operator=(rgb_base&&) noexcept = default;
	template<typename Th, typename Tsv>
	rgb_base& operator=(const hsv_base<Th, Tsv>& hsv)
	{
		double r = 0;
		double g = 0;
		double b = 0;
		hsv_to_rgb(get_dbl<Th>(hsv.get_hue()), 
			get_dbl<Tsv>(hsv.get_saturation()),
			get_dbl<Tsv>(hsv.get_value()),
			r, g, b);
		r_ = from_dbl<T>(r);
		g_ = from_dbl<T>(g);
		b_ = from_dbl<T>(b);
		return *this;
	}
	template<typename Th, typename Tsl>
	rgb_base& operator=(const hsl_base<Th, Tsl>& hsl)
	{
		double r = 0;
		double g = 0;
		double b = 0;
		hsl_to_rgb(get_dbl<Th>(hsl.get_hue()), 
			get_dbl<Tsl>(hsl.get_saturation()),
			get_dbl<Tsl>(hsl.get_lightness()),
			r, g, b);
		r_ = from_dbl<T>(r);
		g_ = from_dbl<T>(g);
		b_ = from_dbl<T>(b);
		return *this;
	}

public: // get/set
	typename T::type get_red() const { return r_; }
	void set_red(typename T::type v) { r_ = v; }

	typename T::type get_green() const { return g_; }
	void set_green(typename T::type v) { g_ = v; }

	typename T::type get_blue() const { return b_; }
	void set_blue(typename T::type v) { b_ = v; }

	template<typename Th, typename Tsv>
	void set_hsv(const hsv_base<Th, Tsv>& hsv)
	{
		operator=(hsv);
	}
	template<typename Th, typename Tsl>
	void set_hsl(const hsl_base<Th, Tsl>& hsl)
	{
		operator=(hsl);
	}

	friend std::ostream& operator << (std::ostream& os, const rgb_base& v)
	{
		if (std::is_same<typename T::type, unsigned char>::value)
			return os << static_cast<int>(v.r_) << "," 
				<< static_cast<int>(v.g_) << "," 
				<< static_cast<int>(v.b_);
		return os << v.r_ << "," << v.g_ << "," << v.b_;
	}
private:
	typename T::type r_{T::min()};
	typename T::type g_{T::min()};
	typename T::type b_{T::min()};
};

//========================== HSV ========================== 
template<typename Th, typename Tsv>
class hsv_base
{
public: // constructors/operators
	hsv_base() = default;
	hsv_base(const hsv_base&) = default;
	hsv_base(hsv_base&&) noexcept = default;
	hsv_base(typename Th::type h, typename Tsv::type s, typename Tsv::type v) : h_(h), s_(s), v_(v) {}
	template<typename T>
	hsv_base(const rgb_base<T>& rgb)
	{
		operator=(rgb);
	}

	//operators
	hsv_base& operator=(const hsv_base&) = default;
	hsv_base& operator=(hsv_base&&) noexcept = default;
	template<typename T>
	hsv_base& operator=(const rgb_base<T>& rgb)
	{
		double h = 0;
		double s = 0;
		double v = 0;
		rgb_to_hsv(get_dbl<T>(rgb.get_red()), 
			get_dbl<T>(rgb.get_green()),
			get_dbl<T>(rgb.get_blue()),
			h, s, v);
		h_ = from_dbl<Th>(h);
		s_ = from_dbl<Tsv>(s);
		v_ = from_dbl<Tsv>(v);
		return *this;
	}

public: // get/set
	typename Th::type get_hue() const { return h_; }
	void set_hue(typename Th::type h) { h_ = h; }

	typename Tsv::type get_saturation() const { return s_; }
	void set_saturation(typename Tsv::type s) { s_ = s; }

	typename Tsv::type get_value() const { return v_; }
	void set_value(typename Tsv::type v) { v_ = v; }

	template<typename T>
	void set_rgb(const rgb_base<T>& rgb)
	{
		operator=(rgb);
	}

	friend std::ostream& operator << (std::ostream& os, const hsv_base& v)
	{
		if (std::is_same<typename Th::type, unsigned char>::value)
			os << static_cast<int>(v.h_);
		else
			os << v.h_;
		os << ",";
		if (std::is_same<typename Tsv::type, unsigned char>::value)
			os << static_cast<int>(v.s_) << "," << static_cast<int>(v.v_);
		else
			os << v.s_ << "," << v.v_;
		return os;
	}

private:
	typename Th::type h_{Th::min()};
	typename Tsv::type s_{Tsv::min()};
	typename Tsv::type v_{Tsv::min()};
};

//========================== HSL ========================== 
template<typename Th, typename Tsl>
class hsl_base
{
public: // constructors/operators
	hsl_base() = default;
	hsl_base(const hsl_base&) = default;
	hsl_base(hsl_base&&) noexcept = default;
	hsl_base(typename Th::type h, typename Tsl::type s, typename Tsl::type l) : h_(h), s_(s), l_(l) {}
	template<typename T>
	hsl_base(const rgb_base<T>& rgb)
	{
		operator=(rgb);
	}

	//operators
	hsl_base& operator=(const hsl_base&) = default;
	hsl_base& operator=(hsl_base&&) noexcept = default;
	template<typename T>
	hsl_base& operator=(const rgb_base<T>& rgb)
	{
		double h = 0;
		double s = 0;
		double l = 0;
		rgb_to_hsl(get_dbl<T>(rgb.get_red()), 
			get_dbl<T>(rgb.get_green()),
			get_dbl<T>(rgb.get_blue()),
			h, s, l);
		h_ = from_dbl<Th>(h);
		s_ = from_dbl<Tsl>(s);
		l_ = from_dbl<Tsl>(l);
		return *this;
	}

public: // get/set
	typename Th::type get_hue() const { return h_; }
	void set_hue(typename Th::type h) { h_ = h; }

	typename Tsl::type get_saturation() const { return s_; }
	void set_saturation(typename Tsl::type s) { s_ = s; }

	typename Tsl::type get_lightness() const { return l_; }
	void set_lightness(typename Tsl::type l) { l_ = l; }

	template<typename T>
	void set_rgb(const rgb_base<T>& rgb)
	{
		operator=(rgb);
	}

	friend std::ostream& operator << (std::ostream& os, const hsl_base& v)
	{
		if (std::is_same<typename Th::type, unsigned char>::value)
			os << static_cast<int>(v.h_);
		else
			os << v.h_;
		os << ",";
		if (std::is_same<typename Tsl::type, unsigned char>::value)
			os << static_cast<int>(v.s_) << "," << static_cast<int>(v.l_);
		else
			os << v.s_ << "," << v.l_;
		return os;
	}

private:
	typename Th::type h_{Th::min()};
	typename Tsl::type s_{Tsl::min()};
	typename Tsl::type l_{Tsl::min()};
};

using rgb = rgb_base<dbl>;
using rgb256 = rgb_base<byte>;
using hsv = hsv_base<dbl, dbl>;
using hsv360_100 = hsv_base<word360, byte100>;
using hsl = hsl_base<dbl, dbl>;
using hsl360_100 = hsl_base<word360, byte100>;

} // end namespace colorpp


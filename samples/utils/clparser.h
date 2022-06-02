/*!
\file clparser.h
\brief This file contains the source code of Command Line Parser class
\authors Konstantin A. Pankov, explorus@mail.ru
\copyright MIT License
\version 1.0
\date 15/03/2022 (DD/MM/YYYY)
\warning Unstable code. Under development.

The MIT License

Copyright(c) 2022 Konstantin Pankov, explorus@mail.ru

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

#include <map>
#include <string>
#include <utility>
#include <regex>

class clparser
{
	bool add_option(const std::string& name, const std::string& value)
	{
		if (options_.find(name) != options_.end())
			return false;
		options_.emplace(name, value);
		sequence_.emplace_back(name);
		return true;
	}
public:
	void clear(bool valid_opt = false)
	{
		sequence_.clear();
		options_.clear();
		if (valid_opt)
			valid_options_.clear();
	}

	void add_param(
		const std::string& name, 
		std::string separators = std::string(), 
		std::string revalue = std::string())
	{
		valid_options_.emplace(
			std::move(name), 
			std::make_pair<std::string, std::string>(
				std::move(separators), 
				std::move(revalue)
			)
		);
	}

	size_t get_options_count() const
	{
		return options_.size();
	}

	bool is_exists(const std::string& name) const
	{
		return options_.find(name) != options_.end();
	}

	std::string get_value(const std::string& name) const
	{
		auto it = options_.find(name);
		if (it != options_.end())
			return it->second;
		return std::string();
	}

	std::string get_name(size_t index) const
	{
		if (index < sequence_.size())
			return sequence_[index];
		return std::string();
	}

	std::string get_value(size_t index) const
	{
		if (index < sequence_.size())
			return get_value(sequence_[index]);
		return std::string();
	}

	bool parse(int argc, char** argv)
	{
		clear(false);
		for (int i = 1; i < argc; ++i)
		{
			bool is_valid = false;
			std::string arg = argv[i];
			// is this option allowed in the valid_options_
			for (const auto& option : valid_options_)
			{
				std::string pattern = option.first;
				const std::string& sep = option.second.first;
				const std::string& value = option.second.second;
				if (!value.empty())
				{
					if (!sep.empty())
						pattern += "[" + sep + "]";
					pattern += value;
				}
				std::regex  re{ pattern };
				std::smatch match;
				// if there is a space in the separator, add the next parameter
				if (sep.find(" ") != std::string::npos)
				{
					if ((i + 1) < argc)
					{
						std::string dblarg = arg + " " + argv[i + 1];
						if (std::regex_match(dblarg, match, re))
						{
							// found, add to options_
							if (!add_option(option.first, argv[i + 1]))
								return false;
							is_valid = true;
							++i;
							break;
						}
					}
				}
				if (std::regex_match(arg, match, re))
				{
					// found, add to options_
					std::string extracted_value = 
						arg.substr(
							option.first.length() + (sep.empty() ? 0 : 1),
							arg.length() - option.first.length() + (sep.empty() ? 0 : 1)
						);
					if (!add_option(option.first, extracted_value))
						return false;
					is_valid = true;
					break;
				}
			}
			if (!is_valid)
				return false;
		}

		return true;
	}
private:
	std::map < std::string, std::string > options_;
	std::map < std::string, std::pair<std::string, std::string> > valid_options_;
	std::vector<std::string> sequence_;
};

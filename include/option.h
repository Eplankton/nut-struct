#ifndef _NUTS_OPTION_
#define _NUTS_OPTION_ 1

/** @file Option
     *  Experimental
     *  For exception use
     */

#include "basic_string.h"

namespace nuts
{
	enum Option_case
	{
		Success,
		Panic,
		Error
	};

	template <typename T>
	struct Option
	{
		T elem;
		Option_case code;
		string info;

		Option() = default;
		~Option() = default;

		Option(const T& _x, Option_case _case = Success, const string& _text = "")
		    : elem(_x), code(_case), info(_text) {}

		Option<T>& print_info()
		{
			switch (code)
			{
				case Success:
					printf("\n[Success]: ");
					break;
				case Panic:
					printf("\n[Panic]: ");
					break;
				case Error:
					printf("\n[Error]: ");
					break;
			}
			std::cout << info << '\n';
			return *this;
		}

		string get_info() const
		{
			switch (code)
			{
				case Success:
					return "\n[Success]: " + info;
				case Panic:
					return "\n[Panic]: " + info;
				case Error:
					return "\n[Error]: " + info;
			}
		}

		template <class Helper>
		auto handle(Helper fn)
		{
			return fn();
		}
	};

	template <typename T>
	struct default_measure
	{
		nuts::pair<Option_case, string>
		operator()(const T& some) { return {Success, ""}; }
	};

	template <typename T, typename Detect = default_measure<T>>
	Option<T> make_option(const T& some,
	                      Detect hr = default_measure<T>())
	{
		auto tmp = hr(some);
		return {some, tmp.first, tmp.second};
	}
}

#endif

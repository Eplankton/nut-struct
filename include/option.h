#ifndef _NUTS_OPTION_
#define _NUTS_OPTION_

/** @file Option
     *  Experimental
     *  For exception use
     */

#include "concept.h"
#include "utility.h"

namespace nuts
{
	enum Option_case
	{
		Success,
		Panic,
		Error
	};

	template <typename T>
	class Option
	{
		T elem;
		Option_case code;

	public:
		Option() = default;
		~Option() = default;

		Option(const T& _x, Option_case _case = Success)
		    : elem(_x), code(_case) {}

		decltype(auto) unwarp() { return elem; }

		decltype(auto) handle(nuts::Invocable auto&& fn)
		{
			return fn();
		}
	};

	// ## How-to-use?

	// auto detect_scheme = [&m](const auto& it) {
	// 				if (it == m.npos)
	// 	        		return make_pair(Panic, (string)"Get npos iterator!");
	// 	            else
	// 					return make_pair(Success, (string)"Mooooo!"); };

	// auto opt = make_option(m.find(555), detect_scheme);

	// auto handle_scheme = [&opt, &m] {
	// 				if (opt.elem == m.npos) m.insert(555, 0);
	// 				else m[555] = 1;
	// 					return opt; };
	// opt.handle(handle_scheme);
}

#endif

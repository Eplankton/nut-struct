#ifndef _NUTS_FUNC_
#define _NUTS_FUNC_

namespace nuts
{
	template <typename FuncType>
	class function
	{
	protected:
		FuncType *(fn) = nullptr;

	public:
		function() = default;
		function(const FuncType &R) : fn(R) {}
		function(const function<FuncType> &OBJ) : fn(OBJ.fn) {}
		~function() { fn = nullptr; }

		template <typename... _ArgTypes>
		auto operator()(_ArgTypes... args)
		{
			return fn(args...);
		}
	};
}

#endif

#include <chrono>// for std::chrono functions
#include <iostream>

class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_type = std::chrono::steady_clock;
	using second_type = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_type> m_beg {clock_type::now()};

public:
	void reset()
	{
		m_beg = clock_type::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_type>(clock_type::now() - m_beg).count();
	}

	unsigned long long elapsed_ms() const
	{
		return std::chrono::milliseconds((clock_type::now() - m_beg).count()).count();
	}

	double print()
	{
		auto tmd = elapsed();
		std::cout << "\nTime count: " << tmd * 1000.0 << "(ms)\n";
		reset();
		return tmd * 1000.0;
	}
};
#pragma once

#include <iostream>

constexpr int DIV_BY_ZERO(0);
constexpr int NONPRIME_MODULUS(1);

bool is_prime (const unsigned int n)
{
	if (n <= 1) return false;

	for(int i(2); i < n-1; ++i){
		if(n % i == 0) return false;
	}

	return true;
}

template<int p> class Z
{
	private:
		int representative; // representative of the instance in {0,...,p-1}

	public:
		Z<p>(int x) : representative(x < 0 ? p + (x % p) : x % p)
		{
			if (not is_prime(p)) throw NONPRIME_MODULUS; // prevents instanciation if p is not prime
		}

		Z<p> inverse(void) const
		{
			// returns the multiplicative inverse of instance in Z_p if instance is nonzero
			// throws an error otherwise
			// basically uses Euclid's algorithm + Bezout's identity
			
			if (representative == 0) throw DIV_BY_ZERO;

			int a(representative);
			int p_0(p);

			int y(0);
			int x(1);

			while (a > 1)
			{
				int q(a / p_0);
				int r(p_0);

				p_0 = a % p_0;
				a = r;
				r = y;

				y = x - q*y;
				x = r;
			}
			return Z<p>(x);
		}


		void print(std::ostream& output = std::cout) const
		{
			output << representative << " (mod " << p << ") ";
		}

		Z<p> operator+(const Z<p> x)
		{
			return Z<p>(representative + x.representative);
		}

		Z<p> operator-(const Z<p> x)
		{
			return Z<p>(representative - x.representative);
		}

		Z<p> operator*(const Z<p> x)
		{
			return Z<p>(representative * x.representative);
		}

		Z<p> operator/(const Z<p> x)
		{
			return (*this) * x.inverse();
		}
};


/* Example of usage:

int main(void)
{
	constexpr int p(29);

	Z<p> a(18);
	Z<p> b(7);

	(a+b).print();
	std::cout << "\n";
	(a-b).print();
	std::cout << "\n";
	(a*b).print();
	std::cout << "\n";
	(a/b).print();
}

*/

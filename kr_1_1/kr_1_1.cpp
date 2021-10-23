#include <iostream>

int main()
{
	std::cout.precision(30);
	double result = 0.5;
	double sn = 0.5;
	for (int n = 2; n < 31; n++) {
		sn *= (-1) * 0.25 / n * (2 * n - 3) / (2 * n - 1);
		result += sn;
		
		
		if (n > 20)
			std::cout << result<< "\n";
	}

}



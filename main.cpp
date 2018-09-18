#include <iostream>
#include <vector>
#include <string>

// Greatest common divisor
int gcd(int a, int b) {
	int number = 0;
	int min = a < b ? a : b;

	for (int i = 1; i <= min; i++)
		if (a % i == 0 && b % i == 0)
			number = i;

	return number;
}

std::vector<int> eulersTotient(int n) {
	std::vector<int> numbers;

	for (int i = 1; i <= n; i++)
		if (gcd(n, i) == 1)
			numbers.push_back(i);

	return numbers;
}

int* dodecagonalNumbers(int amount) {
	int* numbers = new int[amount];

	// Dodecagonal number rule:
	// 5n^2 - 4n; n > 0
	for (int i = 1; i <= amount; i++)
		numbers[i - 1] = 5 * i * i - 4 * i;

	return numbers;
}

int* centeredTriangularNumbers(int amount) {
	int* numbers = new int[amount];

	// Centered triangular number rule:
	// (3n^2 + 3n + 2) / 2
	for (int i = 0; i < amount; i++)
		numbers[i] = (3 * i * i + 3 * i + 2) / 2;

	return numbers;
}

int* selfNumbers(int amount) {
	int* numbers = new int[amount];
	int index = 0, i = 1;

	while (index < amount) {
		bool valid = true;
		for (int j = 0; j < i; j++) {
			int sum = j;
			std::string digits = std::to_string(j);
			for (int k = 0; k < digits.length(); k++)
				sum += digits[k] - 48;

			if (sum == i)
				valid = false;
		}

		if (valid) {
			numbers[index] = i;
			index++;
		}

		i++;
	}

	return numbers;
}

int main() {

	// 64 is the 10th Self number
	int amount = 11;
	int* numbers = selfNumbers(amount);

	std::cout << "Self numbers:\n" << numbers[0];
	for (int i = 1; i < amount; i++)
		std::cout << ", " << numbers[i];

	std::cin.get();

	return 0;
}
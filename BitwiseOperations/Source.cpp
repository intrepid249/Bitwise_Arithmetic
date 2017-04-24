#include <Windows.h>
#include <time.h>
#include <iostream>

int negate(int x);
int bitwiseAdd(int a, int b);
int bitwiseSubtract(int a, int b);
int bitwiseMultiply(int a, int b);
int bitwiseDivide(int a, int b, int *remainder = 0);


int negate(int x) {
	return bitwiseAdd(~x, 1); // return the twos compliment of the negated bit-string
}

int bitwiseAdd(int a, int b) {
	int carry;
	while (b) {
		carry = a & b; // 1 + 1 = 10 -> carry bit is true when (a & b) == 0x1
		a = a ^ b; // use XOR to add the two numbers -> 0 + 1 = 1, 1 + 0 = 1
		b = carry << 1; // push the carry to the next column and store to b for next addition
	}
	return a;
}

int bitwiseSubtract(int a, int b) {
	return bitwiseAdd(a, negate(b));
}

int bitwiseMultiply(int a, int b) {
	int ans = 0;

	if (a < 0 || b < 0)
		a = negate(a), b = negate(b);

	while (b > 0) {
		if ((b & 0x1) == 0x1)
			// we're only concerned when the trailing bit of the multiplication is a 1
			// since anything x 0 is 0
			ans = bitwiseAdd(ans, a);

		a <<= 1;
		b >>= 1;
	}
	return ans;
}

int bitwiseDivide(int a, int b, int *remainder) {
	int ans = 0, sign = 0;

	if (a < 0) a = negate(a), sign ^= 1;
	if (b < 0) b = negate(b), sign ^= 1;

	if (b != 0) {
		while (a >= b)
			a = bitwiseSubtract(a, b), ++ans;
		*remainder = a;
	}


	if (sign) ans = negate(ans);

	return ans;
}



int main() {
	srand((unsigned int)time(NULL));

	int a = rand() % 100 - 100;
	int b = rand() % 100 - 100;
	int remainder = 0;

	int mult = bitwiseMultiply(a, b);
	int div = bitwiseDivide(a, b, &remainder);

	// Multiplication unit test
	if (mult == (a * b))
		std::cout << "TEST PASSED: Multiplication\t" << mult << "\n";
	else
		std::cout << "TEST FAILED: Multiplication\n";

	// Division unit test
	if (div == (a / b))
		std::cout << "TEST PASSED: Division\t" << a << " / " << b << " = " << div << " remainder " << remainder << "\n";
	else
		std::cout << "TEST FAILED: Division\n";


	system("pause");
}
#include <atomic>
#include <iostream>

#include "palindromic.h"

extern std::atomic<bool> interrupted;

/**
 * @brief Reverse a number
 * 
 * @param num The number
 * @param base The base to reverse in
 * @return The reversed number
 */
checked_cpp_int reverse(checked_cpp_int num, const unsigned int base) {
	checked_cpp_int result = 0;

	while (num != 0) {
		result = (result * base) + (num % base);
		num /= base;
	}

	return result;
}

/**
 * @brief Test whether a number is palindromic
 * 
 * @param num The number to be tested
 * @return True if the number is palindromic, false otherwise
 */
bool is_palindromic(const checked_cpp_int &num) {
	return num >= 0 && (num == 0 || num == reverse(num));
}

/**
 * @brief Count the number of steps to reach a palindromic sum for a number
 * 
 * @param num The number
 * @param max_steps The maximum number of steps to try
 * @return The number of steps, or 0 if interrupted or no palindromic sum found after max_steps steps
 */
unsigned int count_steps_to_palindromic_sum(checked_cpp_int num, unsigned int max_steps) {
	unsigned int step = 1;
	while (step <= max_steps) {
	    checked_cpp_int sum = num + reverse(num);
	    if (is_palindromic(sum)) return step;
		if (interrupted) break;
	    num = sum;
	    step++;
	}
	return 0;
}

/**
 * @brief Print out the steps to a palindromic sum for a number
 * 
 * @param num The number
 */
void print_steps_to_palindromic_sum(checked_cpp_int num) {
	unsigned int step = 1;
	while (true) {
	    checked_cpp_int rev_num = reverse(num);
	    checked_cpp_int sum = num + rev_num;
	    std::cout << step << ": " << num << " + " << rev_num << " = " << sum << std::endl;
	    if (is_palindromic(sum)) break;
	    num = sum;
	    step++;
	}
}

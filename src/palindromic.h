#pragma once

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::checked_cpp_int;

checked_cpp_int reverse(checked_cpp_int num, const unsigned int base = 10);
bool is_palindromic(const checked_cpp_int &num);
unsigned int count_steps_to_palindromic_sum(checked_cpp_int num, unsigned int max_steps);
void print_steps_to_palindromic_sum(checked_cpp_int num);

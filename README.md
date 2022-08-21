# Palindromic Sums

This is a command line tool that searches a range of numbers to find the one
requiring the most steps to reach a palindromic sum. 

A palindromic sum is found by taking the reverse of a number, adding it to the
original number, and seeing if the result is a palindrome (the same backwards
as it is forwards). If the result is not a palindrome the process is repeated
with the sum, until a palindrome is found.

The number 15, for example, requires only one step, since `15 + 51 = 66`, which
is a palindrome.

The number 19, however, requires two steps, since `19 + 91 = 110`, which is not
a palindrome, but `110 + 11 = 121`, which is.

[Boost][boost] [Multiprecision][multiprecision] is used for large number
support. 

C++11 or later is required for [thread][threads] and 
[lambda expression][lambdas] support.

[Catch2][catch2] is used for unit testing.

[boost]: https://www.boost.org/
[multiprecision]: https://www.boost.org/doc/libs/1_80_0/libs/multiprecision/doc/html/index.html
[threads]: https://en.cppreference.com/w/cpp/thread
[lambdas]: https://en.cppreference.com/w/cpp/language/lambda
[catch2]: https://github.com/catchorg/Catch2

# Usage

```
palin [-t n-threads] start end step-limit
```

Where `start` is the first number in the range to be searched, `end` is the
last, and `step-limit` is the maximum number of steps to try before giving
up. 

If `start` and `end` are the same number, `step-limit` will be ignored, and
the program will print out the steps for the number continually until a
palindromic sum is found, or the program is terminated.

The optional argument `-t` can be given, followed by a number indicating how
many threads should be used. If this argument is not given, the program will
attempt to detect how many threads are available and will use all of them.

The arguments `-t 8 100 1000 500`, for example, will produce the following
output:

```
Using 8 threads
100: 1 step
109: 2 steps
156: 3 steps
166: 5 steps
167: 11 steps
177: 15 steps
187: 23 steps
Gave up after 500 steps for 13 out of 901 numbers tested
Largest number of steps found: 23 for the number: 187
1: 187 + 781 = 968
2: 968 + 869 = 1837
3: 1837 + 7381 = 9218
4: 9218 + 8129 = 17347
5: 17347 + 74371 = 91718
6: 91718 + 81719 = 173437
7: 173437 + 734371 = 907808
8: 907808 + 808709 = 1716517
9: 1716517 + 7156171 = 8872688
10: 8872688 + 8862788 = 17735476
11: 17735476 + 67453771 = 85189247
12: 85189247 + 74298158 = 159487405
13: 159487405 + 504784951 = 664272356
14: 664272356 + 653272466 = 1317544822
15: 1317544822 + 2284457131 = 3602001953
16: 3602001953 + 3591002063 = 7193004016
17: 7193004016 + 6104003917 = 13297007933
18: 13297007933 + 33970079231 = 47267087164
19: 47267087164 + 46178076274 = 93445163438
20: 93445163438 + 83436154439 = 176881317877
21: 176881317877 + 778713188671 = 955594506548
22: 955594506548 + 845605495559 = 1801200002107
23: 1801200002107 + 7012000021081 = 8813200023188
```

# License

**Palindromic Sums Copyright © 2022 Steven Saunders**

```
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```

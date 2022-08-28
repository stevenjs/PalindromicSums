#include <atomic>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "palindromic.h"

std::atomic<bool> interrupted {false};

void sigint_handler(int) {
    interrupted = true;
}

struct ThreadSharedData {
    std::mutex queue_mutex;
    std::mutex largest_mutex;
    std::mutex aborted_mutex;
    checked_cpp_int current;
    checked_cpp_int max;
    checked_cpp_int largest;
    checked_cpp_int tested;
    checked_cpp_int aborted;
    unsigned int most_steps;
    unsigned int max_steps;

    ThreadSharedData() : queue_mutex(), largest_mutex(), aborted_mutex(), current(10), max(100000), largest(0), 
        tested(0), aborted(0), most_steps(0), max_steps(100) { }
};

/**
 * @brief Execute function f exclusive of other threads based on a mutex
 * 
 * @param mutex The mutex
 * @param f The function to execute
 */
void run_exclusively(std::mutex &mutex, std::function<void()> f) {
    const std::lock_guard<std::mutex> lock(mutex);
    f();
}

void thread_worker(ThreadSharedData &shared) {
    checked_cpp_int num;

    while (true) {
        // Get the next number to test
        run_exclusively(shared.queue_mutex, [&] {
            num = shared.current;
            shared.current++;
        });
        if (num > shared.max) break;

        // Count the number of steps to a palindromic sum
        const unsigned int steps = count_steps_to_palindromic_sum(num, shared.max_steps);
        if (interrupted) break;

        if (steps == 0) {
            run_exclusively(shared.aborted_mutex, [&] {
                shared.aborted += 1;
            });
        }

        // If the number of steps is larger than any seen so far print the number and steps
        run_exclusively(shared.largest_mutex, [&, steps] {
            shared.tested += 1;
            if (steps > shared.most_steps) {
                shared.largest = num;
                shared.most_steps = steps;
                std::cout << num << ": " << steps << " step" << ((steps > 1) ? "s" : "") << std::endl;
            }
        });
    }
}

void usage(std::ostream &out, char *command) {
    out << "Usage: " << command << " [-t n-threads] start end [max-steps]" << std::endl;
}

void validate_n_threads(unsigned long n_threads, unsigned int hardware_threads, char *command, char *arg_str) {
    if (n_threads < 1 || n_threads > hardware_threads) {
        std::cerr << command << ": invalid '-t' argument '" << arg_str << "'" << " (minimum: 1, maximum: "
            << hardware_threads << ")" << std::endl;
        exit(1);
    }
}

void handle_positional_args(std::vector<int> &positional_args, char **argv, ThreadSharedData &shared) {
    if (positional_args.size() < 2 || positional_args.size() > 3) {
        usage(std::cerr, argv[0]);
        exit(1);
    }

    std::istringstream in(argv[positional_args[0]]);
    in >> shared.current;
    in.str(argv[positional_args[1]]);
    in.seekg(0);
    in >> shared.max;

    if (positional_args.size() == 3) {
        in.str(argv[positional_args[2]]);
        in.seekg(0);
        in >> shared.max_steps;
    }

    if (shared.current < 1 || shared.max < shared.current || shared.max_steps < 1) {
        std::cerr << argv[0] << ": error: start, end, and max-steps values must all exceed 0," << std::endl
            << "and end must match or exceed start" << std::endl;
        exit(1);
    }
}

void handle_args(int argc, char **argv, ThreadSharedData &shared, unsigned long &n_threads) {
    unsigned int hardware_threads = std::thread::hardware_concurrency();
    if (hardware_threads == 0) hardware_threads = 1;
    n_threads = hardware_threads;

    std::vector<int> positional_args;

    for (int i=1; i<argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'h':
                    usage(std::cout, argv[0]);
                    exit(0);
                    break;
                case 't':
                    if (i == (argc - 1)) {
                        std::cerr << argv[0] << ": option '-t' requires an argument" << std::endl;
                        exit(1);
                    }
                    n_threads = strtoul(argv[++i], nullptr, 0);
                    validate_n_threads(n_threads, hardware_threads, argv[0], argv[i]);
                    break;
            }

        }
        else positional_args.emplace_back(i);
    }

    handle_positional_args(positional_args, argv, shared);
}

int main(int argc, char **argv) {
    struct ThreadSharedData shared;
    unsigned long n_threads = 1;
    
    handle_args(argc, argv, shared, n_threads);

    if (shared.current == shared.max) {
        print_steps_to_palindromic_sum(shared.current);
        exit(0);
    }
    
    std::cout << "Using " << n_threads << " threads\n";

    std::signal(SIGINT, sigint_handler);

    std::vector<std::thread> thread_pool;
    for (unsigned long t = 0; t < n_threads; t++) {
        thread_pool.emplace_back([&] { thread_worker(shared); });
    }

    for (auto &thread : thread_pool) {
        thread.join();
    }

    if (interrupted) std::cout << "Interrupted!" << std::endl;

    if (shared.aborted > 0) {
        std::cout << "Gave up after " << shared.max_steps << " steps for " << shared.aborted << " out of " 
            << shared.tested << " number" << ((shared.tested > 1) ? "s" : "")  << " tested" << std::endl;
    }

    if (shared.largest != 0) {
        std::cout << "Largest number of steps found: " << shared.most_steps << " for the number: " << shared.largest
            << std::endl;
        print_steps_to_palindromic_sum(shared.largest);
    }
}

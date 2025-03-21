/*  finetest.h - A simple test framework library for C, written in C.
    Version 1

    MIT License

    Copyright (c) 2023 Sachin Hegde

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/


/*
    DESCRIPTION:

    finetest.h is a simple test framework library for C, written in C. It is a single header file that can be included in your C project to write and run tests. It provides a simple API to define test cases, run them, and print the results. It also provides assertion macros to check conditions in the test cases.

    FLAGS:

    FINE_NO_PRINT_TESTS 0 => Print test names and descriptions 1 => Do not print test names and descriptions
    FINE_NO_PRINT_ASSERTS 0 => Print assertion results 1 => Do not print assertion results
    FINE_NO_PRINT_FINAL 0 => Print final assertion results 1 => Do not print final assertion results
    FINE_DO_NOT_CONTINUE 0 => Continue running tests after an assertion failure 1 => Stop running tests after an assertion failure

    MACRO FUNCTIONS:

    FINE_INIT() - Initialize the test system
    FINE_TEST(test, name, description, pass, fail) - Define a test case
    FINE_RUN() - Run all tests
    FINE_ASSERT(condition) - Assertion macro
    FINE_ASSERT_FINAL(condition) - Final assertion macro (to print a message without failing the test)

*/

#ifndef FINE_TEST_H
#define FINE_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Test and Result Structures
typedef struct f_fTest {
    void (*test)();
    bool passed;
    const char *name;
    const char *description;
    const char *pass;
    const char *fail;
} f_fTest;

typedef struct f_fResult {
    int total;
    int passed;
    int failed;
} f_fResult;

// Global test variables
static f_fTest *tests = NULL;
static f_fResult *result = NULL;
static f_fTest *current_test = NULL;

// flags
#define FINE_NO_PRINT_TESTS 0
#define FINE_NO_PRINT_ASSERTS 0
#define FINE_NO_PRINT_FINAL 0
#define FINE_DO_NOT_CONTINUE 0

// Function prototype
void (*f_fdone)(f_fResult *result) = NULL;  // Function pointer

// Clean up and return result is how many tests failed
static int f_fine_done() {  // Change macro to function
    if (f_fdone) {
        f_fdone(result);
    }
    free(tests);
    int failed = result->failed;
    free(result);
    return failed;
}

// Initialize test system
#define FINE_INIT() \
    do { \
        tests = NULL; \
        result = (f_fResult *)malloc(sizeof(f_fResult)); \
        if (!result) exit(EXIT_FAILURE); \
        result->total = result->passed = result->failed = 0; \
    } while (0)

// Define a test case
#define FINE_TEST(test, name, description, pass, fail) \
    do { \
        f_fTest *temp = (f_fTest *)realloc(tests, sizeof(f_fTest) * (result->total + 1)); \
        if (!temp) exit(EXIT_FAILURE); \
        tests = temp; \
        tests[result->total] = (f_fTest){ test, true, name, description, pass, fail }; \
        result->total++; \
    } while (0)

// Run all tests
#define FINE_RUN() \
    do { \
        for (int i = 0; i < result->total; i++) { \
            current_test = &tests[i]; \
            current_test->passed = true; \
            \
            /* Print test name and description */ \
            if(!FINE_NO_PRINT_TESTS) {\
            printf("\n\033[1;34mTest: %s\n", tests[i].name); \
            if (tests[i].description) \
                printf("Description: %s\n", tests[i].description); \
            printf("\033[0m"); \
            }\
            \
            /* Run test function */ \
            tests[i].test(); \
            \
            /* Print test result */ \
            if(!FINE_NO_PRINT_TESTS) {\
                if (current_test->passed) { \
                    printf("\033[1;32m%s\033[0m\n", tests[i].pass); \
                } else { \
                    printf("\033[1;31m%s\033[0m\n", tests[i].fail); \
                } \
            } \
        } \
        current_test = NULL; \
    } while (0)


// Assertion macros
#define FINE_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            if(!FINE_NO_PRINT_ASSERTS) {\
            printf("\n\033[1;31mAssertion failed: %s\nFile: %s\nLine: %d\033[0m\n", \
                   #condition, __FILE__, __LINE__); \
            }\
            result->failed++; \
            current_test->passed = false; \
            if(FINE_DO_NOT_CONTINUE) { \
            printf("\033[1;31m%s\033[0m\n", current_test->fail); \
            printf("\033[1;31mStopping tests\033[0m\n"); \
            exit(f_fine_done()); \
            } \
            return; \
        } else { \
            if(!FINE_NO_PRINT_ASSERTS) { \
            printf("\033[1;32mAssertion passed: %s\033[0m\n", #condition); \
            } \
            result->passed++; \
        } \
    } while (0)

// Final assertion (does not fail test)
#define FINE_ASSERT_FINAL(condition) \
    do { \
        if (!(condition)) { \
            if(!FINE_NO_PRINT_FINAL) { \
            printf("\n\033[1;31mFinal assertion failed: %s\nFile: %s\nLine: %d\033[0m\n", \
                   #condition, __FILE__, __LINE__); \
            } \
        } \
    } while (0)

#endif // FINE_TEST_H

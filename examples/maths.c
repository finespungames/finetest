#include "../finetest.h"

#define FINE_NO_PRINT_ASSERTS 1 // dont print asserts
#define FINE_DO_NOT_CONTINUE 1 // stop on first fail

void test_add() {
    int a = 1;
    int b = 2;
    int c = a + b;
    FINE_ASSERT(c == 1);
}

void test_sub() {
    int a = 1;
    int b = 2;
    int c = a - b;
    FINE_ASSERT(c == -1);
}

void test_mul() {
    int a = 1;
    int b = 2;
    int c = a * b;
    FINE_ASSERT(c == 2);
}

void test_div() {
    int a = 1;
    int b = 2;
    int c = a / b;
    FINE_ASSERT(c == 0);
}


void onResult(f_fResult *result) { // called before returning from f_fine_done
    printf("Total: %d\n", result->total);
    printf("Passed: %d\n", result->passed);
    printf("Failed: %d\n", result->failed);
}


int main() {
    f_fdone = onResult; // set fdone callback, print final results, free memory
    FINE_INIT();
    FINE_TEST(test_add, "Addition", "Test addition", "Pass", "Fail");
    FINE_TEST(test_sub, "Subtraction", "Test subtraction", "Pass", "Fail");
    FINE_TEST(test_mul, "Multiplication", "Test multiplication", "Pass", "Fail");
    FINE_TEST(test_div, "Division", "Test division", "Pass", "Fail");
    FINE_RUN();
    return f_fine_done(); // get return value from fine_done (also free memory)
}
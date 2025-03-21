#include "../finetest.h"

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


void onResult(fResult *result) {
    printf("Total: %d\n", result->total);
    printf("Passed: %d\n", result->passed);
    printf("Failed: %d\n", result->failed);
}


int main() {
    fdone = onResult;
    FINE_INIT();
    FINE_TEST(test_add, "Addition", "Test addition", "Pass", "Fail");
    FINE_TEST(test_sub, "Subtraction", "Test subtraction", "Pass", "Fail");
    FINE_TEST(test_mul, "Multiplication", "Test multiplication", "Pass", "Fail");
    FINE_TEST(test_div, "Division", "Test division", "Pass", "Fail");
    FINE_RUN();
    return fine_done();
}
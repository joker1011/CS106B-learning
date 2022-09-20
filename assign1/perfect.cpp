/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function takes one argument `n` and calculates
 * the sum of all proper divisors of `n` excluding itself.
 * To find divisors a loop iterates from 1 to sqrt(n),
 * testing for a zero remainder from the division.
 */
long smarterSum(long n) {
    long total = 1;
    long divisorSmall;
    for (divisorSmall = 2; divisorSmall < sqrt(n); divisorSmall++) {
        if (n % divisorSmall == 0) {
            long  divisorBig = n / divisorSmall;
            total += (divisorSmall + divisorBig);
        }
    }
    //if sqrt(n) is divisor, add it to the total
    if (n % divisorSmall == 0){
        total += divisorSmall;
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */

long findNthPerfectEuclid(long n) {
    long k = 1, m = 0 , perfectNum = 0 , count = 0;
    while(count != n){
        m = pow(2, k) - 1;
        if(smarterSum(m) == 1 && m != 1){
            perfectNum = pow(2, k - 1) * (pow(2, k) - 1);
            cout << "Found perfect number: " << perfectNum << endl;
            count++;
        }
        k++;
    }
    cout << endl << "Done searching up to " << n << endl;
    return perfectNum;
}

void findNthPerfectEuclidTest()
{
    for(int i = 1; i < 1000; i++){
        findNthPerfectEuclid(5);
    }
}

/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here
//STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
//    TIME_OPERATION(15250, findPerfects(15250));
//    TIME_OPERATION(30500, findPerfects(30500));
//    TIME_OPERATION(61000, findPerfects(61000));
//    TIME_OPERATION(122000, findPerfects(122000));
//}
//STUDENT_TEST("Time of isPerfect") {
//    TIME_OPERATION(10, isPerfect(10));
//    TIME_OPERATION(100000, isPerfect(1000000));
//}
//STUDENT_TEST("Time of divisorSum") {
//    TIME_OPERATION(10, divisorSum(10));
//    TIME_OPERATION(100000, divisorSum(1000000));
//}
//STUDENT_TEST("Time of findPerfect") {
//    TIME_OPERATION(500, findPerfects(500));
//    TIME_OPERATION(1500, findPerfects(1500));
//}
//STUDENT_TEST("isPerfect") {
//    EXPECT(isPerfect(-11));
//    EXPECT(isPerfect(-1223));
//    EXPECT(isPerfect(-1123213));
//}
//STUDENT_TEST("divisorSum vs smarterSum") {
//    EXPECT_EQUAL(divisorSum(25), smarterSum(25));
//    EXPECT_EQUAL(divisorSum(100), smarterSum(100));
//    EXPECT_EQUAL(divisorSum(1351), smarterSum(1351));
//}
//STUDENT_TEST("Time trials of findPerfectsSmarter on doubling input sizes") {
//    TIME_OPERATION(460000, findPerfectsSmarter(460000));
//    TIME_OPERATION(920000, findPerfectsSmarter(920000));
//    TIME_OPERATION(1840000, findPerfectsSmarter(1840000));
//    TIME_OPERATION(3680000, findPerfectsSmarter(3680000));
//}
STUDENT_TEST("正常测试") {
      EXPECT(isPerfect(findNthPerfectEuclid(5)));
}
STUDENT_TEST("真实值测试") {
      EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
      EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}
STUDENT_TEST("时间测试 单次findNthPerfectEuclid(5)") {
      TIME_OPERATION(5, findNthPerfectEuclid(5));
}
STUDENT_TEST("压力测试 1000次findNthPerfectEuclid(5)") {
      TIME_OPERATION(1000, findNthPerfectEuclidTest());
}

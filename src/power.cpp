
using namespace std;

/**
 * Computes the power of a number recursively.
 * This is tail recursive.
 */ 
double powRec(double base, double exp, double accum=1) {
    if (exp == 0) return accum;
    return powRec(base, exp-1, accum * base);
}

/**
 * Computes the power of a number iteratively.
 */ 
double powIter(double base, double exp) {
    double res = 1;
    while (exp) { res *= base; --exp; }
    return res;
}


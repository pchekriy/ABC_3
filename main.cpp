// Вариант 30
#include <iostream>
#include <vector>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <cmath>

#include <iostream>
#include <vector>
#include <pthread.h>
#include <cmath>

using namespace std;

double f(double x) {
    return x;
}

double coarse_helper(double a, double b, double (f)(double)) {
    return 0.5 * (b - a) * (f(a) + f(b));
}

double fine_helper(double a, double b, double (f)(double)) {
    double c = (a + b) / 2.0;
    return 0.25 * (b - a) * (f(a) + 2 * f(c) + f(b));
}

double *a, *b, *c;
int count = 0, count1 = 0;
double tolerance = 0.1;
double fine, fine1;

void *func(void *param) {
    double coarse = coarse_helper(a[0], c[0], f);
    fine = fine_helper(a[0], c[0], f);
    c[0] /= 2.0;
    if ((fabs(coarse - fine) <= 3.0 * tolerance) && (count >= 1)) {
        return NULL;
    } else if (count >= 10000) {
        return NULL;
    } else {
        ++count;
        return func((void *) param);
    }
}

void *func1(void *param) {
    double coarse = coarse_helper(c[1], b[0], f);
    fine1 = fine_helper(c[1], b[0], f);
    c[1] /= 2.0;

    if ((fabs(coarse - fine1) <= 3.0 * tolerance) && (count1 >= 1)) {
        return NULL;
    } else if (count1 >= 10000) {
        return NULL;
    } else {
        ++count1;
        return func1((void *) param);
    }
}


int main() {
    a = new double[1];
    b = new double[1];
    c = new double[2];
    cout << "Write a(left border):";
    cin >> a[0];
    cout << "Write b(right border):";
    cin >> b[0];
    c[0] = (a[0] + b[0]) / 2.0;
    c[1] = (a[0] + b[0]) / 2.0;
    pthread_t mythread1, mythread2;
    int num[3];
    for (int i = 0; i < 3; i++) num[i] = i;
    pthread_create(&mythread1, NULL, func, (void *) (1));
    pthread_create(&mythread2, NULL, func1, (void *) (2));
    pthread_join(mythread1, NULL);
    pthread_join(mythread2, NULL);

    cout << fine + fine1;
    return 0;
}

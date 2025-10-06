#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "fraction_toolbox.hpp"

using namespace std;

// read command line arguments
static void readcmdline(fraction & frac, int argc, char* argv[])
{
    if (argc!=3)
    {
        printf("Usage: n d\n");
        printf("  n        numerator of fraction\n");
        printf("  d        denominator of fraction\n");
        exit(1);
    }

    // read n
    frac.num = atoi(argv[1]);

    // read d
    frac.denom = atoi(argv[2]);
}

static void test23467(int argc, char* argv[])
{
    //TODO: implement function
    fraction frac, temp_frac;
    readcmdline(frac, argc, argv);
    temp_frac = frac; // to keep the original fraction for inplace operations

    cout << "Initial fraction: "; 
    print_fraction(frac);

    cout << "2. square_fraction(fraction frac): "; // function 2
    print_fraction(square_fraction(frac));

    cout << "3. square_fraction_inplace(fraction& frac): "; // function 3
    square_fraction_inplace(temp_frac);
    print_fraction(frac);

    // reset temp_frac
    // temp_frac = frac;
    // cout << "Fraction is restored to original value: ";
    // print_fraction(temp_frac);

    cout << "4. fraction2double(fraction frac): "; // function 4
    cout << fraction2double(frac) << endl;

    cout << "6. gcd(fraction frac): "; // function 6
    cout << gcd(frac) << endl;

    cout << "7. reduce_fraction_inplace(fraction& frac): "; // function 7
    reduce_fraction_inplace(temp_frac);
    print_fraction(temp_frac);
}

static void test5()
{
    //TODO: implement function 
    int a, b;
    cout << "Enter two integers: ";
    cin >> a >> b;
    cout << "gcd(" << a << ", " << b << ") = " << gcd(a, b) << endl;
}

static void test_array_functions(int n)
{
    //TODO: implement function
    cout << "Provide integer n: ";
    cin >> n;
    fraction* frac_array = (fraction*) malloc(n * sizeof(fraction));
    fill_fraction_array(frac_array, n);
    print_fraction_array(frac_array, n);
    cout << "Approximate sum of array elements: " << sum_fraction_array_approx(frac_array, n) << endl;
    cout << "Exact sum of array elements: "; 
    print_fraction(sum_fraction_array(frac_array, n));
    free(frac_array);

    //TODO: find n for which sum function breaks. Explain what is happening.
    /*
        It depends on the implementation of the sum_fraction_array function, let's disguish two cases:

        === Case 1 (with reduction) ===
        In my implementation of the sum_fraction_array I reused the add_fractions previously required, the latter was by assignment reducing the fraction after each summing.
        The denominator of the fraction struct is a 32-bit integer, so it can represent 2^32 values of which 2^31 - 1 are positive and the max value is therefore 2^31 - 1 = 2_147_483_647.

        Doing some printing with terminal we see in the case of overflow:
            ~ Exact sum of array elements: Warning: denominator overflow during sum at index 1290
            ~ Sum reached value: 1289/1290
            ~ Current fraction: 1/1665390
        
        We can see that the first function to crash is the one adding fractions, not the one generating them.
        Also we can notice that the current sum's denominator (1290) and the new fraction’s denominator (1665390) SUMMED wont actually reach the max int value, but the resulting denominator is computed by temporarily multiplying (1290 * 1665390 = 2_149_058_100) which will indeed overflow the int type.

        === Case 2 (no reduction) ===
        If we consider the case that we don't reduce the fraction after each summing, the overflow happens much sooner, at n = 8 because the denominator grows exponentially.
        

        Step    Current Denominator    Multiplier    New Denominator
        1       1                      2             2
        2       2                      6             12
        3       12                     12            144
        4       144                    20            2,880
        5       2,880                  30            86,400
        6       86,400                 42            3,628,800
        7       3,628,800              56            203,212,800
        8       203,212,800            72            14,631,321,600 → overflow

        Therefore the first overflow happens at n = 8.
    */
}

static void test_toolbox(int argc, char* argv[])
{
    cout << "\n===============  test23467  =============== " << endl;
    test23467(argc, argv);

    cout << "\n=================  test5  ================= " << endl;
    test5();

    cout << "\n==========  test_array_functions  ========= " << endl;
    int n = 1290; // overflow happens first at 1290
    test_array_functions(n);
}

int main(int argc, char* argv[])
{
    test_toolbox(argc, argv);
}
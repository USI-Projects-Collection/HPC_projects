#include <iostream>

#include "fraction_toolbox.hpp"

void print_fraction(fraction frac)
{
    std::cout << frac.num << '/' << frac.denom << std::endl;
}

void print_fraction_array(fraction frac_array[], int n)
{
    std::cout << "[ " << frac_array[0].num << '/' << frac_array[0].denom << std::endl;
    for (int i = 1; i < n-1; i++)
    {
        std::cout << "  ";
        print_fraction(frac_array[i]);
    }
    std::cout << "  " << frac_array[n-1].num << '/' << frac_array[n-1].denom << " ]" << std::endl;
}

fraction square_fraction(fraction frac)
{
    //TODO: implement function 2
    frac.num *= frac.num;
    frac.denom *= frac.denom;
    return frac;
}

//TODO: implement function 3
void square_fraction_inplace(fraction& frac) {
    frac.num *= frac.num;
    frac.denom *= frac.denom;
}

double fraction2double(fraction frac)
{
    //TODO: implement function 4
    return (double) frac.num / frac.denom;
}

int gcd(int a, int b)
{
    //TODO: implement function 5
    return b == 0 ? a : gcd(b, a % b);
}

//TODO: implement function 6
int gcd(fraction frac) {
    int t;
    while (frac.denom != 0) {
        t = frac.denom;
        frac.denom = frac.num % frac.denom;
        frac.num = t;
    }
    return frac.num;
}

void reduce_fraction_inplace(fraction& frac)
{
    //TODO: implement function 7
    int divisor = gcd(frac);
    frac.num /= divisor;
    frac.denom /= divisor;

    //TODO: add short comment to explain which of the gcd() functions your code is calling
    /*
        This code is using the gcd(fraction frac) function defined at line 49, I know this because it is passing a fraction as an argument.
    */
}

fraction add_fractions(fraction frac1, fraction frac2)
{
    //TODO: implement function 8
    fraction result;
    result.num = frac1.num * frac2.denom + frac2.num * frac1.denom;
    result.denom = frac1.denom * frac2.denom;
    reduce_fraction_inplace(result);
    return result;
}

double sum_fraction_array_approx(fraction frac_array[], int n)
{
    //TODO: implement function 9
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += fraction2double(frac_array[i]);
    }
    return sum;
    
    //TODO: add short comment to explain why this function is approximate
    /*
        This function is approximate because fraction2double converts each fraction to a double for summation.
        
        I can also add the value of the nominators and denominators and only at the end compute the division, this way I would lose less precision but it might overflow for int type.
    */
}

//TODO: implement function 10
fraction sum_fraction_array(fraction frac_array[], int n) {
    fraction result = frac_array[0];
    for (int i = 1; i < n; i++) {
        fraction prev = result;
        result = add_fractions(result, frac_array[i]);

        if (result.denom < 0) {
            std::cout << "Warning: denominator overflow during sum at index " << i + 1 << std::endl;
            std::cout << "Sum reached value: ";
            print_fraction(prev);
            std::cout << "Current fraction: ";
            print_fraction(frac_array[i]);
            exit(1);
        }
    }
    return result;
}

void fill_fraction_array(fraction frac_array[], int n)
{
    fraction temp_frac;
    temp_frac.num = 1;
    for (int i = 1; i <= n; i++)
    {
        temp_frac.denom = i * (i+1);
        frac_array[i-1] = temp_frac;
        if (temp_frac.denom < 0) {
            std::cout << "Warning: denominator overflow at index " << i-1 << std::endl;
            exit(1);
        }
    }
}
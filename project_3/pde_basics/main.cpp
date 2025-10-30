/*
 * HPC Assignment: Heat Equation Play Code (Explicit Matrix-Free Scheme)
 * Author: Aryan Eftekhari <aryan.eftkehari@gmail.com>
 *
 * Description:
 * This program solves the 2D heat equation ∂s/∂t = αΔs using an explicit matrix-free scheme.
 * The grid is updated iteratively with a finite difference method, and the results are displayed
 * as an ASCII representation of the temperature distribution.
 *
 * Compilation Instructions:
 * To compile the program, use the following command:
 * g++ -o main.exe main.cpp
 *
 * Run Instructions:
 * To run the program:
 * $ ./heat_equation_solver
 *
 *
 * TODO:
 * 1) Analyze the effect of increasing the time step `tau`, `alpha` and `h` on the stability of the solution. 
 * See the Courant-Friedrichs-Lewy (CFL) condition for stability: https://people.math.ethz.ch/~grsam/SS21/NAII/resources/slides/ODE-Lecture6.pdf
 */

#include <iostream>
#include <vector>

// Grid and time parameters
int nx = 20;          // Number of spatial grid points in x direction
int ny = 20;          // Number of spatial grid points in y direction
double h = 0.01;      // Spatial step size in x and y direction
double tau = 0.0001;  // Time step size
double alpha = 0.01;  // Thermal diffusivity
int T = 500;          // Number of time steps

/**
 * @brief Initializes the solution grid with the initial condition (hotspot in the center) 
 *        and zero boundary conditions.
 * 
 * @param S Solution grid (2D vector).
 */
void initialize(std::vector<std::vector<double>>& S) {
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            S[i][j] = 0.0;
            
            // Initial condition: set a hot spot in the center
            if (i > 1 && i < 5 && j > 1 && j < 5) {
                S[i][j] = 1.0;
            }
        }
    }
}

/**
 * @brief Updates the solution grid using the explicit time-stepping method.
 * 
 * @param S Solution grid (2D vector).
 */
void update(std::vector<std::vector<double>>& S) {
    std::vector<std::vector<double>> S_new(nx, std::vector<double>(ny));

    for (int i = 1; i < nx - 1; ++i) {
        for (int j = 1; j < ny - 1; ++j) {
            double temp = (-4*S[i][j] + S[i+1][j] + S[i-1][j] + S[i][j+1] + S[i][j-1]) / (h*h);
            S_new[i][j] = S[i][j] + alpha * tau * temp;
        }
    }

    S = S_new;
}

/**
 * @brief Converts a value in the range [0,1] to a corresponding ASCII character for visualization.
 * 
 * @param val The value to be converted.
 * @return char ASCII character representing the value.
 */
char ascii(double val) {
    if (val > 0.99) return '@';  // Maximum intensity
    if (val > 0.8)  return '#';  // Very high intensity
    if (val > 0.7)  return '8';  // High intensity
    if (val > 0.6)  return '&';  // Moderately high intensity
    if (val > 0.5)  return 'o';  // Medium-high intensity
    if (val > 0.4)  return '=';  // Medium intensity
    if (val > 0.3)  return '+';  // Medium-low intensity
    if (val > 0.2)  return '-';  // Low intensity
    if (val > 0.1)  return ':';  // Very low intensity
    if (val > 1e-6) return '.';  // Barely noticeable
    if (val > 1e-12) return ' '; // Very close to zero (heat dissipated)
    return 'O';                  // Zero or below (cold zone, boundary)
}

/**
 * @brief Prints the solution grid either as numerical values or ASCII characters.
 * 
 * @param S Solution grid (2D vector).
 * @param print_ascii If true, prints ASCII representation; otherwise, prints numerical values.
 */
void print(const std::vector<std::vector<double>>& S, bool print_ascii = true) {
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            if (print_ascii) {
                std::cout << ascii(S[i][j]) << " ";
            } else {
                std::cout << S[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    // Initialize the solution grid
    std::vector<std::vector<double>> S(nx, std::vector<double>(ny));
    initialize(S);

    std::cout << "###################" << std::endl;
    std::cout << "# Initial Condition #" << std::endl;
    std::cout << "###################" << std::endl;
    print(S);
    std::cout << "###################" << std::endl;

    // Time-stepping loop
    for (int t = 1; t < T; ++t) {
        update(S);

        // Print the solution grid and clear the screen
        print(S);
        std::cout << "Time Step = " << t << std::endl;

        // Wait for user input to proceed
        std::cin.get();
        std::cout << "\033[H\033[2J\033[3J";  // Clear screen
    }

    return 0;
}

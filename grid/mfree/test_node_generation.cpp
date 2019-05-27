/**
 This is a test and will fill a `d`-dimensional sphere with variable density nodes.
 The program runs the 1, 2 and 3-dimensional example for a specified
 and saves the nodes in a file
 * @brief Illustrates the node placing algorithm.
 run like this
 g++ -o example example.cpp -std=c++11 -O3 -Wall -DNDEBUG
 */


#include <fstream>
#include <string>
#include <Eigen/Core>
#include "PNP.hpp"

template <int dim>
using Vector = Eigen::Matrix<double, dim, 1>;

template <int dim>
void example() {
    double r = 1.0;
    int n = 12;
    std::mt19937 gen(1337);

    auto domain = [=](const Vector<dim>& p) { return p.squaredNorm() < r*r; };
    auto radius = [](const Vector<dim>& p) { return 0.025*std::pow(1+std::abs(1-p[0]), 1.5); };

    Vector<dim> start; start.setConstant(0.5);
    auto pts = pnp<double, dim>(domain, radius, {start}, n, 1000000, gen);

    int N = pts.size();
    std::cout << "N = " << N << std::endl;
    std::string filename = "example";
    filename.push_back('0'+dim);
    filename += ".txt";
    std::ofstream file(filename);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < dim; ++j)
            file << pts[i][j] << " ";
        file << "\n";
    }
}

int main() {
    // 1D, 2D and 3D examples
    example<1>();
    example<2>();
    example<3>();

    // takes about 1 min for ~470 000 nodes.
    // example<4>();

    return 0;
}

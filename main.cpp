#include <iostream>

#include "nn.h"

int main() {
    int input_size = 2;
    std::vector<int> nodes_per_hidden_layer = {
        {2}
    };

    NeuralNetwork nn = NeuralNetwork(input_size, nodes_per_hidden_layer);

    std::vector<std::vector>> X = {
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4}
    };

    std::vector<float> Y = {
        1,
        2,
        3,
        4
    };

    std::vector<float> x = {1, 1};

    float test_output = nn.predict(x);
    std::cout << test_output << std::endl;
}

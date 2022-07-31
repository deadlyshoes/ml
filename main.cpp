#include <iostream>

#include "nn.h"

int main() {
    int input_size = 2;
    std::vector<int> nodes_per_hidden_layer = {
        {2}
    };
    float learning_rate = 0.1;

    NeuralNetwork nn = NeuralNetwork(input_size, nodes_per_hidden_layer, 0.1);

    std::vector<std::vector<float>> X = {
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
    std::vector<float> y = {2, 2};

    std::cout << "Before training" << std::endl;
    float test_output = nn.predict(x);
    std::cout << "{1, 1}: " << test_output << std::endl;
    test_output = nn.predict(y);
    std::cout << "{2, 2}: " << test_output << std::endl;
    
    nn.train(X, Y);

    std::cout << "After training" << std::endl;
    test_output = nn.predict(x);
    std::cout << "{1, 1}: " << test_output << std::endl;
    test_output = nn.predict(y);
    std::cout << "{2, 2}: " << test_output << std::endl;
}

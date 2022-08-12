#include <vector>
#include <iostream>

#pragma once

struct Node {
    std::vector<float> w;
    std::vector<float> error_gradient;
    float output;
    float delta;
};

struct Layer {
    std::vector<Node> nodes;
};

class NeuralNetwork {
public:
    NeuralNetwork(int input_size, int output_size, std::vector<int> nodes_per_hidden_layer, float learning_rate);
    ~NeuralNetwork();

    void train(std::vector<std::vector<float>> X, std::vector<std::vector<float>> Y);
    std::vector<float> predict(std::vector<float> x);

    std::vector<Layer> get_layers();
private:
    int input_size;
    int output_size;
    std::vector<int> nodes_per_hidden_layer;
    std::vector<Layer> layers;
    float learning_rate;

    void backpropagate();
    void calculate_gradient();
    void update_w();
    std::vector<float> output_layer_to_vector();
};

#include <vector>
#include <iostream>

class NeuralNetwork {
public:
    NeuralNetwork(int input_size, std::vector<int> nodes_per_hidden_layer);
    ~NeuralNetwork();

    void train(std::vector<std::vector<float>> X, std::vector<float> Y);
    float predict(std::vector<float> x);
private:
    struct Node {
        std::vector<float> w;
        float output;
    };

    struct Layer {
        std::vector<Node> nodes;
    };

    int input_size;
    std::vector<int> nodes_per_hidden_layer;
    std::vector<Layer> layers;
};

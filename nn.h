#include <vector>
#include <iostream>

class NeuralNetwork {
public:
    NeuralNetwork(int input_size, std::vector<int> nodes_per_hidden_layer, float learning_rate);
    ~NeuralNetwork();

    void train(std::vector<std::vector<float>> X, std::vector<float> Y);
    float predict(std::vector<float> x);
private:
    struct Node {
        std::vector<float> w;
        std::vector<float> error_gradient;
        float output;
        float delta;
    };

    struct Layer {
        std::vector<Node> nodes;
    };

    int input_size;
    std::vector<int> nodes_per_hidden_layer;
    std::vector<Layer> layers;
    float learning_rate;

    void backpropagate();
    void calculate_gradient();
    void update_w();
};

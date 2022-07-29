#include "nn.h"

NeuralNetwork::NeuralNetwork(int input_size, std::vector<int> nodes_per_hidden_layer) {
    this->input_size = input_size;
    this->nodes_per_hidden_layer = nodes_per_hidden_layer;

    // add output as a hidden layer with one node
    nodes_per_hidden_layer.push_back(1);

    // input layer
    Layer input_layer;
    for (int i = 0; i < input_size; i++) {
        Node node;
        input_layer.nodes.push_back(node);
    }
    layers.push_back(input_layer);
    // hidden layers
    for (int nodes : nodes_per_hidden_layer) {
        Layer hidden_layer;
        for (int i = 0; i < nodes; i++) {
            Node node;
            for (int j = 0; j < layers.back().nodes.size(); j++) {
                node.w.push_back(1);
            }
            hidden_layer.nodes.push_back(node);
        }
        layers.push_back(hidden_layer);
    }
}

NeuralNetwork::~NeuralNetwork() {

}

void NeuralNetwork::train(std::vector<std::vector<float> > X, std::vector<float> Y) {

}

float NeuralNetwork::predict(std::vector<float> x) {
    // pass input to input layer output
    for (int i = 0; i < layers[0].nodes.size(); i++) {
        layers[0].nodes[i].output = x[i];
    }

    // calculate hidden layers outputs
    for (int i = 1; i < layers.size(); i++) {
        for (Node &node : layers[i].nodes) {
            float output = 0;
            for (int j = 0; j < layers[i - 1].nodes.size(); j++) {
                Node old_node = layers[i - 1].nodes[j];
                std::cout << "check old layer output " << i << " " << j << " " << old_node.output << std::endl;
                output += node.w[j] * old_node.output;
            }
            std::cout << i << " " << output << std::endl;
            node.output = output;
        }
    }

    return layers.back().nodes.back().output;
}

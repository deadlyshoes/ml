#include "nn.h"
#include <math.h>

float h(Node* node, float a) {
    if (node->activation_function == "logistic_sigmoid") {
        return static_cast<float>(tanh(static_cast<double>(a)));
    }
    return a;
}

float dh(Node* node, float a) {
    if (node->activation_function == "logistic_sigmoid") {
        float tanh_a = static_cast<float>(tanh(static_cast<double>(a)));
        return 1 - tanh_a * tanh_a;
    }
    return 1;
}

NeuralNetwork::NeuralNetwork(int input_size, int output_size, std::vector<int> nodes_per_hidden_layer, std::vector<std::string> activations_per_hidden_layer, float learning_rate) {
    this->input_size = input_size;
    this->output_size = output_size;
    this->nodes_per_hidden_layer = nodes_per_hidden_layer;
    this->activations_per_hidden_layer = activations_per_hidden_layer;
    this->learning_rate = learning_rate;

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
                node.activation_function = activations_per_hidden_layer[layers.size() - 1];
                node.w.push_back(1);
                node.error_gradient.push_back(1);
            }
            hidden_layer.nodes.push_back(node);
        }
        layers.push_back(hidden_layer);
    }
    // output layer
    Layer output_layer;
    for (int i = 0; i < output_size; i++) {
        Node node;
        for (int j = 0; j < layers.back().nodes.size(); j++) {
            node.activation_function = "linear";
            node.w.push_back(1);
            node.error_gradient.push_back(1);
        }
        output_layer.nodes.push_back(node);
    }
    layers.push_back(output_layer);
}

NeuralNetwork::~NeuralNetwork() {

}

float mse(float a, float b) {
    return 0.5 * (a - b) * (a - b);
}

void NeuralNetwork::update_w() {
    for (int i = 1; i < this->layers.size(); i++) {
        for (Node& node : this->layers[i].nodes) {
            for (int j = 0; j < node.w.size(); j++) {
                node.w[j] -= this->learning_rate * node.error_gradient[j];
            }
        }
    }
}

void NeuralNetwork::calculate_gradient() {
    std::cout << "gradients:" << std::endl;
    for (int i = 1; i < this->layers.size(); i++) {
        for (Node &node : this->layers[i].nodes) {
            for (int j = 0; j < this->layers[i - 1].nodes.size(); j++) {
                Node old_node = this->layers[i - 1].nodes[j];
                node.error_gradient[j] = node.delta * old_node.output;
                std::cout << node.error_gradient[j] << std::endl;
            }
        }
    }
}

void NeuralNetwork::backpropagate() {
    for (int i = this->layers.size() - 2; i > 0; i--) {
        for (int j = 0; j < this->layers[i].nodes.size(); j++) {
            float delta = 0;
            for (int k = 0; k < this->layers[i + 1].nodes.size(); k++) {
                delta += this->layers[i + 1].nodes[k].delta * this->layers[i + 1].nodes[k].w[j];
            }
            delta *= dh(&this->layers[i].nodes[j], this->layers[i].nodes[j].output);
            this->layers[i].nodes[j].delta = delta;
            std::cout << i << " " << j << " delta: " << delta << std::endl;
        }
    }
}

void NeuralNetwork::train(std::vector<std::vector<float>> X, std::vector<std::vector<float>> Y) {
    std::vector<float> errors(X.size());

    int n = 10;
    while (n > 0) {
        std::cout << "N: " << n << std::endl;
        for (int i = 0; i < X.size(); i++) {
            std::cout << "X: " << i << std::endl;
            // calculate deltas
            std::vector<float> prediction = this->predict(X[i]);
            std::cout << Y[i][0] << " " << Y[i][1] << std::endl;
            std::cout << prediction[0] << " " << prediction[1] << std::endl;
            for (int j = 0; j < this->output_size; j++) {
                this->layers.back().nodes[j].delta = prediction[j] - Y[i][j];
                std::cout << j << " delta: " << prediction[j] - Y[i][j] << std::endl;
            }
            // backpropagate 
            this->backpropagate();
            // calculate error gradient
            this->calculate_gradient();
            // update w of each node
            this->update_w();
        }
        n--;
    }
    
}

std::vector<float> NeuralNetwork::output_layer_to_vector() {
    std::vector<float> output(this->output_size);
    for (int i = 0; i < this->output_size; i++) {
        output[i] = this->layers.back().nodes[i].output;
    }
    return output;
}

std::vector<float> NeuralNetwork::predict(std::vector<float> x) {
    // pass input to input layer output
    for (int i = 0; i < layers[0].nodes.size(); i++) {
        layers[0].nodes[i].output = x[i];
    }

    // calculate hidden layers and output layer outputs
    for (int i = 1; i < layers.size(); i++) {
        for (Node &node : layers[i].nodes) {
            float output = 0;
            for (int j = 0; j < layers[i - 1].nodes.size(); j++) {
                Node old_node = layers[i - 1].nodes[j];
                //std::cout << "check old layer output " << i << " " << j << " " << old_node.output << std::endl;
                output += node.w[j] * h(&node, old_node.output);
            }
            //std::cout << i << " " << output << std::endl;
            node.output = output;
        }
    }

    return this->output_layer_to_vector();
}

std::vector<Layer> NeuralNetwork::get_layers()
{
    return this->layers;
}

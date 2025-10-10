#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <numeric>
#include <limits>
#include <algorithm>
using namespace std;

class NeuralNetwork {
public:
    int input_nodes;
    int hidden_nodes;
    int output_nodes;

    // Weights and biases
    vector<vector<double>> weights_ih; // Input -> Hidden
    vector<vector<double>> weights_ho; // Hidden -> Output
    vector<double> bias_h;
    vector<double> bias_o;

    NeuralNetwork(int in, int hidden, int out)
        : input_nodes(in), hidden_nodes(hidden), output_nodes(out) {
        weights_ih.resize(hidden_nodes, vector<double>(input_nodes));
        weights_ho.resize(output_nodes, vector<double>(hidden_nodes));
        bias_h.resize(hidden_nodes);
        bias_o.resize(output_nodes);
    }

    // Sigmoid activation function
    static double sigmoid(double x) {
        return 1.0 / (1.0 + exp(-x));
    }

    // Feedforward operation
    vector<double> feedForward(vector<double> inputs) const {
        // Calculate hidden layer outputs
        vector<double> hidden(hidden_nodes);
        for (int i = 0; i < hidden_nodes; ++i) {
            double sum = 0.0;
            for (int j = 0; j < input_nodes; ++j) {
                sum += inputs[j] * weights_ih[i][j];
            }
            hidden[i] = sigmoid(sum + bias_h[i]);
        }

        // Calculate final output
        vector<double> outputs(output_nodes);
        for (int i = 0; i < output_nodes; ++i) {
            double sum = 0.0;
            for (int j = 0; j < hidden_nodes; ++j) {
                sum += hidden[j] * weights_ho[i][j];
            }
            outputs[i] = sigmoid(sum + bias_o[i]);
        }
        return outputs;
    }

    // Set network weights and biases from a flat vector (a particle's position)
    void setWeights(const vector<double>& flat_weights) {
        int idx = 0;
        for (int i = 0; i < hidden_nodes; ++i) {
            for (int j = 0; j < input_nodes; ++j) {
                weights_ih[i][j] = flat_weights[idx++];
            }
        }
        for (int i = 0; i < hidden_nodes; ++i) {
            bias_h[i] = flat_weights[idx++];
        }
        for (int i = 0; i < output_nodes; ++i) {
            for (int j = 0; j < hidden_nodes; ++j) {
                weights_ho[i][j] = flat_weights[idx++];
            }
        }
        for (int i = 0; i < output_nodes; ++i) {
            bias_o[i] = flat_weights[idx++];
        }
    }

    // Get the total number of parameters (weights + biases)
    int getTotalParams() const {
        return (input_nodes * hidden_nodes) + hidden_nodes + (hidden_nodes * output_nodes) + output_nodes;
    }
};

// Particle Struct for PSO
struct Particle {
    vector<double> position;
    vector<double> velocity;
    vector<double> best_position;
    double best_fitness;

    Particle(int num_dimensions) {
        position.resize(num_dimensions);
        velocity.resize(num_dimensions);
        best_position.resize(num_dimensions);
        best_fitness = numeric_limits<double>::max();
    }
};

class PSO {
public:
    int num_particles;
    int num_dimensions;
    int max_iterations;

    // PSO parameters
    double w;  // Inertia weight
    double c1; // Cognitive (personal) parameter
    double c2; // Social (global) parameter

    vector<Particle> swarm;
    vector<double> g_best_position;
    double g_best_fitness;

    const vector<vector<double>>& X_train;
    const vector<vector<double>>& y_train;
    NeuralNetwork& nn;

    mt19937 rng{random_device{}()};

    PSO(int particles, int iterations, NeuralNetwork& net, const vector<vector<double>>& X, const vector<vector<double>>& y)
        : num_particles(particles), max_iterations(iterations), nn(net), X_train(X), y_train(y) {
        
        num_dimensions = nn.getTotalParams();
        g_best_fitness = numeric_limits<double>::max();
        g_best_position.resize(num_dimensions);

        // Initialize PSO parameters
        w = 0.5;
        c1 = 1.5;
        c2 = 1.5;

        // Initialize the swarm
        uniform_real_distribution<double> dist(-1.0, 1.0);
        for (int i = 0; i < num_particles; ++i) {
            swarm.emplace_back(num_dimensions);
            for (int j = 0; j < num_dimensions; ++j) {
                swarm[i].position[j] = dist(rng);
                swarm[i].velocity[j] = dist(rng);
            }
        }
    }
    
    // Calculate fitness (Mean Squared Error) for a particle
    double calculate_fitness(const vector<double>& weights) {
        nn.setWeights(weights);
        double total_error = 0.0;
        for (size_t i = 0; i < X_train.size(); ++i) {
            vector<double> prediction = nn.feedForward(X_train[i]);
            for (size_t j = 0; j < prediction.size(); ++j) {
                total_error += pow(y_train[i][j] - prediction[j], 2);
            }
        }
        return total_error / X_train.size();
    }


    // Main training loop
    void train() {
        for (int iter = 0; iter < max_iterations; ++iter) {
            for (auto& p : swarm) {
                double current_fitness = calculate_fitness(p.position);

                // Update personal best
                if (current_fitness < p.best_fitness) {
                    p.best_fitness = current_fitness;
                    p.best_position = p.position;
                }

                // Update global best
                if (current_fitness < g_best_fitness) {
                    g_best_fitness = current_fitness;
                    g_best_position = p.position;
                }
            }

            // Update particle velocities and positions
            uniform_real_distribution<double> dist(0.0, 1.0);
            for (auto& p : swarm) {
                for (int i = 0; i < num_dimensions; ++i) {
                    double r1 = dist(rng);
                    double r2 = dist(rng);

                    double cognitive_component = c1 * r1 * (p.best_position[i] - p.position[i]);
                    double social_component = c2 * r2 * (g_best_position[i] - p.position[i]);
                    
                    p.velocity[i] = w * p.velocity[i] + cognitive_component + social_component;
                    p.position[i] += p.velocity[i];
                }
            }
            
            if ((iter + 1) % 10 == 0) {
                cout << "Iteration " << iter + 1 << ", Best Fitness (MSE): " << g_best_fitness << endl;
            }
        }
    }
};

int main() {
    // Define the Problem (XOR)
    vector<vector<double>> X_train = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    vector<vector<double>> y_train = {{0},    {1},    {1},    {0}};

    // Create the Neural Network
    // 2 inputs, 4 hidden neurons, 1 output
    NeuralNetwork nn(2, 4, 1);

    // Set up and run PSO
    int num_particles = 50;
    int max_iterations = 200;

    PSO pso_trainer(num_particles, max_iterations, nn, X_train, y_train);
    cout << "Starting PSO training..." << endl;
    pso_trainer.train();
    cout << "Training finished." << endl;

    // Get the best weights and test the trained network
    vector<double> best_weights = pso_trainer.g_best_position;
    nn.setWeights(best_weights);

    cout << "\n--- Testing Trained Network ---" << endl;
    for (const auto& input : X_train) {
        vector<double> prediction = nn.feedForward(input);
        cout << "Input: [" << input[0] << ", " << input[1] << "] -> Output: " << prediction[0] << endl;
    }

    return 0;
}
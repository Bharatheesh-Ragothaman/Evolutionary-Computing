#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
using namespace std;

// --- Helper for random number generation ---
double random_double(double min, double max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

// --- Neural Network Class ---
class NeuralNetwork {
public:
    int input_nodes;
    int hidden_nodes;
    int output_nodes;

    vector<vector<double>> w1;
    vector<double> b1;
    vector<vector<double>> w2;
    vector<double> b2;

    NeuralNetwork(int in, int hidden, int out)
        : input_nodes(in), hidden_nodes(hidden), output_nodes(out) {
        // Initialize weights and biases with random values
        w1.resize(in, vector<double>(hidden));
        b1.resize(hidden);
        w2.resize(hidden, vector<double>(out));
        b2.resize(out);

        for (int i = 0; i < in; ++i)
            for (int j = 0; j < hidden; ++j)
                w1[i][j] = random_double(-1.0, 1.0);

        for (int i = 0; i < hidden; ++i) b1[i] = random_double(-1.0, 1.0);

        for (int i = 0; i < hidden; ++i)
            for (int j = 0; j < out; ++j)
                w2[i][j] = random_double(-1.0, 1.0);

        for (int i = 0; i < out; ++i) b2[i] = random_double(-1.0, 1.0);
    }

    double sigmoid(double x) const {
        return 1.0 / (1.0 + exp(-x));
    }

    vector<double> forward(const vector<double>& x) const {
        vector<double> hidden_layer_output(hidden_nodes);
        for (int j = 0; j < hidden_nodes; ++j) {
            double sum = 0.0;
            for (int i = 0; i < input_nodes; ++i) {
                sum += x[i] * w1[i][j];
            }
            hidden_layer_output[j] = sigmoid(sum + b1[j]);
        }

        vector<double> final_output(output_nodes);
        for (int j = 0; j < output_nodes; ++j) {
            double sum = 0.0;
            for (int i = 0; i < hidden_nodes; ++i) {
                sum += hidden_layer_output[i] * w2[i][j];
            }
            final_output[j] = sigmoid(sum + b2[j]);
        }
        return final_output;
    }

    vector<double> get_weights_flat() const {
        vector<double> flat_weights;
        for (const auto& row : w1) flat_weights.insert(flat_weights.end(), row.begin(), row.end());
        flat_weights.insert(flat_weights.end(), b1.begin(), b1.end());
        for (const auto& row : w2) flat_weights.insert(flat_weights.end(), row.begin(), row.end());
        flat_weights.insert(flat_weights.end(), b2.begin(), b2.end());
        return flat_weights;
    }

    void set_weights_from_flat(const vector<double>& flat_weights) {
        int idx = 0;
        for (int i = 0; i < input_nodes; ++i)
            for (int j = 0; j < hidden_nodes; ++j)
                w1[i][j] = flat_weights[idx++];
        
        for (int i = 0; i < hidden_nodes; ++i) b1[i] = flat_weights[idx++];

        for (int i = 0; i < hidden_nodes; ++i)
            for (int j = 0; j < output_nodes; ++j)
                w2[i][j] = flat_weights[idx++];
        
        for (int i = 0; i < output_nodes; ++i) b2[i] = flat_weights[idx++];
    }
};

// --- Fitness Function ---
double fitness_function(const vector<double>& weights, NeuralNetwork& nn, 
                        const vector<vector<double>>& X, 
                        const vector<vector<double>>& y) {
    nn.set_weights_from_flat(weights);
    double total_error = 0.0;
    for (size_t i = 0; i < X.size(); ++i) {
        vector<double> prediction = nn.forward(X[i]);
        for (size_t j = 0; j < y[i].size(); ++j) {
            total_error += pow(prediction[j] - y[i][j], 2);
        }
    }
    return total_error / X.size();
}

// --- PSO Optimizer Class ---
class PSOOptimizer {
public:
    int num_particles;
    int dimensions;
    double w, c1, c2; // PSO parameters

    NeuralNetwork& nn;
    const vector<vector<double>>& X;
    const vector<vector<double>>& y;

    vector<vector<double>> swarm_pos;
    vector<vector<double>> swarm_vel;
    vector<vector<double>> pbest_pos;
    vector<double> pbest_fitness;
    vector<double> gbest_pos;
    double gbest_fitness;

    PSOOptimizer(int n_particles, int dims, NeuralNetwork& network,
                 const vector<vector<double>>& train_X,
                 const vector<vector<double>>& train_y,
                 double inertia = 0.7, double cognitive = 1.5, double social = 1.5)
        : num_particles(n_particles), dimensions(dims), nn(network), X(train_X), y(train_y),
          w(inertia), c1(cognitive), c2(social) {

        swarm_pos.resize(num_particles, vector<double>(dimensions));
        swarm_vel.resize(num_particles, vector<double>(dimensions));
        pbest_pos.resize(num_particles, vector<double>(dimensions));
        pbest_fitness.resize(num_particles);
        gbest_pos.resize(dimensions);
        gbest_fitness = numeric_limits<double>::max();

        // Initialize swarm
        for (int i = 0; i < num_particles; ++i) {
            for (int j = 0; j < dimensions; ++j) {
                swarm_pos[i][j] = random_double(-1.0, 1.0);
                swarm_vel[i][j] = random_double(-0.1, 0.1);
            }
            pbest_pos[i] = swarm_pos[i];
            pbest_fitness[i] = fitness_function(swarm_pos[i], nn, X, y);

            if (pbest_fitness[i] < gbest_fitness) {
                gbest_fitness = pbest_fitness[i];
                gbest_pos = pbest_pos[i];
            }
        }
    }

    vector<double> optimize(int iterations) {
        for (int iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < num_particles; ++i) {
                // Evaluate fitness
                double current_fitness = fitness_function(swarm_pos[i], nn, X, y);

                // Update pbest
                if (current_fitness < pbest_fitness[i]) {
                    pbest_fitness[i] = current_fitness;
                    pbest_pos[i] = swarm_pos[i];
                }

                // Update gbest
                if (current_fitness < gbest_fitness) {
                    gbest_fitness = current_fitness;
                    gbest_pos = swarm_pos[i];
                }
            }

            // Update particle velocities and positions
            for (int i = 0; i < num_particles; ++i) {
                for (int j = 0; j < dimensions; ++j) {
                    double r1 = random_double(0.0, 1.0);
                    double r2 = random_double(0.0, 1.0);

                    double cognitive_vel = c1 * r1 * (pbest_pos[i][j] - swarm_pos[i][j]);
                    double social_vel = c2 * r2 * (gbest_pos[j] - swarm_pos[i][j]);
                    
                    swarm_vel[i][j] = w * swarm_vel[i][j] + cognitive_vel + social_vel;
                    swarm_pos[i][j] += swarm_vel[i][j];
                }
            }
            if ((iter + 1) % 10 == 0) {
                cout << "Iteration " << iter + 1 << "/" << iterations << ", Best Fitness: " << gbest_fitness << endl;
            }
        }
        return gbest_pos;
    }
};

// --- Main Program ---
int main() {
    // 1. Define the XOR problem dataset
    vector<vector<double>> X = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    vector<vector<double>> y = {{0}, {1}, {1}, {0}};

    // 2. Create the Neural Network
    NeuralNetwork nn(2, 3, 1);
    int num_dimensions = nn.get_weights_flat().size();

    // 3. Set up and run the PSO Optimizer
    int num_particles = 100;
    int iterations = 100;

    PSOOptimizer pso(num_particles, num_dimensions, nn, X, y);

    cout << "Starting PSO training..." << endl;
    vector<double> best_weights = pso.optimize(iterations);
    cout << "\nTraining finished. Best fitness (MSE): " << pso.gbest_fitness << endl;

    // 4. Test the trained network
    nn.set_weights_from_flat(best_weights);
    cout << "\n--- Network Predictions after PSO Training ---" << endl;
    for (size_t i = 0; i < X.size(); ++i) {
        vector<double> prediction = nn.forward(X[i]);
        cout << "Input: [" << X[i][0] << ", " << X[i][1] << "] -> Output: " 
                  << prediction[0] << " (Expected: " << y[i][0] << ")" << endl;
    }

    return 0;
}
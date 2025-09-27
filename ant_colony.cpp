#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <limits>

using namespace std;

struct City {
    double x, y;
};


/* Calculates the Euclidean distance between two cities. */
double distance(const City& city1, const City& city2) {
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
}

/* Calculates the total length of a given tour.*/
double calculate_tour_length(const vector<int>& tour, const vector<City>& cities) {
    double length = 0.0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        length += distance(cities[tour[i]], cities[tour[i + 1]]);
    }
    // Add the distance from the last city back to the starting city.
    length += distance(cities[tour.back()], cities[tour.front()]);
    return length;
}

//Main ACO Class
class AntColonyOptimizer {
public:
    // ACO Hyperparameters
    int num_ants;
    double alpha;       // Pheromone influence
    double beta;        // Heuristic (distance) influence
    double rho;         // Pheromone evaporation rate
    double Q;           // Pheromone deposit factor

    vector<City> cities;
    vector<vector<double>> pheromones;
    vector<vector<double>> distances;
    int num_cities;

    vector<int> best_tour;
    double best_tour_length;

    // Modern C++ random number generator
    mt19937 rng;

    AntColonyOptimizer(const vector<City>& city_list, int ants, double a, double b, double r, double q)
        : num_ants(ants), alpha(a), beta(b), rho(r), Q(q) {
        
        cities = city_list;
        num_cities = cities.size();
        best_tour_length = numeric_limits<double>::max();

        // Initialize a random number generator with a random seed.
        random_device rd;
        rng = mt19937(rd());

        // Pre-calculate the distance matrix between all cities.
        distances.resize(num_cities, vector<double>(num_cities, 0.0));
        for (int i = 0; i < num_cities; ++i) {
            for (int j = i + 1; j < num_cities; ++j) {
                distances[i][j] = distances[j][i] = distance(cities[i], cities[j]);
            }
        }

        // Initialize the pheromone matrix with a small positive value.
        pheromones.resize(num_cities, vector<double>(num_cities, 1.0));
    }

    /** Runs the main optimization loop for a given number of iterations. */
    void run(int iterations) {
        for (int iter = 0; iter < iterations; ++iter) {
            vector<vector<int>> ant_tours;
            vector<double> ant_tour_lengths;

            // 1. Each ant constructs a complete tour.
            for (int k = 0; k < num_ants; ++k) {
                vector<int> tour = construct_tour();
                ant_tours.push_back(tour);
                ant_tour_lengths.push_back(calculate_tour_length(tour, cities));

                // If this ant's tour is the best found so far, save it.
                if (ant_tour_lengths.back() < best_tour_length) {
                    best_tour_length = ant_tour_lengths.back();
                    best_tour = tour;
                }
            }

            // 2. Update the pheromone trails based on the tours constructed by the ants.
            update_pheromones(ant_tours, ant_tour_lengths);
            
            if ((iter + 1) % 10 == 0) {
                cout << "Iteration " << iter + 1 << "/" << iterations 
                     << ", Best Tour Length: " << best_tour_length << endl;
            }
        }
    }

private:
    /** Constructs a single tour for one ant. */
    vector<int> construct_tour() {
        vector<int> tour;
        vector<bool> visited(num_cities, false);

        // Start the tour at a random city.
        uniform_int_distribution<int> start_dist(0, num_cities - 1);
        int current_city = start_dist(rng);
        tour.push_back(current_city);
        visited[current_city] = true;

        // Build the rest of the tour.
        while (tour.size() < num_cities) {
            current_city = select_next_city(current_city, visited);
            tour.push_back(current_city);
            visited[current_city] = true;
        }
        return tour;
    }

    /** Selects the next city for an ant based on pheromones and distance. */
    int select_next_city(int current_city, const vector<bool>& visited) {
        vector<double> move_probabilities(num_cities, 0.0);
        double total_prob = 0.0;

        // Calculate the probability to move to each unvisited city.
        for (int i = 0; i < num_cities; ++i) {
            if (!visited[i]) {
                double pheromone = pow(pheromones[current_city][i], alpha);
                double heuristic = pow(1.0 / distances[current_city][i], beta);
                move_probabilities[i] = pheromone * heuristic;
                total_prob += move_probabilities[i];
            }
        }

        // Use roulette wheel selection to pick the next city.
        uniform_real_distribution<double> rand_dist(0.0, total_prob);
        double r = rand_dist(rng);
        double cumulative_prob = 0.0;
        
        for (int i = 0; i < num_cities; ++i) {
            if (!visited[i]) {
                cumulative_prob += move_probabilities[i];
                if (r <= cumulative_prob) {
                    return i;
                }
            }
        }

        // Fallback: in case of floating point issues, pick the first available city.
        for (int i = 0; i < num_cities; ++i) {
            if (!visited[i]) return i;
        }
        return -1; // Should not happen in a valid TSP instance.
    }
    
    /** Updates pheromones: evaporation and deposition. */
    void update_pheromones(const vector<vector<int>>& ant_tours, const vector<double>& ant_tour_lengths) {
        // 1. Evaporation: Reduce all pheromone levels.
        for (int i = 0; i < num_cities; ++i) {
            for (int j = 0; j < num_cities; ++j) {
                pheromones[i][j] *= (1.0 - rho);
            }
        }

        // 2. Deposition: Add new pheromones based on the quality of the ants' tours.
        for (size_t k = 0; k < ant_tours.size(); ++k) {
            const auto& tour = ant_tours[k];
            double tour_length = ant_tour_lengths[k];
            double pheromone_deposit = Q / tour_length;

            for (size_t i = 0; i < tour.size() - 1; ++i) {
                int city1 = tour[i];
                int city2 = tour[i+1];
                pheromones[city1][city2] += pheromone_deposit;
                pheromones[city2][city1] += pheromone_deposit; // For symmetric TSP
            }
            // Deposit on the edge from the last city back to the first.
            pheromones[tour.back()][tour.front()] += pheromone_deposit;
            pheromones[tour.front()][tour.back()] += pheromone_deposit;
        }
    }
};

// Main Program
int main() {
    // 1. Define the problem: A list of city coordinates.
    vector<City> cities = {
        {60, 200}, {180, 200}, {80, 180}, {140, 180}, {20, 160},
        {100, 160}, {200, 160}, {140, 140}, {40, 120}, {100, 120},
        {180, 100}, {60, 80}, {120, 80}, {180, 60}, {20, 40},
        {100, 40}, {200, 40}, {20, 20}, {60, 20}, {160, 20}
    };

    // 2. Set ACO parameters.
    int num_ants = 25;
    int iterations = 200;
    double alpha = 1.0;
    double beta = 5.0;
    double rho = 0.5;
    double Q = 100.0;

    // 3. Create the optimizer and run it.
    AntColonyOptimizer aco(cities, num_ants, alpha, beta, rho, Q);
    cout << "Starting ACO for TSP..." << endl;
    aco.run(iterations);

    // 4. Print the final results.
    cout << "\n--- ACO Finished ---" << endl;
    cout << "Best tour length found: " << aco.best_tour_length << endl;
    cout << "Best tour path: ";
    for (int city_index : aco.best_tour) {
        cout << city_index << " -> ";
    }
    cout << aco.best_tour[0] << endl;

    return 0;
}
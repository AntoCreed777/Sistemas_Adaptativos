#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>

#include "brkga_mp_ipr_cpp/brkga_mp_ipr/brkga_mp_ipr.hpp"
#include "brkga_decoder.h"
#include "brkga_encoder.h"
#include "cxxopts/cxxopts.hpp"
#include "graph_matrix.h"
#include "utils.h"          // para graph_creation::read_graph(...)
#include "greedy_random.h"

using Clock = std::chrono::high_resolution_clock;

int main(int argc, char* argv[]) {
    try {
        cxxopts::Options options("BRKGA", "Metaheuristica poblacional para MISP");
        options.add_options()
            // Parametros default de la pauta
            ("i,input", "Ruta de la instancia", cxxopts::value<std::string>())
            ("c,config_file", "Ruta del archivo de configuracion", cxxopts::value<std::string>())
            ("t,time_limit", "Tiempo maximo en segundos", cxxopts::value<int>()->default_value("10"))
            ("seed", "Semilla del generador de números aleatorios", cxxopts::value<int>()->default_value("1234"))
            ("num_threads", "Numero de Threads", cxxopts::value<int>()->default_value("1"))

            // Helper
            ("h,help",    "Mostrar ayuda");

        auto result = options.parse(argc, argv);

        if (result.count("help") || !result.count("input")) {
            std::cout << options.help() << "\n";
            return result.count("help") ? 0 : 1;
        }

        ////////////////////////////////////////
        // Read command-line arguments and the instance
        ////////////////////////////////////////

        const std::string instancia = result["input"].as<std::string>();
        const std::string config_file  = result["config_file"].as<std::string>();
        const int seconds_limit = result["time_limit"].as<int>();
        const int seed = result["seed"].as<int>();
        const int num_threads = result["num_threads"].as<int>();

        // Validacion de variables
        //if (seconds_limit <= 0) throw std::range_error("El limite de tiempo debe de ser mayor a 0 en segundos");
        if (seed <= 0) throw std::range_error("La semilla (seed) debe ser mayor a 0");
        if (num_threads <= 0) throw std::range_error("Como minimo debe de haber 1 thread");

        // Cargar grafo (mismo helper que en tu main_greedy)
        GraphMatrix g = graph_creation::read_graph(instancia);

        ////////////////////////////////////////
        // Read algorithm parameters
        ////////////////////////////////////////
        auto [brkga_params, control_params] = BRKGA::readConfiguration(config_file);

        // Overwrite the maximum time from the config file.
        control_params.maximum_running_time = std::chrono::seconds {seconds_limit};

        ////////////////////////////////////////
        // Build the BRKGA data structures
        ////////////////////////////////////////

        BRKGADecoder decoder(g);

        BRKGA::BRKGA_MP_IPR<BRKGADecoder> algorithm(
            decoder,
            BRKGA::Sense::MAXIMIZE,
            unsigned(seed),
            unsigned(g.get_num_vertices()),
            brkga_params,
            unsigned(num_threads)
        );

        ////////////////////////////////////////
        // Find Pre-Solutions with Heuristics for the Initial Population
        ////////////////////////////////////////

        std::vector<BRKGA::Chromosome> initial_population(brkga_params.population_size);

        for (unsigned i = 0; i < brkga_params.population_size; i++)
            initial_population[i] = encode(g.get_num_vertices(), greedy_random::solve_misp(g));

        algorithm.setInitialPopulation(initial_population);

        ////////////////////////////////////////
        // Find good solutions / evolve
        ////////////////////////////////////////
        
        const auto final_status = algorithm.run(control_params, &std::cout);
        std::cout << final_status.best_fitness << ";" << final_status.current_time.count() << ";F" << std::endl;

        return 0;

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error en opciones: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}

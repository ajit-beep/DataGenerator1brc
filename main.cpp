#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>


bool parse_args(int argc, char* argv[], uint64_t& num_rows, int& num_stations, std::string& output_file) {
    num_rows = 0;
    num_stations = 0;
    output_file = "large_dataset.txt";

    for (int i = 1; i < argc ; i++) {
        std::string arg = argv[i];
        if((arg == "--rows" || arg == "-r") && i + 1 < argc) {
            num_rows = std::stoull(argv[++i]);
        } else if((arg == "--stations" || arg == "-s") && i + 1 < argc) {
            num_stations = std::stoi(argv[++i]);
        } else if((arg == "--output" || arg == "-o") && i + 1 < argc) {
            output_file = argv[++i];
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return false;
        }
    }

    if (num_rows == 0 || num_stations == 0) {
        std::cerr << "Error: Number of rows and number of stations must be greater than zero." << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {

    uint64_t num_rows;
    int num_stations;
    std::string output_file;


	if(!parse_args(argc, argv, num_rows, num_stations, output_file)) {
        std::cerr << "Usage: " << argv[0] << " --rows <num_rows> --stations <num_stations> --output <output_file>" << std::endl;
        std::cout << "Example: " << argv[0] << " --rows 100000000 --stations 1000 --output large_dataset.txt" << std::endl;
        return 1;
    }

    // start timing
    auto start = std::chrono::high_resolution_clock::now();

    std::ofstream outfile(output_file);
    if(!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << output_file << " for writing." << std::endl;
        return 1;
    }

	std::vector<std::string> station_names;
    station_names.reserve(num_stations);
	for (int i = 0; i < num_stations; ++i) {
		station_names.push_back("Station_" + std::to_string(i));
	}

	std::default_random_engine generator(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> station_dist(0, num_stations - 1);
	std::uniform_real_distribution<float> temp_dist(-50.0, 50.0);

    const uint64_t progress_interval = num_rows / 100;
    uint64_t next_progress = progress_interval;
    std::cout << "Generating " << num_rows << " lines of data..." << std::endl;

	for(uint64_t i = 0; i < num_rows; ++i) {
        const std::string& station = station_names[station_dist(generator)];
        float temp = temp_dist(generator);

        outfile << station << ";" << std::fixed << std::setprecision(1) << temp <<"\n";

        if (i+1 >= next_progress) {
            double progress = (static_cast<double>(i+1)/num_rows) * 100;
            std::cout << "\rProgress: " << std::fixed << std::setprecision(1) << progress << "%" << std::flush;
            next_progress += progress_interval;
        }
        
    }

    outfile.close();

    //end timing
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start;

    std::cout << "\nDataset generated completed in " << duration.count() << " seconds" << std::endl;

    return 0;
}
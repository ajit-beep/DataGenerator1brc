#include <iostream>
#include <fstream>
#include <string>
#include <random>

int main() {
	std::ofstream outfile("large_dataset.txt");
	if (!outfile.is_open()) {
		std::cerr << "Error opening file for writing.\n";
		return 1;
	}

	const uint64_t NUM_LINES = 100000000;
	const int NUM_STATIONS = 1000;

	std::vector<std::string> station_names;
	for (int i = 0; i < NUM_STATIONS; ++i) {
		station_names.push_back("Station_" + std::to_string(i));
	}

	std::default_random_engine generator;
	std::uniform_int_distribution<int> station_dist(0, NUM_STATIONS - 1);
	std::uniform_real_distribution<float> temp_dist(-50.0, 50.0);

	for(uint64_t i = 0; i < NUM_LINES; ++i) {
        const std::string& station = station_names[station_dist(generator)];
        float temp = temp_dist(generator);

        outfile << station << ";" << temp << "\n";

        // print progress
        // if (i % 1000000 == 0) {
        //     std::cout << "Writing line " << i << " of " << NUM_LINES << "\n";
        // }
    }

    outfile.close();
}
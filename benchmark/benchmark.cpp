#include <hashmaps/HashMapLL.h>
#include <hashmaps/HashMapDH.h>
#include <hashmaps/HashMapRH.h>

#include <vector>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <ctime>
#include <sstream>

std::vector<std::string> getElements(const std::string& s) {
    std::vector<std::string> row;
    std::string element;

    bool inQuotes = false;
    for (auto &c: s) {
        if (c == '"') inQuotes = !inQuotes;
        else if (c == ',' && !inQuotes) {
            element.erase(std::remove(element.begin(), element.end(), '\"'), element.end());
            row.push_back(element); element.clear();
        } else element += c;
    }

    element.erase(std::remove(element.begin(), element.end(), '\"'), element.end());
    row.push_back(element);
    return row;
}

std::vector<std::vector<std::string>> getData(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::fstream file;
    file.open(filename, std::fstream::in);
    std::string line;

    if (file.is_open()) {
        std::getline(file, line);
        while(!file.eof()) {
            std::getline(file, line);
            data.push_back(getElements(line));
        }
        file.close();
    }
    return data;
}

size_t nearestPowerOf2(size_t number) {
    if (number < 1) return number;
    auto power = static_cast<size_t>(std::pow(2, std::floor(std::log2(number))));
    if (number - power <= power / 2) return power;
    else return power * 2;
}

std::string convertToString(char *buffer) {
    std::string s(buffer);
    return s;
}

std::vector<std::string> analyse(const std::vector<std::vector<std::string>>& data) {
    std::vector<int> values = {50, 100, 250, 500, 1000, 5000, 10000, 15000, 30000, 50000, 75000, 100000, 150000};
    std::vector<float> loadFactors = {0.75f, 0.80f, 0.90f, 0.95f, 0.99f};

    HashMapLL<std::string, float> *hashMapLL;
    HashMapDH<std::string, float> *hashMapDH;
    HashMapRH<std::string, float> *hashMapRH;

    std::vector<std::string> results;
    results.emplace_back("HashMap,\"Number of elements\",Load factor,Operation,Time");
    char buffer[100];

    for (float loadFactor : loadFactors) {
        for (int value : values) {
            auto hashMapSize = static_cast<size_t>(std::floor(value / loadFactor));
            hashMapLL = new HashMapLL<std::string, float>(nearestPowerOf2(hashMapSize), loadFactor);
            hashMapDH = new HashMapDH<std::string, float>(hashMapSize, loadFactor);
            hashMapRH = new HashMapRH<std::string, float>(nearestPowerOf2(hashMapSize), loadFactor);

            // Adding elements to hash maps and benchmarking performance
            auto start = std::chrono::high_resolution_clock::now();
            for (size_t e = 0; e < value; e++) {
                hashMapLL->put(data[e][0], std::stof(data[e][1]));
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "LL",
                         value,
                         loadFactor,
                         "put",
                         duration.count());
            results.push_back(convertToString(buffer));

            start = std::chrono::high_resolution_clock::now();
            for (size_t e = 0; e < value; e++) {
                hashMapDH->put(data[e][0], std::stof(data[e][1]));
            }
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "DH",
                         value,
                         loadFactor,
                         "put",
                         duration.count());
            results.push_back(convertToString(buffer));

            start = std::chrono::high_resolution_clock::now();
            for (size_t e = 0; e < value; e++) {
                hashMapRH->put(data[e][0], std::stof(data[e][1]));
            }
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "RH",
                         value,
                         loadFactor,
                         "put",
                         duration.count());
            results.push_back(convertToString(buffer));


            // Coping data for shuffling and further benchmarks
            std::vector<std::vector<std::string>> copiedData(data.begin(), data.begin() + value);
            std::random_device rd;
            std::mt19937 rng(rd());
            std::shuffle(copiedData.begin(), copiedData.end(), rng);

            // Looking up for elements in hash maps and benchmarking performance
            start = std::chrono::high_resolution_clock::now();
            for (size_t e = 0; e < value; e++) {
                hashMapLL->containsKey(copiedData[e][0]);
            }
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "LL",
                         value,
                         loadFactor,
                         "containsKey",
                         duration.count());
            results.push_back(convertToString(buffer));

            start = std::chrono::high_resolution_clock::now();
            for (size_t e = 0; e < value; e++) {
                hashMapDH->containsKey(copiedData[e][0]);
            }
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "DH",
                         value,
                         loadFactor,
                         "containsKey",
                         duration.count());
            results.push_back(convertToString(buffer));

            start = std::chrono::high_resolution_clock::now();
            for (size_t e = 0; e < value; e++) {
                hashMapRH->containsKey(copiedData[e][0]);
            }
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "RH",
                         value,
                         loadFactor,
                         "containsKey",
                         duration.count());
            results.push_back(convertToString(buffer));

            // Intentionally looking for element that does not exist in hash maps and benchmarking performance
            start = std::chrono::high_resolution_clock::now();
            hashMapLL->containsKey("im-not-existing");
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "LL",
                         value,
                         loadFactor,
                         "containsKeyFailed",
                         duration.count());
            results.push_back(convertToString(buffer));

            start = std::chrono::high_resolution_clock::now();
            hashMapDH->containsKey("im-not-existing");
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "DH",
                         value,
                         loadFactor,
                         "containsKeyFailed",
                         duration.count());
            results.push_back(convertToString(buffer));

            start = std::chrono::high_resolution_clock::now();
            hashMapLL->containsKey("im-not-existing");
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "RH",
                         value,
                         loadFactor,
                         "containsKeyFailed",
                         duration.count());
            results.push_back(convertToString(buffer));

            // Removing elements from hash maps and benchmarking performance
            start = std::chrono::high_resolution_clock::now();
            for (size_t e = 0; e < value; e++) {
                hashMapLL->remove(copiedData[e][0]);
            }
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "LL",
                         value,
                         loadFactor,
                         "remove",
                         duration.count());
            results.push_back(convertToString(buffer));

            start = std::chrono::high_resolution_clock::now();
            for (size_t e = 0; e < value; e++) {
                hashMapDH->remove(copiedData[e][0]);
            }
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "DH",
                         value,
                         loadFactor,
                         "remove",
                         duration.count());
            results.push_back(convertToString(buffer));

            start = std::chrono::high_resolution_clock::now();
            for (size_t e = 0; e < value; e++) {
                hashMapRH->remove(copiedData[e][0]);
            }
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            std::sprintf(buffer,
                         "%s,%d,%.2f,%s,%lld",
                         "RH",
                         value,
                         loadFactor,
                         "remove",
                         duration.count());
            results.push_back(convertToString(buffer));
        }
    }

    return results;
}

std::string writeToCSVFile(const std::vector<std::string>& results) {
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* timeInfo = std::localtime(&currentTime);
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%d-%m-%Y_%H:%M:%S");
    char buffer[100];
    std::sprintf(buffer, "results-%s.csv", oss.str().c_str());

    std::fstream file;
    file.open(convertToString(buffer), std::fstream::out | std::fstream::trunc);
    if (file.is_open()) {
        for (size_t i = 0; i < results.size(); i++) {
            if (i == results.size() - 1) file << results[i];
            else file << results[i] << "\n";
        }
        file.close();
        return "Successfully saved results to file!";
    }

    return "There was a problem with saving results to file";
}

int main(int argc, char *argv[]) {
    if (argc != 2) throw std::invalid_argument("Benchmark requires the path of file from which it reads data");
    std::vector args(argv + 1, argv + argc);

    auto results = analyse(getData(args[0]));
    std::cout << writeToCSVFile(results) << "\n";

    return 0;
}
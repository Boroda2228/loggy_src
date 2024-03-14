#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <numeric>

double extract_number(const std::string& str) {
    auto it = std::find_if(str.begin(), str.end(), [](unsigned char c) { return std::isdigit(c); });

    if (it == str.end()) {
        //no numbers in input string
        return 0;
    }

    // transform substr from first digit to nmber
    return std::stod(std::string(it, it+5 ));
}

void copy_lines(const std::string& input_file_path, const std::string& output_file_path, const std::string& product_id, std::vector<double>& test_time_val)
{
    std::ifstream input_file(input_file_path);
    std::ofstream output_file(output_file_path);

    if (!input_file.is_open() || !output_file.is_open()) 
        {
            std::cout << "Unable to open file"; 
            return;
        }
    
    std::string line;

    
while (std::getline(input_file, line))
    {
        bool is_product = false;
        if (std::regex_search(line, std::regex(product_id)))
        {
            is_product = true;
          //  output_file << line << '\n';
        }

        // find corresponding test time
        if (is_product)
        {
            while (std::getline(input_file, line))
            {
                if (std::regex_search(line, std::regex(";   test time   : ")))
                {
                    double number = extract_number(line);
                    output_file << std::fixed << std::setprecision(2) << number << '\n';
                    test_time_val.push_back(number);
                    is_product = false;
                    number = 0.0;
                    break;
                }
            }
        }
    }
}

int main() {
    using namespace std;
    string input_file_path = "";
    string output_file_path = "";
    string product_id = "";
    vector<double> test_time_val;
    cout << "Please type product_id in #XXPUSXXXX/60 format, 12NC or 6 digits of SN#: " << endl; 
    cin >> product_id;
    cout << "Please type input file path: " << endl; 
    cin >> input_file_path;
    cout << "Please type output file path: " << endl; 
    cin >> output_file_path;

    std::ifstream input_file(input_file_path);
    std::ofstream output_file(output_file_path);


    copy_lines(input_file_path, output_file_path, product_id, test_time_val);

        if (test_time_val.empty()) {
        std::cout << "Vector is empty." << std::endl;
        return 0;
    }

    double min = *std::min_element(test_time_val.begin(), test_time_val.end());
    double max = *std::max_element(test_time_val.begin(), test_time_val.end());

    double sum = accumulate(test_time_val.begin(), test_time_val.end(), 0.0);
    double mean = sum / test_time_val.size();

    double variance = accumulate(test_time_val.begin(), test_time_val.end(), 0.0,
        [mean](const double a, const double b) {
             return a + (b - mean) * (b - mean);
            }) / test_time_val.size();


    std::cout << "Average value: " << mean << std::endl;
    std::cout << "Max: " << max << std::endl;
    std::cout << "Min: " << min << std::endl;
    std::cout << "Dispersion: " << variance << std::endl;


    if (!input_file.is_open() || !output_file.is_open()) 
        {
            std::cout << "Unable to open file"; 
        }

    output_file << "Average value  "<< mean << '\n';
    output_file << "Max:  " << max << '\n';
    output_file << "Min:  " << min << '\n';
    output_file << "Dispersion:  " << variance << '\n';

}


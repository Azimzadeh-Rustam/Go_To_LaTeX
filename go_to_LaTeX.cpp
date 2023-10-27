#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>

using namespace std;

const std::map<char, int> PULSE_TO_DIGIT{
    {'K', 2},
    {'L', 3},
    {'Q', 5},
    {'U', 7},
    {'O', 11},
    {'I', 13}
};

// Regular expressions for searching
const std::string SEARCH_PATTERNS[] {
    "e\\^(-?\\d+)",                          //0
    "F_(-?\\d+)[\\^_]-?\\d+\\{(-?\\d+)\\}",  //1
    "F#_(-?\\d+)[\\^_]-?\\d+\\{(-?\\d+)\\}", //2
    "F1_(-?\\d+)[\\^_]-?\\d+\\{(-?\\d+)\\}", //3
    "V_(-?\\d+)[\\^_]-?\\d+\\{(-?\\d+)\\}",  //4
    "d_\\{(-?\\d+)\\}\\{(-?\\d+)\\}",        //5
    "I\\{(-?\\d+)\\^-(\\d+)\\}",             //6
    "I\\{(-?\\d+)\\^(\\d+)\\}",              //7
    "I\\{(-?\\d+)_(-?\\d+)\\}",              //8
    "K2[-_\\^\\d+]*?\\{(-?\\d+)\\}",         //9
    "K3[-_\\^\\d+]*?\\{(-?\\d+)\\}",         //10
    "K4[-_\\^\\d+]*?\\{(-?\\d+)\\}",         //11
    "K5[-_\\^\\d+]*?\\{(-?\\d+)\\}"          //12
};

// LaTeX templates
const std::string LATEX_SAMPLES[] {
    "{e}^{argument1}",
    "{\\phi}_{argument1}(argument2,\\theta)",
    "{\\phi}^{*}_{argument1}(argument2,\\theta)",
    "\\tilde{\\phi}_{argument1}(argument2,\\theta)",
    "{V}_{argument1}(argument2)",
    "{\\delta}^8_{argument2}(argument1)",
    "\\frac{1}{argument1^{argument2}}",
    "{argument1}^{argument2}",
    "{argument1}_{argument2}",
    "\\frac{1}{argument1^2+M^2}",
    "\\frac{M}{argument1^2+M^2}",
    "\\frac{{\\xi}_0}{K_{argument1}}",
    "\\frac{1}{R_{argument1}}"
};

// Titles
const std::string DESIGNATIONS[] {
    "Констант связи",
    "Киральных полей",
    "Сопряжённых киральных полей",
    "Киральных полей с тильдой",
    "Вещественных полей",
    "Дельта функции",
    "Дробных констант",
    "Степенных констант",
    "Констант с нижним индексом",
    "Констант типа 2",
    "Констант типа 3",
    "Констант типа 4",
    "Констант типа 5"
};

std::string RemoveHyphensAndSpaces(const std::string& input) {
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) {
        return std::isspace(c);
        }), result.end());
    return result;
}

std::string ReplaceStarsWithCdots(const std::string& input) {
    std::string result(input);
    size_t pos = result.find('*');
    while (pos != std::string::npos) {
        result.replace(pos, 1, "\\cdot");
        pos = result.find('*', pos + 1);
    }
    return result;
}

std::string DecodeNumber(std::string input) {

    if (input.empty()) {
        return "";
    }
    else if (input == "1" || input == "-1") {
        return "0";
    }

    int number = std::stoi(input);

    // Factoring a number into prime factors
    std::vector<int> prime_factors;
    int current_code(number);
    for (const auto& pair : PULSE_TO_DIGIT) {
        char pulse(pair.first);
        int digit(pair.second);
        while (current_code % digit == 0) {
            prime_factors.push_back(digit);
            current_code /= digit;
        }
    }

    // Generating an analytical view as a string
    int prime_factors_size(prime_factors.size());
    std::string analytical_representation;
    for (size_t i = 0; i < prime_factors_size; ++i) {
        char pulse;
        for (const auto& pair : PULSE_TO_DIGIT) {
            if (pair.second == prime_factors[i]) {
                pulse = pair.first;
                break;
            }
        }
        analytical_representation += pulse;
        if (i < prime_factors_size - 1) {
            analytical_representation += " + ";
        }
    }

    if (number < 0 && prime_factors_size > 1) {
        return "-(" + analytical_representation + ")";
    }
    else if (number > 0 && prime_factors_size > 1) {
        return "(" + analytical_representation + ")";
    }
    else if (number < 0) {
        return "-" + analytical_representation;
    }
    else {
        return analytical_representation;
    }
}

std::string AddPulseIntegrals(const std::string& input) {
    std::string result(input);

    result = std::regex_replace(result, std::regex("\\{e\\}\\^\\{-?\\d+\\}"), "$&\\int{}d^4\\theta");

    for (const auto& pair : PULSE_TO_DIGIT) {
        char pulse = pair.first;
        if (result.find(pulse) != std::string::npos) {
            std::string pulse_frac("\\frac{d^4" + std::string(1, pulse) + "}{(2\\pi)^4}$&");
            result = std::regex_replace(result, std::regex("\\}d\\^4\\\\theta"), pulse_frac);
        }
    }
    return result;
}

void GoToLaTeX(std::string input) {

    const int REVERSE_ORDER_INDEX = 5;
    int count = 0; // Substitution counter
    std::vector<std::string> warnings; // A vector for storing the names of expressions that were not found in the source string

    input = RemoveHyphensAndSpaces(input);
    input = ReplaceStarsWithCdots(input);

    for (int i = 0; i < size(SEARCH_PATTERNS); i++) {
        std::vector<std::string> expressions; // Vector for storing found expressions
        std::vector<std::string> LaTeX_expressions; // Vector for storing found expressions in LaTeX format
        size_t pos = 0;
        std::cmatch result; // This variable, after executing the code, will be an array, the zeroth element of which will be the found expression itself, and all the rest will correspond to the partitions in the regular expression
        std::regex regular(SEARCH_PATTERNS[i]); // Regular expression
        std::sregex_iterator regex_iterator(input.begin(), input.end(), regular); // Iterator to find all given expressions

        // Loop through all given expressions and add them to the expressions vector
        std::sregex_iterator end_iterator;
        while (regex_iterator != end_iterator) {
            expressions.push_back(regex_iterator->str());
            regex_iterator++;
        }

        if (!expressions.empty()) {
            std::sort(expressions.begin(), expressions.end()); // Sort the expressions vector to group repeating elements
            expressions.erase(std::unique(expressions.begin(), expressions.end()), expressions.end()); // Removing duplicates

            // Generate LaTeX syntax for given expressions and write them to the LaTeX_expressions vector
            for (int j = 0; j < expressions.size(); j++) {

                if (std::regex_search(expressions[j].c_str(), result, regular)) {
                    std::string LaTeX_template = LATEX_SAMPLES[i];
                    // An array whose elements represent the arguments in the LaTeX template to be replaced
                    std::string params[]{ "argument1", "argument2" };

                    std::string argument1, argument2;
                    // Taking into account the correct order of inserting arguments into LaTeX templates
                    if (i < REVERSE_ORDER_INDEX) {
                        argument1 = result[1];
                        argument2 = DecodeNumber(result[2]);
                    }
                    else {
                        argument1 = DecodeNumber(result[1]);
                        argument2 = result[2];
                    }
                    // An array whose elements represent the values with which to replace the arguments in the LaTeX template
                    std::string new_params[]{ argument1, argument2 };

                    // Substituting values into corresponding arguments in a LaTeX template
                    for (int k = 0; k < std::size(params); k++) {
                        pos = 0;
                        while ((pos = LaTeX_template.find(params[k], pos)) != std::string::npos) {
                            LaTeX_template.replace(pos, params[k].length(), new_params[k]);
                            pos += new_params[k].length();
                        }
                    }
                    // Writing received LaTeX expressions into the LaTeX_expressions vector
                    LaTeX_expressions.push_back(LaTeX_template);
                }
            }

            // Replace all given expressions with LaTeX format in the source string
            for (int n = 0; n < std::size(expressions); n++) {
                pos = 0;
                while ((pos = input.find(expressions[n], pos)) != std::string::npos) {
                    input.replace(pos, expressions[n].length(), LaTeX_expressions[n]);
                    pos += LaTeX_expressions[n].length();
                    count++;
                }
            }
        }
        else {
            warnings.push_back(DESIGNATIONS[i]);
        }
    }

    // Inferences
    std::cout << "Формат LaTeX:\n" << "$" + AddPulseIntegrals(input) + "$\n" << std::endl;

    std::cout << "Заменено выражений: " << count << "\n" << std::endl;

    int warnings_size = warnings.size();
    if (warnings_size > 0) {
        std::cout << "В исходном коде не было найдено:\n";
        for (int i = 0; i < warnings_size; i++) {
            std::cout << i + 1 << ". " << warnings[i] << std::endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "ru");

    // Source string
    std::string program_format("-8*e^8*F_1_1{1}*F#_1^2{-1}*K4{2}*I{2^-4}*K4{5}*I{5^-2}*K4{7}*I{7^-2}*K4{3}*I{3^-2}*I{210^-2}*I{10^-2}*I{30^-2}");

    GoToLaTeX(program_format);

    return 0;
}
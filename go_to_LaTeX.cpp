#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>

using namespace std;

std::string Remove_Hyphens_And_Spaces(const std::string& input) {
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) {
        return std::isspace(c);
        }), result.end());
    return result;
}

std::string Replace_Stars_With_Cdots(const std::string& input) {
    std::string result = input;
    size_t pos = result.find('*');
    while (pos != std::string::npos) {
        result.replace(pos, 1, "\\cdot");
        pos = result.find('*', pos + 1);
    }
    return result;
}

std::string Decode_Number(std::string str) {

    if (str == "") {
        return "";
    }

    int num = std::stoi(str);

    if (num == 1 || num == -1) {
        return "0";
    }

    // Encoding variables and their values as pairs (variable, code)
    std::map<char, int> variableToCode = {
        {'k', 2},
        {'l', 3},
        {'q', 5},
        {'p', 7},
        {'m', 11},
        {'n', 13}
    };

    // Factoring a number into prime factors
    std::vector<int> primeFactors;
    int currentCode = num;
    for (const auto& pair : variableToCode) {
        char variable = pair.first;
        int code = pair.second;
        while (currentCode % code == 0) {
            primeFactors.push_back(code);
            currentCode /= code;
        }
    }

    // Generating an analytical view as a string
    std::string analyticalRepresentation;
    for (size_t i = 0; i < primeFactors.size(); ++i) {
        char variable;
        for (const auto& pair : variableToCode) {
            if (pair.second == primeFactors[i]) {
                variable = pair.first;
                break;
            }
        }
        analyticalRepresentation += variable;
        if (i < primeFactors.size() - 1) {
            analyticalRepresentation += " + ";
        }
    }

    if (num < 0 && primeFactors.size() > 1) {
        return "-(" + analyticalRepresentation + ")";
    }
    else if (num < 0) {
        return "-" + analyticalRepresentation;
    }

    return analyticalRepresentation;
}

void Go_To_LaTeX(std::string str) {

    int count = 0; // Substitution counter
    std::vector<std::string> warnings; // A vector for storing the names of expressions that were not found in the source string

    // Regular expressions for searching
    std::vector<std::string> searchPatterns = {
        "(e\\^)(-?\\d+)",
        "(d_\\{)(-?\\d+)(\\}\\{)(-?\\d+)(\\})",
        "(F_)(-?\\d+)([\\^_]-?\\d+\\{)(-?\\d+)(\\})",
        "(F#_)(-?\\d+)([\\^_]-?\\d+\\{)(-?\\d+)(\\})",
        "(F1_)(-?\\d+)([\\^_]-?\\d+\\{)(-?\\d+)(\\})",
        "(V_)(-?\\d+)([\\^_]-?\\d+\\{)(-?\\d+)(\\})",
        "(I\\{)(\\d+)(\\^-)(\\d+)(\\})",
        "(I\\{)(\\d+)(\\^)(\\d+)(\\})",
        "(I\\{)([\\w-]+)([_]+)([\\w-]+)(\\})",
        "(K2[-_\\^\\d+]*?\\{)(-?\\d+)(\\})",
        "(K3[-_\\^\\d+]*?\\{)(-?\\d+)(\\})",
        "(K4[-_\\^\\d+]*?\\{)(-?\\d+)(\\})",
        "(K5[-_\\^\\d+]*?\\{)(-?\\d+)(\\})"
    };

    // LaTeX templates
    std::vector<std::string> LaTeXSamples = {
        "{e}^{argument1}",
        "{\\delta}^8_{argument2}(argument1)",
        "{\\phi}_{argument1}(argument2,\\theta)",
        "{\\phi}^{*}_{argument1}(argument2,\\theta)",
        "\\tilde{\\phi}_{argument1}(argument2,\\theta)",
        "{V}_{argument1}(argument2)",
        "\\frac{1}{{argument1}^{argument2}}",
        "{argument1}^{argument2}",
        "{argument1}_{argument2}",
        "\\frac{1}{{argument1}^2+M^2}",
        "\\frac{M}{{argument1}^2+M^2}",
        "\\frac{{\\xi}_0}{K_{argument1}}",
        "\\frac{1}{R_{argument1}}"
    };

    // Titles
    std::vector<std::string> designations = {
        "Констант связи",
        "Дельта функции",
        "Киральных полей",
        "Сопряжённых киральных полей",
        "Киральных полей с тильдой",
        "Вещественных полей",
        "Дробных констант",
        "Степенных констант",
        "Констант с нижним индексом",
        "Констант типа 2",
        "Констант типа 3",
        "Констант типа 4",
        "Констант типа 5"
    };

    str = Remove_Hyphens_And_Spaces(str);
    str = Replace_Stars_With_Cdots(str);

    for (int i = 0; i < searchPatterns.size(); i++) {
        std::vector<std::string> expressions; // Vector for storing found expressions
        std::vector<std::string> LaTeXExpressions; // Vector for storing found expressions in LaTeX format
        size_t pos = 0;
        std::cmatch result; // This variable, after executing the code, will be an array, the zeroth element of which will be the found expression itself, and all the rest will correspond to the partitions in the regular expression
        std::regex regular(searchPatterns[i]); // Regular expression
        std::sregex_iterator regex_iterator(str.begin(), str.end(), regular); // Iterator to find all given expressions

        // Loop through all given expressions and add them to the expressions vector
        std::sregex_iterator end_iterator;
        while (regex_iterator != end_iterator) {
            expressions.push_back(regex_iterator->str());
            regex_iterator++;
        }

        if (expressions.size() > 0) {
            std::sort(expressions.begin(), expressions.end()); // Sort the expressions vector to group repeating elements
            expressions.erase(std::unique(expressions.begin(), expressions.end()), expressions.end()); // Removing duplicates

            // Generate LaTeX syntax for given expressions and write them to the LaTeXExpressions vector
            for (int j = 0; j < expressions.size(); j++) {

                if (std::regex_search(expressions[j].c_str(), result, regular)) {
                    std::string LaTeXTemplate = LaTeXSamples[i];
                    // An array whose elements represent the arguments in the LaTeX template to be replaced
                    std::string params[]{ "argument1", "argument2" };

                    std::string argument1;
                    std::string argument2;
                    // Taking into account the correct order of inserting arguments into LaTeX templates
                    if (i == 0 || i == 2 || i == 3 || i == 4 || i == 5) {
                        argument1 = result[2];
                        argument2 = Decode_Number(result[4]);
                    }
                    else {
                        argument1 = Decode_Number(result[2]);
                        argument2 = result[4];
                    }
                    // An array whose elements represent the values with which to replace the arguments in the LaTeX template
                    std::string newParams[]{ argument1, argument2 };

                    // Substituting values into corresponding arguments in a LaTeX template
                    for (int k = 0; k < std::size(params); k++) {
                        pos = 0;
                        while ((pos = LaTeXTemplate.find(params[k], pos)) != std::string::npos) {
                            LaTeXTemplate.replace(pos, params[k].length(), newParams[k]);
                            pos += newParams[k].length();
                        }
                    }
                    // Writing received LaTeX expressions into the LaTeXExpressions vector
                    LaTeXExpressions.push_back(LaTeXTemplate);
                }
            }

            // Replace all given expressions with LaTeX format in the source string
            for (int n = 0; n < std::size(expressions); n++) {
                pos = 0;
                while ((pos = str.find(expressions[n], pos)) != std::string::npos) {
                    str.replace(pos, expressions[n].length(), LaTeXExpressions[n]);
                    pos += LaTeXExpressions[n].length();
                    count++;
                }
            }
        }
        else {
            warnings.push_back(designations[i]);
        }
    }

    // Inferences
    std::cout << "Формат LaTeX:\n" << "$" + str + "$\n" << std::endl;

    std::cout << "Заменено выражений: " << count << "\n" << std::endl;

    int warningsSize = warnings.size();
    if (warningsSize > 0) {
        std::cout << "В исходном коде не было найдено:\n";
        for (int i = 0; i < warningsSize; i++) {
            std::cout << i + 1 << ". " << warnings[i] << std::endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "ru");

    // Source string
    std::string programFormat = "-8*e^8*F_1_1{1}*F#_1^2{-1}*K4{2}*I{2^-4}*K4{5}*I{5^-2}*K4{7}*I{7^-2}*K4{3}*I{3^-2}*I{210^-2}*I{10^-2}*I{30^-2}";

    Go_To_LaTeX(programFormat);

    return 0;
}
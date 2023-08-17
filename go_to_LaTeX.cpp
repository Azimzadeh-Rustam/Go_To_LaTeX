#include <iostream>
#include <regex>
#include <vector>

using namespace std;

// Удаление пробелов и переносов в исходной строке
std::string removeHyphensAndSpaces(const std::string& input)
{
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) {
        return std::isspace(c);
    }), result.end());
    return result;
}

int main()
{
    setlocale(LC_ALL, "ru");

    // Исходная строка
    std::string str = "-1/2*        e^2*      F#_1^1{-1}*     F_1_   2{1}*           I{2  ^-4}*K  5^-1{2}";

    std::string cleanedString = removeHyphensAndSpaces(str);
    
    std::vector<std::string> search_patterns   = {"(d_\\{[\\w-]+\\}\\{[\\w-]+\\})",
                                                  "2"};

    std::vector<std::string> LaTeX_expressions = {"{\\delta}^8_{argument2}\\left(argument1\\right)",
                                                  "2"};

    std::vector<std::string> warnings          = {"дельта функции",
                                                  "2" };

    std::cout << "Исходная строка:\n" << str << "\n";
    std::cout << "Очищенная строка:\n" << cleanedString << "\n";

    return 0;
}
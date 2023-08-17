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
    
    std::vector<std::string> search_patterns   = {"\"(e\\^)\"\"([+-]?\\d+)\"",
                                                  "\"(d_\\{)\"\"([\\w-]+)\"\"(\\}\\{)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(F_)\"\"([\\w-]+)\"\"([\\^_]+[\\w-]+\\{)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(F#_)\"\"([\\w-]+)\"\"([\\^_][\\w-]+\\{)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(F1_)\"\"([\\w-]+)\"\"([\\^_][\\w-]+\\{)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(V_)\"\"([\\w-]+)\"\"([\\^_]+[\\w-]+\\{)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(I\\{)\"\"([\\w-]+)\"\"([\\^]+)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(I\\{)\"\"([\\w-]+)\"\"([_]+)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(K2[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(K3[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(K4[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)\"\"([\\w-]+)\"\"(\\})\"",
                                                  "\"(K5[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)\"\"([\\w-]+)\"\"(\\})\""};

    std::vector<std::string> LaTeX_expressions = {"{e}^{argument1}",
                                                  "{\\delta}^8_{argument2}\\left(argument1\\right)",
                                                  "{\\phi}_{argument1}\\left(argument2\\right)",
                                                  "{\\phi}^{*}_{argument1}\\left(argument2\\right)",
                                                  "\\tilde{\\phi}_{argument1}\\left(argument2\\right)",
                                                  "{V}_{argument1}\\left(argument2\\right)",
                                                  "{argument1}^{argument2}",
                                                  "{argument1}_{argument2}",
                                                  "\\frac{1}{{argument1}^2+M^2}",
                                                  "\\frac{M}{{argument1}^2+M^2}",
                                                  "\\frac{{\\xi}_0}{K_{argument1}}",
                                                  "\\frac{1}{R_{argument1}}"};

    std::vector<std::string> warnings          = {"Констант связи",
                                                  "Дельта функции",
                                                  "Киральных полей",
                                                  "Сопряжённых киральных полей",
                                                  "Киральных полей с тильдой",
                                                  "Вещественных полей",
                                                  "Степенных констант",
                                                  "Констант с нижним индексом",
                                                  "Констант типа 2",
                                                  "Констант типа 3",
                                                  "Констант типа 4",
                                                  "Констант типа 5"};

    std::cout << "Исходная строка:\n" << str << "\n";
    std::cout << "Очищенная строка:\n" << cleanedString << "\n";

    return 0;
}
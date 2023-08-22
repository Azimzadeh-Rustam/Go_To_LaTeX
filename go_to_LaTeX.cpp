#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// Удаление пробелов и переносов в строке
std::string removeHyphensAndSpaces(const std::string& input)
{
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) {
        return std::isspace(c);
        }), result.end());
    return result;
}

// Основной алгоритм для замены выражений
std::string go_to_LaTeX(std::string input) {

    input = removeHyphensAndSpaces(input);

    // Регулярные выражения для поиска
    std::vector<std::string> search_patterns = {
        "(e\\^)([+-]?\\d+)",
        "(d_\\{)([\\w-]+)(\\}\\{)([\\w-]+)(\\})",
        "(F_)([\\w-]+)([\\^_]+[\\w-]+\\{)([\\w-]+)(\\})",
        "(F#_)([\\w-]+)([\\^_][\\w-]+\\{)([\\w-]+)(\\})",
        "(F1_)([\\w-]+)([\\^_][\\w-]+\\{)([\\w-]+)(\\})",
        "(V_)([\\w-]+)([\\^_]+[\\w-]+\\{)([\\w-]+)(\\})",
        "(I\\{)([\\w-]+)([\\^]+)([\\w-]+)(\\})",
        "(I\\{)([\\w-]+)([_]+)([\\w-]+)(\\})",
        "(K2[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)([\\w-]+)(\\})",
        "(K3[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)([\\w-]+)(\\})",
        "(K4[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)([\\w-]+)(\\})",
        "(K5[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)([\\w-]+)(\\})"
    };

    // Шаблоны LaTeX
    std::vector<std::string> LaTeX_sample = {
        "{e}^{argument1}",
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
        "\\frac{1}{R_{argument1}}"
    };

    // Предупреждения
    std::vector<std::string> warnings = {
        "Констант связи",
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
        "Констант типа 5"
    };

    std::cmatch result;
    std::regex regular(search_patterns[11]);

    if (std::regex_search(input.c_str(), result, regular)) {
        for (int i = 0; i < result.size(); i++) {
            std::cout << result[i] << std::endl;
        }
    }

    return "$" + input + "$";
}

int main()
{
    setlocale(LC_ALL, "ru");

    // Исходная строка
    std::string str = "-1/2*e^2*F#_1^1{-1}*F_1_2{1}*I{2^-4}*K5^-1{2}";

    std::cout << go_to_LaTeX(str) << std::endl;

    return 0;
}
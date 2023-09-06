#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>

using namespace std;

std::string Remove_Hyphens_And_Spaces(const std::string& input)
{
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) {
        return std::isspace(c);
        }), result.end());
    return result;
}

// Функция для декодирования числа в аналитическое представление
std::string Decode_Number(std::string str) {

    if (str == "") {
        return "";
    }

    int num = std::stoi(str);

    if (num == 1 || num == -1) {
        return "0";
    }

    // Кодирование переменных и их значений в виде пар (переменная, код)
    std::map<char, int> variableToCode = {
        {'k', 2},
        {'l', 3},
        {'p', 5}
    };

    // Разложение числа на простые множители
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

    // Формирование аналитического представления в виде строки
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

void Go_To_LaTeX(std::string str)
{

    str = Remove_Hyphens_And_Spaces(str);

    // Регулярные выражения для поиска
    std::vector<std::string> search_patterns = {
        "\\*",
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
    std::vector<std::string> LaTeX_samples = {
        "\\cdot",
        "{e}^{argument1}",
        "{\\delta}^8_{argument2}\\left(argument1\\right)",
        "{\\phi}_{argument1}\\left(argument2,\\theta\\right)",
        "{\\phi}^{*}_{argument1}\\left(argument2,\\theta\\right)",
        "\\tilde{\\phi}_{argument1}\\left(argument2,\\theta\\right)",
        "{V}_{argument1}\\left(argument2\\right)",
        "{argument1}^{argument2}",
        "{argument1}_{argument2}",
        "\\frac{1}{{argument1}^2+M^2}",
        "\\frac{M}{{argument1}^2+M^2}",
        "\\frac{{\\xi}_0}{K_{argument1}}",
        "\\frac{1}{R_{argument1}}"
    };

    // Названия
    std::vector<std::string> designations = {
        "Умножений",
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

    std::vector<std::string> warnings; // Вектор для хранения предупреждений
    int count = 0; // Счетчик замен 

    for (int i = 0; i < search_patterns.size(); i++) {
        std::vector<std::string> expressions; // Вектор для хранения найденных выражений
        std::vector<std::string> LaTeX_expressions; //Вектор для хранения найденных выражений в формате LaTeX
        size_t pos = 0;
        std::cmatch result; // Эта переменная после выполнения кода будет представлять из себя массив, нулевой элемент которого будет самим найденным выражением, а все остальные - соответствовать разбиениям в регулярном выражении
        std::regex regular(search_patterns[i]); //Регулярное выражение
        std::sregex_iterator regex_iterator(str.begin(), str.end(), regular); // Итератор для поска всех заданных выражений

        // Перебор всех заданных выражений и добавьте их в вектор expressions
        std::sregex_iterator end_iterator;
        while (regex_iterator != end_iterator) {
            expressions.push_back(regex_iterator->str());
            regex_iterator++;
        }

        if (expressions.size() > 0) {
            std::sort(expressions.begin(), expressions.end()); // Сортируем вектор expressions, чтобы группировать повторяющиеся элементы
            expressions.erase(std::unique(expressions.begin(), expressions.end()), expressions.end()); // Удаляем дубликаты

            // Создание LaTeX синтаксиса для заданных выражений и запись их в вектор LaTeX_expressions
            for (int j = 0; j < expressions.size(); j++) {

                if (std::regex_search(expressions[j].c_str(), result, regular)) {
                    std::string LaTeX_template = LaTeX_samples[i];
                    // Массив, элементы которого представляют из себя аргументы в шаблоне LaTeX которые нужно заменить
                    std::string params[]{ "argument1", "argument2" };

                    std::string argument1;
                    std::string argument2;
                    if (i == 1 || i == 3 || i == 4 || i == 5 || i == 6) {
                        argument1 = result[2];
                        argument2 = Decode_Number(result[4]);
                    }
                    else {
                        argument1 = Decode_Number(result[2]);
                        argument2 = result[4];
                    }
                    // Массив, элементы которого представляют из себя значения на которые нужно заменить аргументы в шаблоне LaTeX
                    std::string new_params[]{ argument1, argument2 };

                    // Подстановка значений в соответствующие аргументы в шаблоне LaTeX
                    for (int k = 0; k < std::size(params); k++) {
                        pos = 0;
                        while ((pos = LaTeX_template.find(params[k], pos)) != std::string::npos) {
                            LaTeX_template.replace(pos, params[k].length(), new_params[k]);
                            pos += new_params[k].length();
                        }
                    }
                    // Запись полученных LaTeX выражений в вектор LaTeX_expressions
                    LaTeX_expressions.push_back(LaTeX_template);
                }
            }

            // Замена всех заданных выражений на формат LaTeX в исходной строке
            for (int n = 0; n < std::size(expressions); n++) {
                pos = 0;
                while ((pos = str.find(expressions[n], pos)) != std::string::npos) {
                    str.replace(pos, expressions[n].length(), LaTeX_expressions[n]);
                    pos += LaTeX_expressions[n].length();
                    count++;
                }
            }
        }
        else {
            warnings.push_back(designations[i]);
        }
    }

    // Вывод преобразованного кода
    std::cout << "Формат LaTeX:\n" << "$" + str + "$\n" << std::endl;

    //Вывод количества замен
    std::cout << "Количество замен: " << count << "\n" << std::endl;

    // Вывод предупреждений
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

    // Исходная строка
    std::string programFormat = "-1/2*e^-2*F#_1^1{-1}*F_1_2{1}*I{2^-4}*K5^-1{2}";

    Go_To_LaTeX(programFormat);

    return 0;
}
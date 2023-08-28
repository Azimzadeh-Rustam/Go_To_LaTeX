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
void go_to_LaTeX(std::string str) {

    str = removeHyphensAndSpaces(str);

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
    std::vector<std::string> LaTeX_samples = {
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
    std::vector<std::string> designations = {
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

    for (int i = 0; i < search_patterns.size(); i++)
    {
        std::vector<std::string> expressions; // Вектор для хранения найденных выражений
        std::vector<std::string> LaTeX_expressions; //Вектор для хранения найденных выражений в формате LaTeX
        size_t pos = 0;
        std::cmatch result; // Эта переменная после выполнения кода будет представлять из себя массив, нулевой элемент которого будет самим найденным выражением, а все остальные - соответствовать разбиениям в регулярном выражении
        std::regex regular(search_patterns[i]); //Регулярное выражение
        std::sregex_iterator regex_iterator(str.begin(), str.end(), regular); // Итератор для поска всех заданных выражений

        // Перебор всех заданных выражений и добавьте их в вектор expressions
        std::sregex_iterator end_iterator;
        while (regex_iterator != end_iterator)
        {
            expressions.push_back(regex_iterator->str());
            regex_iterator++;
        }

        if (expressions.size() > 0) {
            std::sort(expressions.begin(), expressions.end()); // Сортируем вектор expressions, чтобы группировать повторяющиеся элементы
            expressions.erase(std::unique(expressions.begin(), expressions.end()), expressions.end()); // Удаляем дубликаты

            // Создание LaTeX синтаксиса для заданных выражений и запись их в вектор LaTeX_expressions
            for (int j = 0; j < expressions.size(); j++)
            {

                if (std::regex_search(expressions[j].c_str(), result, regular))
                {
                    std::string LaTeX_template = LaTeX_samples[i];
                    // Массив, элементы которого представляют из себя аргументы в шаблоне LaTeX которые нужно заменить
                    std::string params[]{ "argument1", "argument2" };
                    // Массив, элементы которого представляют из себя значения на которые нужно заменить аргументы в шаблоне LaTeX
                    std::string new_params[]{ result[2], result[4] };

                    // Подстановка значений в соответствующие аргументы в шаблоне LaTeX
                    for (int k = 0; k < std::size(params); k++)
                    {
                        pos = 0;
                        while ((pos = LaTeX_template.find(params[k], pos)) != std::string::npos)
                        {
                            LaTeX_template.replace(pos, params[k].length(), new_params[k]);
                            pos += new_params[k].length();
                        }
                    }
                    // Запись полученных LaTeX выражений в вектор LaTeX_expressions
                    LaTeX_expressions.push_back(LaTeX_template);
                }
            }

            // Замена всех заданных выражений на формат LaTeX в исходной строке
            for (int n = 0; n < std::size(expressions); n++)
            {
                pos = 0;
                while ((pos = str.find(expressions[n], pos)) != std::string::npos)
                {
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
    std::cout << "Формат LaTeX:\n" << "$" + str + "$" << "\n" << std::endl;

    //Вывод количества замен
    std::cout << "Заменено выражений: " << count << "\n" << std::endl;

    // Вывод предупреждений
    int warningsSize = size(warnings);
    if (warningsSize > 0) {
        std::cout << "В исходном коде не было найдено:\n";
        for (int i = 0; i < warningsSize; i++) {
            std::cout << i + 1 << ". " << warnings[i] << std::endl;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru");

    // Исходная строка
    std::string programFormat = "-1/2*e^-2*F#_1^1{-1}*F_1_2{1}*I{2^-4}*K5^-1{2}";

    go_to_LaTeX(programFormat);

    return 0;
}
#include <iostream>
#include <regex>
#include <vector>

using namespace std;

// ОСНОВНОЙ АЛГОРИТМ, КОТОРЫЙ ЗАМЕНЯЕТ ФОРМАТ ВЫРАЖЕНИЙ В ИСХОДНОЙ СТРОКЕ
std::string change_format(std::string str, std::regex search_pattern, std::regex expression, std::string LaTeX_expression_general_new, std::string warning_new, int* pcount, std::vector<std::string>* pwarnings) {

    // Вектор для хранения всех заданных выражений, встречающихся в исходной строке
    std::vector<std::string> expressions,

    // Вектор для хранения этих же выражений в формате LaTeX
    LaTeX_expressions;

    size_t pos = 0;

    // Переменная expression_result после выполнения кода будет представлять из себя массив, нулевой элемент которого будет самим найденным выражением, а все остальные - соответствовать разбиениям в регулярном выражении
    std::cmatch expression_result;

    // Итератор для поска всех заданных выражений
    std::sregex_iterator regex_iterator(str.begin(), str.end(), search_pattern);

    // Перебор всех заданных выражений и добавьте их в вектор expressions
    std::sregex_iterator end_iterator;
    while (regex_iterator != end_iterator)
    {
        expressions.push_back(regex_iterator->str());
        regex_iterator++;
    }

    // Если в вектор expressions был записан хотя бы один элемент, то попадаем внутрь тела if
    if (std::size(expressions) > 0)
    {
        // Сортируем вектор expressions, чтобы группировать повторяющиеся элементы
        std::sort(expressions.begin(), expressions.end());

        // Удаляем дубликаты
        expressions.erase(std::unique(expressions.begin(), expressions.end()), expressions.end());

        // Создание LaTeX синтаксиса для заданных выражений и запись их в вектор LaTeX_expressions
        for (int i = 0; i < std::size(expressions); i++)
        {

            if (std::regex_search(expressions[i].c_str(), expression_result, expression))
            {
                std::string LaTeX_expression_general = LaTeX_expression_general_new;
                // Массив, элементы которого представляют из себя аргументы в шаблоне LaTeX которые нужно заменить
                std::string param_delta[]{ "argument1", "argument2" };
                // Массив, элементы которого представляют из себя значения на которые нужно заменить аргументы в шаблоне LaTeX
                std::string new_param_delta[]{ expression_result[2], expression_result[4] };

                // Подстановка значений в соответствующие аргументы в шаблоне LaTeX
                for (int i = 0; i < std::size(param_delta); i++)
                {
                    pos = 0;
                    while ((pos = LaTeX_expression_general.find(param_delta[i], pos)) != std::string::npos)
                    {
                        LaTeX_expression_general.replace(pos, param_delta[i].length(), new_param_delta[i]);
                        pos += new_param_delta[i].length();
                    }
                }
                // Запись полученных LaTeX выражений в вектор LaTeX_expressions
                LaTeX_expressions.push_back(LaTeX_expression_general);
            }
        }


        // Замена всех заданных выражений на формат LaTeX в исходной строке
        for (int i = 0; i < std::size(expressions); i++)
        {
            pos = 0;
            while ((pos = str.find(expressions[i], pos)) != std::string::npos)
            {
                str.replace(pos, expressions[i].length(), LaTeX_expressions[i]);
                pos += LaTeX_expressions[i].length();
                (*pcount)++;
            }
        }
    }
    else
    {
        std::string warning = warning_new;
        (*pwarnings).push_back(warning);
    }

    return str;
}

// ЗАМЕНА КОНСТАНТ СВЯЗИ
std::string coupling_constants_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    // Регулярное выражение для поиска констант связи
    std::regex search_pattern("(e\\^[+-]?\\d+)");

    // Разбиение регулярного выражения для поиска аргументов констант связи
    std::regex expression("(e\\^)"
        "([+-]?\\d+)"); // Степень (под индексом 2 в массиве expression_result)

    // Шаблон LaTeX для констант связи
    std::string LaTeX_expression_general_new = "{e}^{argument1}";

    // Данная строка будет записана в вектор warnings, если при сканировании строки не было найдено ни одной константы связи
    std::string warning_new = "Констант связи";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ЗАМЕНА УМНОЖЕНИЙ
std::string dots_to_LaTeX(std::string str) {
    std::regex search_pattern("(\\*)");
    std::string LaTeX_expression_general_new = "\\cdot";
    std::string result = std::regex_replace(str, search_pattern, LaTeX_expression_general_new);
    str = result;

    return str;
}

// ЗАМЕНА ДЕЛЬТА ФУНКЦИЙ
std::string deltas_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    // Регулярное выражение для поиска дельта функций | В некоторых местах дважды ставим косую черту, чтобы экранировать впереди стоящий символ
    std::regex search_pattern("(d_\\{[\\w-]+\\}\\{[\\w-]+\\})");

    // Разбиение регулярного выражения для поиска аргументов дельта функций
    std::regex expression("(d_\\{)"
        "([\\w-]+)" // Импульс - argument1 (под индексом 2 в массиве expression_result)
        "(\\}\\{)"
        "([\\w-]+)" // Точки - argument2 (под индексом 4 в массиве expression_result)
        "(\\})");

    // Шаблон LaTeX для дельта функции | В некоторых местах дважды ставим косую черту, чтобы экранировать впереди стоящую
    std::string LaTeX_expression_general_new = "{\\delta}^8_{argument2}\\left(argument1\\right)";

    std::string warning_new = "Дельта функций";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ЗАМЕНА СОПРЯЖЕННЫХ КИРАЛЬНЫХ ПОЛЕЙ
std::string phis_star_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    std::regex search_pattern("(F#_[\\w-]+[\\^_][\\w-]+\\{[\\w-]+\\})");

    std::regex expression("(F#_)"
        "([\\w-]+)"
        "([\\^_][\\w-]+\\{)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "{\\phi}^{*}_{argument1}\\left(argument2\\right)";

    std::string warning_new = "Сопряжённых киральных полей";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ЗАМЕНА ПОЛЕЙ
std::string phis_tilde_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    // Регулярное выражение для поиска дельта функций
    std::regex search_pattern("(F1_[\\w-]+[\\^_][\\w-]+\\{[\\w-]+\\})");

    std::regex expression("(F1_)"
        "([\\w-]+)"
        "([\\^_][\\w-]+\\{)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "\\tilde{\\phi}_{argument1}\\left(argument2\\right)";

    std::string warning_new = "Киральных полей с тильдой";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ЗАМЕНА ПОЛЕЙ
std::string phis_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    // Регулярное выражение для поиска дельта функций
    std::regex search_pattern("(F_[\\w-]+[\\^_]+[\\w-]+\\{[\\w-]+\\})");

    std::regex expression("(F_)"
        "([\\w-]+)"
        "([\\^_]+[\\w-]+\\{)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "{\\phi}_{argument1}\\left(argument2\\right)";

    std::string warning_new = "Киральных полей";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ЗАМЕНА ВЕЩЕСТВЕННЫХ ПОЛЕЙ
std::string V_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    // Регулярное выражение для поиска дельта функций
    std::regex search_pattern("(V_[\\w-]+[\\^_]+[\\w-]+\\{[\\w-]+\\})");

    std::regex expression("(V_)"
        "([\\w-]+)"
        "([\\^_]+[\\w-]+\\{)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "{V}_{argument1}\\left(argument2\\right)";

    std::string warning_new = "Вещественных полей";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

std::string constsUp_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    std::regex search_pattern("(I\\{[\\w-]+[\\^]+[\\w-]+\\})");

    std::regex expression("(I\\{)"
        "([\\w-]+)"
        "([\\^]+)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "{argument1}^{argument2}";

    std::string warning_new = "Степенных констант";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

std::string constsDown_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    std::regex search_pattern("(I\\{[\\w-]+[_]+[\\w-]+\\})");

    std::regex expression("(I\\{)"
        "([\\w-]+)"
        "([_]+)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "{argument1}_{argument2}";

    std::string warning_new = "Констант с нижним индексом";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ЗАМЕНА КОНСТАНТ ТИПА 5
std::string consts5_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    std::regex search_pattern("(K5[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{[\\w-]+\\})");

    std::regex expression("(K5[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "\\frac{1}{R_{argument1}}";

    std::string warning_new = "Констант типа 5";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ЗАМЕНА КОНСТАНТ ТИПА 4
std::string consts4_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    std::regex search_pattern("(K4[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{[\\w-]+\\})");

    std::regex expression("(K4[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "\\frac{{\\xi}_0}{K_{argument1}}";

    std::string warning_new = "Констант типа 4";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ЗАМЕНА КОНСТАНТ ТИПА 3
std::string consts3_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    std::regex search_pattern("(K3[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{[\\w-]+\\})");

    std::regex expression("(K3[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "\\frac{M}{{argument1}^2+M^2}";

    std::string warning_new = "Констант типа 3";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ЗАМЕНА КОНСТАНТ ТИПА 2
std::string consts2_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings)
{
    std::regex search_pattern("(K2[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{[\\w-]+\\})");

    std::regex expression("(K2[+-]?[\\w-]*(?:_[+-]?[\\w-]+)?(?:\\^[\\w-]+)?\\{)"
        "([\\w-]+)"
        "(\\})");

    std::string LaTeX_expression_general_new = "\\frac{1}{{argument1}^2+M^2}";

    std::string warning_new = "Констант типа 2";

    str = change_format(str, search_pattern, expression, LaTeX_expression_general_new, warning_new, pcount, pwarnings);

    return str;
}

// ОБЩАЯ ЗАМЕНА
std::string go_to_LaTeX(std::string str, int* pcount, std::vector<std::string>* pwarnings) {
    str = coupling_constants_to_LaTeX(str, pcount, pwarnings);
    str = dots_to_LaTeX(str);
    str = deltas_to_LaTeX(str, pcount, pwarnings);
    str = phis_star_to_LaTeX(str, pcount, pwarnings);
    str = phis_tilde_to_LaTeX(str, pcount, pwarnings);
    str = phis_to_LaTeX(str, pcount, pwarnings);
    str = V_to_LaTeX(str, pcount, pwarnings);
    str = constsUp_to_LaTeX(str, pcount, pwarnings);
    str = constsDown_to_LaTeX(str, pcount, pwarnings);
    str = consts5_to_LaTeX(str, pcount, pwarnings);
    str = consts4_to_LaTeX(str, pcount, pwarnings);
    str = consts3_to_LaTeX(str, pcount, pwarnings);
    str = consts2_to_LaTeX(str, pcount, pwarnings);
    return "$" + str + "$";
}

int main()
{
    setlocale(LC_ALL, "ru");

    // Исходная строка
    std::string str = "-1/2*e^2*F#_1^1{-1}*F_1_2{1}*I{2^-4}*K5^-1{2}";

    // Переменная, которая будет показывать количество произведённых замен
    int count = 0,
    *pcount = &count;

    // Вектор, в который будут записываться предупреждения
    std::vector<std::string> warnings,
    *pwarnings = &warnings;

    // Результат
    std::cout << "Формат программы:\n" << str << "\n\nФормат LaTeX:\n" << go_to_LaTeX(str, &count, &warnings) << "\n\nЗаменено выражений: " << count << std::endl;

    // Вывод предупреждений
    if (size(warnings) > 0) {
        std::cout << "\nВ исходном выражении не было найдено:\n";
        for (int i = 0; i < size(warnings); i++) {
            std::cout << i + 1 << ". " << warnings[i] << std::endl;
        }
    }

    return 0;
}
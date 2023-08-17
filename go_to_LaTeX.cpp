#include <iostream>
#include <regex>
#include <vector>

using namespace std;

int main()
{
    setlocale(LC_ALL, "ru");

    // Исходная строка
    std::string str = "-1/2*e^2*F#_1^1{-1}*F_1_2{1}*I{2^-4}*K5^-1{2}";

    /*
    std::vector<std::string> search_patterns =   {"(d_\\{[\\w-]+\\}\\{[\\w-]+\\})",                  "2"};

    std::vector<std::string> LaTeX_expressions = {"{\\delta}^8_{argument2}\\left(argument1\\right)", "2"};

    // Результат
    std::cout << str << std::endl;
    */
    return 0;
}
#include <iostream>

#include "enum_reflection.cpp"

enum Products { BANANA, ORANGE, APPLE };
enum { NUMBER_ONE, NUMBER_TWO };
enum class Numbers : unsigned short { One = 1, Two = 2, Three = 3, Eighteen = 18 };
enum struct Animals : int { Horse = -1, Elephant = 0, Dog = 5 };

int main() {
    std::cout << enumreflex<ORANGE>::constant_name << '\n';
    std::cout << enumreflex<ORANGE>::type_name << '\n';
    std::cout << enumreflex<ORANGE>::underlying_type_name << '\n';
    std::cout << enumreflex<ORANGE>::underlying_value << "\n\n";

    std::cout << enumreflex<NUMBER_TWO>::constant_name << '\n';
    std::cout << enumreflex<NUMBER_TWO>::type_name << '\n';
    std::cout << enumreflex<NUMBER_TWO>::underlying_type_name << '\n';
    std::cout << enumreflex<NUMBER_TWO>::underlying_value << "\n\n";

    std::cout << enumreflex<Numbers::Eighteen>::constant_name << '\n';
    std::cout << enumreflex<Numbers::Eighteen>::type_name << '\n';
    std::cout << enumreflex<Numbers::Eighteen>::underlying_type_name << '\n';
    std::cout << enumreflex<Numbers::Eighteen>::underlying_value << "\n\n";

    std::cout << enumreflex<Animals::Horse>::constant_name << '\n';
    std::cout << enumreflex<Animals::Horse>::type_name << '\n';
    std::cout << enumreflex<Animals::Horse>::underlying_type_name << '\n';
    std::cout << enumreflex<Animals::Horse>::underlying_value << "\n\n";
}
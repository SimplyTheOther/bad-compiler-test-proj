#include "stupidclasses.h"

namespace TestProgram {
    void StupidClass::update() {
        if (this->pointedClass != NULL) {
            std::cout << "pointed class ain't null" << std::endl;

            this->pointedClass->crappy_interface_method();
        }
    }

    void StupidClass::base_thing() {
        std::cout << "Not the base thing anymore!!!" << std::endl;
    }

    void AlternateExtdClass::base_thing() {
        std::cout << "Alternate base thing" << std::endl;
    }

    void PointedClass::crappy_interface_method() {
        std::cout << "Crappy interface thing does work" << std::endl;
    }

    bool comparison_function_1(int value_to_compare) {
        return value_to_compare == 0 || value_to_compare == 2 || value_to_compare < -5;
    }

    bool comparison_function_2(int value_to_compare) {
        return value_to_compare == 3 || value_to_compare == 9 || value_to_compare < -15;
    }

    bool comparison_function_3(int value_to_compare) {
        return value_to_compare == 7 || value_to_compare == 21 || value_to_compare > 12;
    }

    template <typename T>
    int HoldFunctionToPassToClass::method_with_function_param(int random_param, T function_param, std::string random_param_2, int random_param_3) {
        int length = 1;
        while (length < 66 && function_param(random_param)) {
            random_param++;
            some_random_state++;
        }
        return random_param + random_param_3;
    }

    void HoldFunctionToPassToClass::some_public_method_to_call(int value, int other_value) {
        std::string str;
        str.reserve(16);
        str += std::to_string(value);

        if (other_value > 3) {
            method_with_function_param(value, comparison_function_1, str, 69);
        } else if (other_value > 66) {
            method_with_function_param(value, comparison_function_2, str, 73);
        } else if (other_value > 128) {
            method_with_function_param(value, comparison_function_3, str, 2);
        } else {
            // do nothing
        }
    }
}
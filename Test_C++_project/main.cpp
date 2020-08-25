#include "functions.h"
#include "stupidclasses.h"
#include <iostream>

void test_bitwise_ops() {
    int KIND_CYMBAL = 1;
    int KIND_NOTE = 2;
    int KIND_SLANTED_DIAMOND = 10;
    //int KIND_TRIANGLE = 20;
    int KIND_EFFECT_CYMBAL = 40;

    int KIND_OPEN = 4;
    int KIND_CLOSED = 8;
    int KIND_CIRCLE_AROUND = 80;

    std::cout << "cross stick: " << (KIND_NOTE | KIND_CIRCLE_AROUND) << std::endl;

    std::cout << "closed hi-hat: " << (KIND_CYMBAL | KIND_CLOSED) << std::endl;

    std::cout << "open hi-hat: " << (KIND_CYMBAL | KIND_OPEN) << std::endl;

    std::cout << "china cymbal: " << (KIND_EFFECT_CYMBAL | KIND_CIRCLE_AROUND) << std::endl;

    std::cout << "ride bell: " << (KIND_SLANTED_DIAMOND) << std::endl;

    if (((KIND_EFFECT_CYMBAL | KIND_CIRCLE_AROUND) & KIND_CYMBAL) != 0) {
        std::cout << "painting non cymbal as cymbal 1" << std::endl;
    }

    if ((KIND_NOTE & KIND_CYMBAL) != 0) {
        std::cout << "painting non cymbal as cymbal 2" << std::endl;
    }

    if (((KIND_NOTE | KIND_CIRCLE_AROUND) & KIND_CYMBAL) != 0) {
        std::cout << "painting non cymbal as cymbal 3" << std::endl;
    }
}

void stupid_math_tests() {
    int a = 3;
    int b = 4;

    int c = TestProgram::Math::sum(a, b);

    std::cout << "Hello, world!" << std::endl;

    std::cout << c << std::endl;

    int d = TestProgram::Math::generic_sum(a, b);

    std::cout << "Next time trying a generic method:" << std::endl;

    std::cout << d << std::endl;

    float e = TestProgram::Math::generic_sum(2.4F, 3.7F);

    std::cout << e << std::endl;
}

void call_base_thing_on_class(TestProgram::BaseClass* baseClass) {
    baseClass->base_thing();
}

void oop_tests() {
    // creates stack instance of StupidClass
    TestProgram::StupidClass stupidClass;
    // creates stack instance of string
    std::string input;

    std::cout << "Enter Pause to stop." << std::endl;

    do {
        stupidClass.update();

        std::cin >> input;
    } while (input != "Pause");

    std::cout << "Program done." << std::endl;

    // polymorphism test
    TestProgram::StupidClass* stupidClassP = new TestProgram::StupidClass();

    std::cout << "About to call base thing on stupidclass" << std::endl;
    call_base_thing_on_class(stupidClassP);

    TestProgram::AlternateExtdClass* altClass = new TestProgram::AlternateExtdClass();

    std::cout << "About to call base thing on alt stupidclass" << std::endl;
    call_base_thing_on_class(altClass);

    delete stupidClassP;
    delete altClass;
}

void benchmark_stuff() {
    TestProgram::UnorderedStuffClass stuff;
    stuff.benchmark_all();
}

void test_function_template_passing() {
    TestProgram::HoldFunctionToPassToClass stuff;
    stuff.some_public_method_to_call(22, 33);
}

int main() {
    test_function_template_passing();

    //benchmark_stuff();

    //stupid_math_tests();

    //test_bitwise_ops();

    //oop_tests();

    return 0;
}
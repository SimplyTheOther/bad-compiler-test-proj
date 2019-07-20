#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

namespace TestProgram {

    class Math {
      public:
        static int sum(int a, int b);

        // template <typename T> static T generic_sum(T a, T b);
        template<typename T>
        static T generic_sum(T a, T b) {
            return a + b;
        }
    };
}
// int sum(int a, int b);

#endif
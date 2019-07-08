#ifndef STUPIDCLASSES_H_INCLUDED
#define STUPIDCLASSES_H_INCLUDED

#include <iostream>
#include <vector>

namespace TestProgram {
    class ICrappyInterface {
      public:
        virtual ~ICrappyInterface() {}
        virtual void crappy_interface_method() = 0;
    };

    class BaseClass {
      protected:
        int baseNumber;

      public:
        // constructor (not used)
        BaseClass() {}
        // destructor (may actually be required)
        virtual ~BaseClass() {}

        // a bit dodgy having it in here but eh
        virtual void base_thing() {
            std::cout << "Base method" << std::endl;
        }
    };

    class DumbClass {
      public:
        short dumbNo;

        // constructor
        DumbClass(short number) {
            dumbNo = number;
        }
    };

    class PointedClass : public ICrappyInterface {
      public:
        // constructor
        PointedClass() {
            std::cout << "Called pointed class constructor" << std::endl;
        }

        virtual void crappy_interface_method();
    };

    class StupidClass : public BaseClass {
        std::vector<TestProgram::DumbClass> dumbClasses;
        // something with a pointer
        TestProgram::PointedClass* pointedClass;

      public:
        // constructor
        StupidClass() {
            baseNumber = 3;

            TestProgram::DumbClass dC1(1);

            TestProgram::DumbClass dC2(3);

            dumbClasses.push_back(dC2);
            dumbClasses.push_back(dC1);

            pointedClass = new TestProgram::PointedClass();
        }

        // destructor
        ~StupidClass() {
            delete pointedClass;

            // don't need to delete vector because memory is automatically deleted
            // the issue with pointers is that they are deleted but the memory that
            // they point to is not deleted!!!
        }
        void update();

        // may have to do this? idk, dodgy
        virtual void base_thing();
    };

    class AlternateExtdClass : public BaseClass {
      public:
        AlternateExtdClass() {}

        ~AlternateExtdClass() {}

        virtual void base_thing();
    };
}

#endif
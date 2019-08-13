#ifndef STUPIDCLASSES_H_INCLUDED
#define STUPIDCLASSES_H_INCLUDED

#include <iostream>
#include <vector>

namespace TestProgram {
    // pure abstract base class?
    class ICrappyInterface {
      public:
        // abstract destructor
        virtual ~ICrappyInterface() {}

        // pure abstract method
        virtual void crappy_interface_method() = 0;
    };

    // base class that is not abstract
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

    // simple class
    class DumbClass {
      public:
        short dumbNo;

        // constructor (member assignment) - apparently the "non-preferred way"
        DumbClass(short number) {
            dumbNo = number;
        }
    };

    // class extending pure abstract base class
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
        // maybe it's only virtual if the enclosing superclass only has to be able to have it
        // overriden
        virtual void base_thing();
    };

    // alternate extension of base class for polymorphism
    class AlternateExtdClass : public BaseClass {
      public:
        AlternateExtdClass() {}

        ~AlternateExtdClass() {}

        virtual void base_thing();
    };

    // for demonstrating member initialisation list
    class MemberInitListClass {
        int randomNo;
        bool randomBool;
        DumbClass& dumbClassRef;

      public:
        /* member initialisation list constructor - required for references and encouraged for other
         * member variables */
        MemberInitListClass(int randomNum, int randomBool, DumbClass& dumbClassRef) :
          randomNo(randomNum), randomBool(randomBool), dumbClassRef(dumbClassRef) {}

        // const-suffixed function - means function cannot change member variable of its class
        DumbClass& view_dumb_class_ref() const {
            return dumbClassRef;
        }
    };
}

#endif
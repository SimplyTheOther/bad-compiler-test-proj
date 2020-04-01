#ifndef STUPIDCLASSES_H_INCLUDED
#define STUPIDCLASSES_H_INCLUDED

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
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

    // hash test for pair as set - combines strings and then finds hash of that
    struct pair_hash_combine {
        ::std::size_t operator()(const ::std::pair< ::std::string, ::std::string>& p) const {
            return ::std::hash< ::std::string>()(p.first + p.second);
        }
    };

    class UnorderedStuffClass {
        ::std::unordered_set< ::std::pair< ::std::string, ::std::string>, pair_hash_combine> test_set;

        ::std::unordered_map< ::std::string, ::std::unordered_set< ::std::string> > test_map;

        ::std::unordered_multimap< ::std::string, ::std::string> test_multimap;

      public:
        void add_stuff() {
            add_set();

            add_map();

            //add_multimap();
        }

        void search_stuff() {
            search_set();

            search_map();

            search_map_full();

            //search_multimap();
        }

        void benchmark_all() {
            add_stuff();

            search_stuff();
        }

        // seems to have better performance
        void add_set() {
            std::vector<long long> times;
            times.reserve(1000);

            for (int i = 0; i < 1000; i++) {
                auto t1 = std::chrono::high_resolution_clock::now();
                test_set.insert(::std::pair< ::std::string, ::std::string>(
                  ::std::string("wow:") + ::std::to_string(i),
                  ::std::string("wow 2:") + ::std::to_string(i)));
                auto t2 = std::chrono::high_resolution_clock::now();

                auto duration
                  = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

                times.push_back(duration);
            }

            long double avg = avg_without_overflow(times);
            std::cout << "Insert for set:      " << avg << std::endl;
        }

        // seems to have worse performance
        void add_map() {
            std::vector<long long> times;
            times.reserve(1000);

            for (int i = 0; i < 1000; i++) {
                auto t1 = std::chrono::high_resolution_clock::now();
                ::std::unordered_set< ::std::string> dummy_set;
                dummy_set.insert(::std::string("wow 2:") + ::std::to_string(i));
                test_map.insert(::std::pair< ::std::string, ::std::unordered_set< ::std::string> >(
                  ::std::string("wow:") + std::to_string(i), ::std::move(dummy_set)));
                auto t2 = std::chrono::high_resolution_clock::now();

                auto duration
                  = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

                times.push_back(duration);
            }

            long double avg = avg_without_overflow(times);
            std::cout << "Insert for map:      " << avg << std::endl;
        }

        void add_multimap() {
            std::vector<long long> times;
            times.reserve(1000);

            for (int i = 0; i < 1000; i++) {
                auto t1 = std::chrono::high_resolution_clock::now();
                test_multimap.insert(::std::pair< ::std::string, ::std::string>(
                  ::std::string("wow:") + ::std::to_string(i),
                  ::std::string("wow 2:") + ::std::to_string(i)));
                auto t2 = std::chrono::high_resolution_clock::now();

                auto duration
                  = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

                times.push_back(duration);
            }

            long double avg = avg_without_overflow(times);
            std::cout << "Insert for multimap: " << avg
                      << " (note doesn't add duplicates at this point)" << std::endl;
        }

        // seems to have worse performance than initial search for map, will see if it has worse than
        // both map searches.
        // for both map searches, it usually (but not always) has worse performance. this may be the
        // hash collisions or whatever occuring with this when it doesn't for the map.
        void search_set() {
            std::vector<long long> times;
            times.reserve(1000);

            for (int i = 0; i < 1000; i++) {
                auto t1 = std::chrono::high_resolution_clock::now();
                auto it = test_set.find(::std::pair< ::std::string, ::std::string>(
                  ::std::string("wow:") + ::std::to_string(i),
                  ::std::string("wow 2:") + ::std::to_string(i)));
                if (it == test_set.end())
                    std::cout << "ISSUE: COULD NOT FIND elem " << i << " in set!" << std::endl;
                auto t2 = std::chrono::high_resolution_clock::now();

                auto duration
                  = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

                times.push_back(duration);
            }

            long double avg = avg_without_overflow(times);
            std::cout << "Search for set:      " << avg << std::endl;
        }

        void search_map() {
            std::vector<long long> times;
            times.reserve(1000);

            for (int i = 0; i < 1000; i++) {
                auto t1 = std::chrono::high_resolution_clock::now();
                auto it = test_map.find((::std::string("wow:") + std::to_string(i)));
                if (it == test_map.end())
                    std::cout << "ISSUE: COULD NOT FIND elem " << i << " in map!" << std::endl;
                auto t2 = std::chrono::high_resolution_clock::now();

                auto duration
                  = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

                times.push_back(duration);
            }

            long double avg = avg_without_overflow(times);
            std::cout << "Search for map:      " << avg << std::endl;
        }

        void search_map_full() {
            std::vector<long long> times;
            times.reserve(1000);

            for (int i = 0; i < 1000; i++) {
                auto t1 = std::chrono::high_resolution_clock::now();
                auto it = test_map.find((::std::string("wow:") + std::to_string(i)));
                if (it == test_map.end()) {
                    std::cout << "ISSUE: COULD NOT FIND elem " << i
                              << " in map full search - discounting!" << std::endl;
                    break;
                }
                auto it2 = it->second.find(::std::string("wow 2:") + std::to_string(i));
                if (it2 == it->second.end()) {
                    std::cout << "ISSUE: COULD NOT FIND set item " << i
                              << " in map full search (2nd search) - discounting!" << std::endl;
                    break;
                }
                auto t2 = std::chrono::high_resolution_clock::now();

                auto duration
                  = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

                times.push_back(duration);
            }

            long double avg = avg_without_overflow(times);
            std::cout << "Search for map full: " << avg << std::endl;
        }

        // Multimap is too annoying to search properly (by key and value), so this should disqualify it.
        void search_multimap() {
            std::vector<long long> times;
            times.reserve(1000);

            for (int i = 0; i < 1000; i++) {
                auto t1 = std::chrono::high_resolution_clock::now();
                auto it = test_multimap.equal_range(::std::string("wow:") + ::std::to_string(i));
                if (it.first == test_multimap.end() && it.second == test_multimap.end()) {
                    std::cout << "ISSUE: COULD NOT FIND elem " << i << " in multimap - discounting!"
                              << std::endl;
                    break;
                }
                // check if any of these key-value pairs have correct value
                // i imagine this has shit performance, also ew lambda
                /*bool found = std::any_of(it.first, it.second, [i](decltype(test_multimap)::value_type const& p) {
                  return p.second == ::std::string("wow 2:") + std::to_string(i);
                });*/
                auto t2 = std::chrono::high_resolution_clock::now();

                auto duration
                  = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

                times.push_back(duration);
            }

            long double avg = avg_without_overflow(times);
            std::cout << "Search for multimap: " << avg << std::endl;
        }

        // Meant to average without overflow and with little rounding error
        long double avg_without_overflow(std::vector<long long>& vec) {
            int n = 0;
            long double mean = 0.0;
            for (auto x : vec) {
                long double delta = x - mean;
                mean += delta / ++n;
            }
            return mean;
        }
    };
}

#endif
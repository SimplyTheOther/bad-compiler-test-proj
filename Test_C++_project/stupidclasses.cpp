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
}
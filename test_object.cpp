#include "test_object.h"

void test_object::ProcessFlower() {
	
	this->flower = this->flower + 2;
	
}

bool test_object::FlowerPartyYall() {

	return touch.isPressed();

}

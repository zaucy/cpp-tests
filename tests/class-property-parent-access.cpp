#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <string>

class SomeClass;

std::vector<SomeClass*> someClasses;

struct SomeStruct {
	signed someStructSigned;
	unsigned someStructUnsigned;
	
	inline SomeClass* getOwner() const;
	
	inline void assertOwner(const SomeClass& maybeOwner, const std::string& message) const;
};

class SomeClass {
public:
	inline SomeClass();
	inline ~SomeClass();

	unsigned someUnsigned;
	signed someSigned;
	SomeStruct someStruct;
};

SomeClass::SomeClass() {
	someClasses.push_back(this);
}

SomeClass::~SomeClass() {
		auto it = std::find(someClasses.begin(), someClasses.end(), this);
		
		if(it != someClasses.end()) {
			someClasses.erase(it);
		}
	}

SomeClass* SomeStruct::getOwner() const {
	SomeClass* owner = nullptr;
	intptr_t thisPtrInt = reinterpret_cast<intptr_t>(this);
	
	for(SomeClass* someClass : someClasses) {
		intptr_t someClassPtrInt = reinterpret_cast<intptr_t>(someClass);
		
		if(thisPtrInt > someClassPtrInt && thisPtrInt < someClassPtrInt + sizeof(SomeClass)) {
			owner = reinterpret_cast<SomeClass*>(someClassPtrInt);
			break;
		}
	}
	
	return owner;
};

void SomeStruct::assertOwner(const SomeClass& maybeOwner, const std::string& message = "") const {
	if(&maybeOwner == getOwner()) {
		std::cout << "[SUCCESS] SomeClass == SomeClass.someStruct.getOwner()" << std::endl;
	} else {
		std::cout << "[FAILURE] SomeClass != SomeClass.someStruct.getOwner()" << std::endl;
	}
}

int main() {
	SomeClass someClassTest;
	SomeClass* someClassTestPtr = new SomeClass();
	
	someClassTest.someStruct.assertOwner(someClassTest);
	someClassTestPtr->someStruct.assertOwner(*someClassTestPtr);
	
	return 0;
}

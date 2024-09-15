#include <cstring>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <cstdarg>

#define MACRO(...) (std::cout __VA_OPT__(<<) __VA_ARGS__ << std::endl)
//#define eprintf(...) fprintf (stderr, __VA_ARGS__)
//#define match(x, ...) printf("%d, %d", x, __VA_ARGS__);
// #define match 10
#define TEST(...) __VA_OPT__(<<) __VA_ARGS__















//template<typename T>
class pattern {
public:
	template<typename T>
	pattern operator|(T value) {
		// std::cout << "type of pattern: " << typeid(this).name() << std::endl;
		// std::cout << "type of value: " << typeid(value).name() << std::endl;
//		this->value = value;
		std::cout << sizeof(value) << std::endl;
		pattern pat;
		unsigned int sizeOfData = sizeof(value);
//		pat.value = new T[sizeOfData];
		// if ( typeid(T).name() == typeid(int).name() )
		// 	pat.type = typeid(int).name();
		// else if ( typeid(T).name() == typeid(double).name() )
		// 	pat.type = typeid(double).name();
		// else if ( typeid(T).name() == typeid(bool).name() )
		// 	pat.type = typeid(bool).name();
		pat.type = typeid(T).name();
		
		pat.value = new T;
		new(pat.value) T(value);
		return pat;
	}

	template<typename Lambda>
	pattern operator>>=(Lambda lambda) {
		this->lambda = lambda;
		// this->value = value;
//		std::cout << "value " << *(int*)this->value << std::endl;
		return *this;
	}

	void operator()() {
// 		std::cout << "last" << std::endl;
	}
	
	template<typename T2, typename... Args>
	void operator()(T2 first, Args... args) {
		// std::cout << "this type " << typeid((*this).value).name() << std::endl;
		// std::cout << "first type " << typeid(first.value).name() << std::endl;
		// if ( this->value == first.value )
		// 	std::cout << "labmda: " << first.lambda() << std::endl;
		// else
		// 	std::cout << "lamnda hu9mbda" << std::endl;

		// if ( first.type == "int" )
		// 	std::cout << "type: " << typeid(*(int*)first.value).name() << std::endl;
		// else if ( first.type == "double" )
		// 	std::cout << "type: " << typeid(*(double*)first.value).name() << std::endl;
		// else if ( first.type == "bool" )
		// 	std::cout << "type: " << typeid(*(bool*)first.value).name() << std::endl;

//		std::cout << *(int*)this->value << std::endl;
		// std::cout << this->type << std::endl;
		// std::cout << first.type << std::endl;
		if ( this->type == first.type ) {
			const char* type = first.type;
			bool isValueEqual = false;
			if ( strcmp(type, "int") ) {
				if ( *(int*)this->value == *(int*)first.value )
					isValueEqual = true;
			} else if ( strcmp(type, "double") ) {
				if ( *(double*)this->value == *(double*)first.value )
					isValueEqual = true;
			} else if ( strcmp(type, "bool") ) {
				if ( *(bool*)this->value == *(bool*)first.value )
					isValueEqual = true;
			}

			if ( isValueEqual )
				std::cout << first.lambda() << std::endl;
		}
		
		operator()(args...);
	}

	int(*lambda)();
	// T value;
	const char* type = "";
	void* value = nullptr;
};

class match {
public:
	template<typename T>
	pattern operator()(T expression) {
		pattern pat;
		// if ( typeid(T).name() == typeid(int).name() )
		// 	pat.type = "int";
		// else if ( typeid(T).name() == typeid(double).name() )
		// 	pat.type = "double";
		// else if ( typeid(T).name() == typeid(bool).name() )
		// 	pat.type = "bool";
		pat.type = typeid(T).name();
		
		pat.value = new T;
		new((T*)pat.value) T(expression);

		// pat.value = expression;
		return pat;
	}
};

int main(int argc, char* argv[])
{
	pattern pattern;
	match match;
//	int(*ptr)() = [](){ return 10; };
 	match(true)(
		pattern | 10 >>= []{ return 10; },
		pattern | 10 >>= []{ return 200; },
		pattern | 10.0 >>= []{ return 3000; },
		pattern | true >>= []{ return 40000; }
		);

	return 0;
}

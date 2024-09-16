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

class pattern {
public:
	template<typename T>
	pattern operator|(T value) {
		std::cout << sizeof(value) << std::endl;
		pattern pat;
		unsigned int sizeOfData = sizeof(value);
		pat.type = typeid(T).name();
		
		pat.value = new T;
		new(pat.value) T(value);
		return pat;
	}

	template<typename Lambda>
	pattern operator>>=(Lambda lambda) {
		this->lambda = lambda;
		return *this;
	}

	void operator()() {
	}
	
	template<typename T2, typename... Args>
	void operator()(T2 first, Args... args) {
		if ( this->type == first.type ) {
			const char* type = first.type;
			bool isValueEqual = false;
			if ( !strcmp(type, "i") ) {
				if ( *(int*)this->value == *(int*)first.value )
					isValueEqual = true;
			} else if ( !strcmp(type, "d") ) {
				if ( *(double*)this->value == *(double*)first.value )
					isValueEqual = true;
			} else if ( !strcmp(type, "b") ) {
				if ( *(bool*)this->value == *(bool*)first.value )
					isValueEqual = true;
			}

			if ( isValueEqual )
				std::cout << first.lambda() << std::endl;
		}
		
		operator()(args...);
	}

	int(*lambda)();
	const char* type = "";
	void* value = nullptr;
};

template <typename T>
class pattern_custom {
public:
	template<typename T2>
	pattern_custom operator|(T2 value) {
		pattern_custom pat;
		pat.value = value;
		return pat;
	}

	template<typename Lambda>
	pattern_custom operator>>=(Lambda lambda) {
		this->lambda = lambda;
		return *this;
	}

	void operator()() {
	}
	
	template<typename T2, typename... Args>
	void operator()(T2 first, Args... args) {
		// if ( this->type == first.type ) {
		// 	const char* type = first.type;
		// 	bool isValueEqual = false;
		// 	if ( !strcmp(type, "i") ) {
		// 		if ( *(int*)this->value == *(int*)first.value )
		// 			isValueEqual = true;
		// 	} else if ( !strcmp(type, "d") ) {
		// 		if ( *(double*)this->value == *(double*)first.value )
		// 			isValueEqual = true;
		// 	} else if ( !strcmp(type, "b") ) {
		// 		if ( *(bool*)this->value == *(bool*)first.value )
		// 			isValueEqual = true;
		// 	}

		// 	if ( isValueEqual )
		// 		std::cout << first.lambda() << std::endl;
		// }
		// std::cout << "this value: " << this->value << std::endl;
		// std::cout << "first value: " << first.value << std::endl;

		if ( typeid(this->value) == typeid(first.value) ) {
			std::cout << "TEST" << std::endl;
			bool isEqual = false;
			if ( this->value == first.value )
				isEqual = true;

			if ( isEqual )
				std::cout << first.lambda() << std::endl;
		}
		
		operator()(args...);
	}

	int(*lambda)();
	T value;
};

class match {
public:
	// template<typename T>
	// pattern operator()(T expression) {
	// 	pattern pat;
	// 	pat.type = typeid(T).name();
		
	// 	pat.value = new T;
	// 	new((T*)pat.value) T(expression);

	// 	return pat;
	// }

	template<typename T>
	pattern_custom<T> operator()(T expression) {
		pattern_custom<T> pat;
		pat.value = expression;
		return pat;
	}
};

class Unit {
};

int main(int argc, char* argv[])
{
	pattern pattern;
	pattern_custom<bool> pattern_b;
	pattern_custom<int> pattern_i;
	pattern_custom<Unit> pattern_u;
	match match;
	Unit unit;
 	match(true)(
		pattern_i | 10   >>= []{ return 40000; },
		pattern_b | true >>= []{ return 40000; }
//		pattern_u | Unit() >>= []{ return 56000; }
		);

	return 0;
}

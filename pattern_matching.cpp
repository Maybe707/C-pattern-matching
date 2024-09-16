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

static unsigned int counter = 0;

class Unit {
public:
	Unit() = default;
	Unit(int field_) : field(field_) {}
	bool operator==(const Unit& unit) { if ( this->field == unit.field ) return true; else return false; }
	
	int field;
};

class pattern {
public:
	template<typename T>
	pattern& operator|(T value) {
		std::cout << sizeof(value) << std::endl;
		unsigned int typesNumber = this->typesNumber;
		this->type[typesNumber] = typeid(T).name();
		std::cout << "operator type: " << this->type[typesNumber] << std::endl;
		
		this->value[typesNumber] = new T;
		new(this->value[typesNumber]) T(value);
		++this->typesNumber;
		return *this;
	}

	template<typename Lambda>
	pattern& operator>>=(Lambda lambda) {
		this->lambda = lambda;
		return *this;
	}

	void operator()() {
	}
	
	template<typename T2, typename... Args>
	void operator()(T2 first, Args... args) {
		std::cout << "static govno: " << counter << std::endl;
		unsigned int typesNumber = first.typesNumber;
		for ( int i = counter; i < typesNumber; ++i ) {
			++counter;
			std::cout << "i: " << i << std::endl;
			std::cout << "first type before: " << first.type[i] << std::endl;
			if ( this->type[i] == first.type[i] ) {
				const char* type = first.type[i];
				std::cout << "this type: " << this->type[i] << std::endl;
				std::cout << "first type: " << type << std::endl;
				bool isValueEqual = false;
				if ( !strcmp(type, typeid(int).name() ) ) {
					if ( *(int*)this->value[i] == *(int*)first.value[i] )
						isValueEqual = true;
				} else if ( !strcmp(type, typeid(double).name() ) ) {
					if ( *(double*)this->value[i] == *(double*)first.value[i] )
						isValueEqual = true;
				} else if ( !strcmp(type, typeid(bool).name() ) ) {
					if ( *(bool*)this->value[i] == *(bool*)first.value[i] )
						isValueEqual = true;
				} else if ( !strcmp(type, typeid(Unit).name() ) ) {
					if ( *(Unit*)this->value[i] == *(Unit*)first.value[i] )
						isValueEqual = true;
				}

				if ( isValueEqual )
					std::cout << first.lambda() << std::endl;
			}
		}

		operator()(args...);
	}

	int(*lambda)();
	unsigned int typesNumber = 0;
	const char* type[10];
	void* value[10];
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
	pattern pat;
public:
	void operator()() {
	}

	template<typename T, typename ...Args>
	pattern operator()(T expression, Args... args) {
		unsigned int typesNumber = pat.typesNumber;
		pat.type[typesNumber] = typeid(T).name();
		
		pat.value[typesNumber] = new T;
		new((T*)pat.value[typesNumber]) T(expression);
		++pat.typesNumber;
		operator()(args...);
		return pat;
	}

	// template<typename T>
	// pattern_custom<T> operator()(T expression) {
	// 	pattern_custom<T> pat;
	// 	pat.value = expression;
	// 	return pat;
	// }
};

int main(int argc, char* argv[])
{
	pattern pattern;
	pattern_custom<bool> pattern_b;
	pattern_custom<int> pattern_i;
	pattern_custom<Unit> pattern_u;
	match match;
	Unit unit(10);
 	match(unit, 100, true)(
		pattern | Unit(10) | 100 | true >>= []{ return 40000; },
		pattern | true     >>= []{ return 5000; }
//		pattern_u | Unit() >>= []{ return 56000; }
		);

	return 0;
}

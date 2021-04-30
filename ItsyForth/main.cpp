#include <iostream>

#include "Runtime.hpp"
#include "Builder.hpp"

int main(int argc, const char * argv[]) {
	Runtime runtime;
	Builder dictionary(&runtime);
	
	DictionaryWord* interpreter = dictionary.findWord("interpret");
	runtime.execute(interpreter, interpreter);
	
	return 0;
}

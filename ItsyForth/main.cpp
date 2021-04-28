#include <iostream>

#include "Runtime.hpp"
#include "CoreDictionary.hpp"

int main(int argc, const char * argv[]) {
	Runtime runtime;
	CoreDictionary dictionary(&runtime);
	
	DictionaryWord* interpreter = dictionary.find("interpret");
	runtime.execute(interpreter, interpreter);
	
	return 0;
}

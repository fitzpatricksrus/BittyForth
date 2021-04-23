#include <iostream>

#include "Runtime.hpp"
#include "CoreDictionary.hpp"

int main(int argc, const char * argv[]) {
	Runtime runtime;
	runtime.reset();
	CoreDictionary dictionary(&runtime);
	dictionary.rebuildDictionary();
	
	DictionaryWord* interpreter = dictionary.find("interpret");
	runtime.execute(interpreter, interpreter);
	
	return 0;
}

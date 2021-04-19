#include <stddef.h>

#include "BaseMemory.hpp"

void BaseMemory::reset() {
	abortVector = 0;
	dictionaryPtr = (int)offsetof(BaseMemory, dictionaryData);
	numberBase = 10;
	tibPtr = (int)offsetof(BaseMemory, tib);
	nextInputPtr = tibPtr;
	lastWord = 0;
	compilerFlags = 0;
	tib[0] = 0;
}

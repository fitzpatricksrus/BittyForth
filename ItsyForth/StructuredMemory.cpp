/*

#include <stddef.h>
#include <cassert>

#include "StructuredMemory.hpp"

void StructuredMemory::reset() {
	abort();
	abortVector = 0;
}

IPtr StructuredMemory::abort() {
	dictionaryPtr = &dictionaryData;
	numberBase = 10;
	tibPtr = &tib;
	tibContentLength = 0;
	tibInputOffset = 0;
	lastWord = 0;
	compilerFlags = 0;
	tib[0] = 0;
	return abortVector;
}

Ptr StructuredMemory::allocate(int bytes) {
	Ptr result = dictionaryPtr;
	dictionaryPtr += bytes;
	return result;
}
*/
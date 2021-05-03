#ifndef StructuredMemory_hpp
#define StructuredMemory_hpp

#include "Types.hpp"

class DictionaryWord;

class StructuredMemory {
public:
	IPtr abortVector;
	Ptr dictionaryPtr;
	Num numberBase;
	char* tibPtr;
	Num tibContentLength;
	Num tibInputOffset;
	DictionaryWord* lastWord;
	Num compilerFlags;
	char tib[256];
	char dictionaryData[];
	
	void reset();
	DictionaryWord* abort();
	
	Ptr allocate(int bytes);
	
	template <typename T> T* append(T value);
			
private:
	StructuredMemory() {};
	~StructuredMemory() {};
};

template <typename T>
T* StructuredMemory::append(T value) {
	T* result = (T*)allocate(sizeof(value));
	*result = value;
	return result;
}


#endif /* StructuredMemory_hpp */

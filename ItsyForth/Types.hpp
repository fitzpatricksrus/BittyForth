#ifndef Types_hpp
#define Types_hpp


class DictionaryWord;

typedef long Num;
typedef DictionaryWord** IPtr;
typedef char* Ptr;

typedef union XData {
	char* ptr;
	char c;
	Num l;
	DictionaryWord* w;
	
	XData() {}
	XData(long lIn) { l = lIn; }
	XData(char* ptrIn) { ptr = ptrIn; }
	XData(DictionaryWord* wIn) { w = wIn; }
	operator long() { return l; }
	operator char*() { return ptr; }
	operator DictionaryWord*() { return w; }
	} XData;
	

#endif /* Types_hpp */

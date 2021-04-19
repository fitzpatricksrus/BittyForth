//
//  BaseMemory.hpp
//  ItsyForth
//
//  Created by Dad on 4/19/21.
//

#ifndef BaseMemory_hpp
#define BaseMemory_hpp

class BaseMemory {
public:
	int abortVector;
	int dictionaryPtr;
	int numberBase;
	int tibPtr;
	int nextInputPtr;
	int lastWord;
	int compilerFlags;
	char tib[256];
	char dictionaryData[];
	
	void reset();
	
private:
	BaseMemory() {};
	~BaseMemory() {};
};


#endif /* BaseMemory_hpp */

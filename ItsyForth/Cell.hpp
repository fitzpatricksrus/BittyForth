//
//  Cell.hpp
//  ItsyForth
//
//  Created by Dad on 5/1/21.
//

#ifndef Cell_hpp
#define Cell_hpp

#include "Instruction.hpp"

typedef union Cell {
	Instruction asInstruction;	//just here to ensure OpCodes fit in a cell.
	int asData;					//just here to ensure longs fit in a cell.
	
	operator int&() { return asData; };
	operator Instruction&() { return asInstruction; };
} Cell;

#endif /* Cell_hpp */

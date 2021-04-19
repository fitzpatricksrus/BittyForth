//
//  Builder.cpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#include "Builder.hpp"

#include "Runtime.hpp"

Builder::Builder(Runtime* runtimeIn)
:runtime(runtimeIn)
{
	runtime->setInt(0, 0);	//set dp to zero. It will get allocated over by "dp" declaration below
}


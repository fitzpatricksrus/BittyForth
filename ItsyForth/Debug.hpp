//
//  Debug.hpp
//  ItsyForth
//
//  Created by Dad on 4/28/21.
//

#ifndef Debug_hpp
#define Debug_hpp

#include <string>
#include <iostream>

class Debug {
public:
	static bool isOn;
	static void print(std::string msg) { if (isOn) std::cout << msg << std::endl; }
};

#endif /* Debug_hpp */

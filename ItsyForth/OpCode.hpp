#ifndef OpCode_hpp
#define OpCode_hpp

typedef enum OpCode {
	INVALID,
	DoColon,
	DoSemicolon,
	DoLit,
	DoConstant,
	DoVariable,
	Abort,
	Comma,
	Rot,
	Drop,
	Dup,
	Swap,
	Plus,
	Equals,
	At,
	Put,
	ZeroBranch,
	Branch,
	Execute,
	Exit,
	Count,
	ToNumber,
	Accept,
	Word,
	Emit,
	Find,
	Create,
	Constant,
} OpCode;

#endif /* OpCode_hpp */

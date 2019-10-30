//------------------------------------------
//ConcreteArguments.hpp  
//10/27/2019   created
//10/27/2019   modified
//10/27/2019   bu yangwenqi
//------------------------------------------
#include "Types.h"
#ifndef _CONCRETEARGUMENTS_H
#define _CONCRETEARGUMENTS_H
//------------------------------------------
namespace game
{
	//------------------------------------------
	//types of score.
    //------------------------------------------
	enum class ScoreType
	{
		ST_Increase=0,
		ST_Decrease,

	};
	//------------------------------------------
	class ScoreArg : public MsgArgument
	{
	public:
		ScoreType _score_type;
	private:

	};



}
//------------------------------------------
#endif // !_CONCRETEARGUMENTS_H

#pragma once

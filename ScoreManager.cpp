//------------------------------------------
//ScoreMgr.cpp   
//10/27/2019   created
//10/27/2019   modified
//10/27/2019   bu yangwenqi
//------------------------------------------

#include "ScoreManager.h"
#include "ConcreteArguments.hpp"


game::ScoreMgr::ScoreMgr()
{
}

game::ScoreMgr::~ScoreMgr()
{
}

void game::ScoreMgr::on_update(MessageType mt, std::shared_ptr<MsgArgument> args)
{
	if (mt!= MessageType::MT_SCORE)
		return;
	auto score_arg = std::static_pointer_cast<ScoreArg>(args);
	if (!score_arg)
		return;
	if (score_arg->_score_type == ScoreType::ST_Increase)
		_score += 100;
	else
		_score -= 100;
}




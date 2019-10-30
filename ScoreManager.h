//------------------------------------------
//ScoreMgr.h   
//10/27/2019   created
//10/27/2019   modified
//10/27/2019   bu yangwenqi
//------------------------------------------
#ifndef _SCOREMGR_H
#define _SCOREMGR_H

#include "IListener.h"
#include <memory>
//------------------------------------------
namespace game
{
	//------------------------------------------
	class ScoreMgr:public IListener
	{
	public:
		ScoreMgr();
		~ScoreMgr();
		//------------------------------------------
		// inherited from Ilistener
		//------------------------------------------

		virtual void on_update(
			MessageType mt,
			std::shared_ptr<MsgArgument> args) override;

		//------------------------------------------
	private:
		int               _score = 0;
	};

	
	//------------------------------------------
}
//------------------------------------------
#endif // !_SCOREMGR_H

#pragma once

//------------------------------------------
//IListener.h   
//10/27/2019   created
//10/27/2019   modified
//10/27/2019   bu yangwenqi
//------------------------------------------
//headers
//------------------------------------------
#include "Types.h"
#include <memory>
#ifndef _ILISTENER_H
#define _ILISTENER_H
//------------------------------------------
namespace game
{
	//------------------------------------------
	class IListener
	{
	public:
	
		virtual void on_update(
			MessageType mt,
			std::shared_ptr<MsgArgument> args)=0;
	private:

	};

 //------------------------------------------
}

//------------------------------------------
#endif // !_ILISTENER_H


#pragma once

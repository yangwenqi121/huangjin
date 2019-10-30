//------------------------------------------
//ISubject.h   
//10/27/2019   created
//10/27/2019   modified
//10/27/2019   bu yangwenqi
//------------------------------------------
#ifndef _ISUBJECT_H
#define _ISUBJECT_H
#include "IListener.h"
//------------------------------------------
namespace game
{
	//------------------------------------------
	class ISubject
	{
	public:
		
		virtual void add_listener(MessageType mt, IListener* listener)=0;
		virtual void remove_listener(MessageType mt, IListener* listener) = 0;
		virtual void notify(MessageType mt) = 0;
	private:

	};

	
	//------------------------------------------
}
//------------------------------------------

#endif // !_ISUBJECT_H


#pragma once

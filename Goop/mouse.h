#ifndef GUSANOS_MOUSE_H
#define GUSANOS_MOUSE_H

#ifdef DEDSERV
#error "Can't use this in dedicated server"
#endif //DEDSERV

#include <boost/signals2.hpp>

class MouseHandler
{
	struct StopEarly
	{
		typedef bool result_type;
	
		template<typename InputIterator>
		bool operator()(InputIterator first, InputIterator last) const
		{
			// Stop at the first slot returning false
			for(; first != last; ++first)
			{
				if(!*first)
					return false;
			}
			
			return true;
		}
	};
	
public:
	void init();
	void shutDown();
	void poll();
	static int getX();
	static int getY();

	boost::signals2::signal<bool (int), StopEarly> buttonDown;
	boost::signals2::signal<bool (int), StopEarly> buttonUp;
	boost::signals2::signal<bool (int, int), StopEarly> move;
	boost::signals2::signal<bool (int), StopEarly> scroll;
};

extern MouseHandler mouseHandler;

#endif //GUSANOS_MOUSE_H

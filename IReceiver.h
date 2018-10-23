#ifndef _IRECEIVER_H_
#define _IRECEIVER_H_

struct IReceiver 
{ 
	virtual void Receive(const char* data, unsigned int size) = 0; 
}; 

#endif

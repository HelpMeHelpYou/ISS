#ifndef _ICALLBACK_H_
#define _ICALLBACK_H_

struct ICallback
{
	virtual void BinaryPacket(const char* data, unsigned int size) = 0;
	virtual void TextPacket(const char* data, unsigned int size) = 0;
};

#endif //_ICALBACK_H_

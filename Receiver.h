#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include <vector>
#include <cstddef>
#include <cstdint>

#include "IReceiver.h"
#include "ICallback.h"


class Receiver : public IReceiver
{
public:
	Receiver(ICallback * callback );
	void Receive (const char * data, unsigned int size) override;

    static const size_t headerSize;
    static const uint8_t binPacketIndentifier;
    static const char * terminateSeq ;
    static const size_t terminateSeqSz ;

private:
	std::vector <unsigned char> _acc;
	static constexpr size_t _averageSize = 1024;
	ICallback * _callback;
	size_t _awaitedSize;
	bool _stringDetected;

    char getPacketData (const char * data, size_t offset, size_t index);
};
#endif //_RECEIVER_H_



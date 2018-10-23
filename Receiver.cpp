#include "Receiver.h"

#include <algorithm>

 const size_t Receiver::headerSize = 5;
 const uint8_t Receiver::binPacketIndentifier = 0x22;
 const char * Receiver::terminateSeq   = "\t\n\t\n";
 const size_t Receiver::terminateSeqSz = 4;



Receiver::Receiver(ICallback * callback) : _callback(callback)
{
    _acc.reserve(_averageSize);
}

char str[] = "/n/t/n/t";

char Receiver::getPacketData (const char* data, size_t offset, size_t index)
{
    return offset ? data[offset+index] : _acc.size() > index ? _acc[index] : data[index-_acc.size()];
}

void Receiver::Receive(const char* data, unsigned int sz)
{
    size_t offset = 0;
    size_t availableBytes = _acc.size()+sz;

    while (offset < sz)
    {
        //begin of packet check packet type
        if (static_cast<unsigned int> (getPacketData(data,offset,0)) == 0x22)
        {
            //binaray packet
            //try to get packet size
            if (offset + headerSize <= availableBytes)
            {
                _awaitedSize = 	static_cast<unsigned int> (getPacketData(data,offset,1+0))+
                        static_cast<unsigned int> (getPacketData(data,offset,1+1))*(1<<8)+
                        static_cast<unsigned int> (getPacketData(data,offset,1+2))*(1<<16)+
                        static_cast<unsigned int> (getPacketData(data,offset,1+3))*(1<<24);

                //can we extract packet?
                if (_awaitedSize+offset+headerSize <= availableBytes)
                {
                    const char * payloadPtr;

                    // can we pass data without copy to _acc?
                    size_t newOffset =offset + _awaitedSize+headerSize - _acc.size();
                    if(_acc.size())
                    {
                        _acc.insert(_acc.end(), data+offset,data+offset+_awaitedSize+headerSize-_acc.size());

                        payloadPtr = reinterpret_cast<const char *>(_acc.data()) + headerSize;
                    }
                    else
                    {
                        payloadPtr = data+offset+headerSize;
                    }
                    _callback->BinaryPacket(payloadPtr, _awaitedSize);
                    offset = newOffset;

                    _acc.clear();
                    _awaitedSize = 0;

                    continue;
                }
            }


        }
        else
        {
            size_t i =0;
            for (i = _acc.size() > terminateSeqSz ? _acc.size()-terminateSeqSz : 0; i < std::max(_acc.size()+sz-offset,terminateSeqSz); i++)
            {
                size_t k;
                for ( k = 0; k < terminateSeqSz ; k++)
                {
                    if ( getPacketData(data,offset,i+k) != terminateSeq[k])
                    {
                        break;
                    }
                }

                        if (k == terminateSeqSz)
                        {
                            size_t newOffset =offset + _awaitedSize+headerSize - _acc.size();
                            const char * payloadPtr;
                            if(_acc.size())
                            {
                                _acc.insert(_acc.end(), data+offset,data+offset+i+terminateSeqSz-_acc.size());
                                payloadPtr = reinterpret_cast<const char *>(_acc.data());
                            }
                            else
                            {
                                payloadPtr = data+offset;
                            }
                            _callback->TextPacket(payloadPtr,i);
                            offset = offset + i + terminateSeqSz-_acc.size();
                            _acc.clear();
                            goto next;
                        }


            }
        }
        _acc.insert(_acc.end(), data+offset, data+sz);
        break;
next:
        continue;

    }
}

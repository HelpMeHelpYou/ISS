#include <gmock/gmock.h>

#include "ICallback.h"


class MockCallback : public ICallback
{
public:
	MOCK_METHOD2(BinaryPacket, void(const char*, unsigned int));		
	MOCK_METHOD2(TextPacket, void(const char*, unsigned int));
};

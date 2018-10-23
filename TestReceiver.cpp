#include "Receiver.h"
#include "MockCallback.h"

class TestReceiverFixture : public ::testing::Test
{
public:	
	TestReceiverFixture () : callback(), receiver(&callback)
	{

	}

    static std::vector<unsigned char> constructBinPacket(size_t sz)
    {
        std::vector<unsigned char> result;

        result.push_back(0x22);

        result.push_back(sz%256);
        result.push_back((sz%(256*256))/256);
        result.push_back((sz%(256*256*256))/(256*256));
        result.push_back( sz/(256*256*256));

        for ( size_t i = 0; i < sz ; i++)
            result.push_back(i%256);
        return result;
    }

    static std::vector<unsigned char> constructTextPacket(size_t sz)
    {
         std::vector<unsigned char> result;
         for (int i = 0; i < sz; i++)
         result.push_back( (i%10)+'0' );

         for(int i = 0 ; i < 4; i++ )
         result.push_back( Receiver::terminateSeq[i] );

         return result;
    }

	MockCallback callback;
	Receiver receiver;	
};



TEST_F(TestReceiverFixture, ShouldParseSimpleBinPacket)
{
    std::vector<unsigned char> data = constructBinPacket(1);
	
	EXPECT_CALL(callback,BinaryPacket(::testing::_,1));
	
	receiver.Receive(reinterpret_cast<char* > (data.data()), data.size());

}


TEST_F(TestReceiverFixture, ShouldParseEmptyBinPacket)
{
    std::vector<unsigned char> data = constructBinPacket(0);

    EXPECT_CALL(callback,BinaryPacket(::testing::_,0));

    receiver.Receive(reinterpret_cast<char* > (data.data()), data.size());

}

TEST_F(TestReceiverFixture, ShouldParse4SimpleBinPacket)
{
    std::vector<unsigned char> data = constructBinPacket(1);

    for (int i = 0; i < 2 ; i++)
        data.insert(data.end(),data.begin(),data.end());
    EXPECT_CALL(callback,BinaryPacket(::testing::_,1)).Times(4);

    receiver.Receive(reinterpret_cast<char* > (data.data()), data.size());

}


TEST_F(TestReceiverFixture, ShouldParse4EmptyBinPacket)
{
    std::vector<unsigned char> data = constructBinPacket(0);

    for (int i = 0; i < 2 ; i++)
        data.insert(data.end(),data.begin(),data.end());
    EXPECT_CALL(callback,BinaryPacket(::testing::_,0)).Times(4);

    receiver.Receive(reinterpret_cast<char* > (data.data()), data.size());

}



TEST_F(TestReceiverFixture, ShouldParseSimpleBinPacketByOne)
{
    std::vector<unsigned char> data = constructBinPacket(1);;

    for (int i = 0; i < 2 ; i++)
        data.insert(data.end(),data.begin(),data.end());
    EXPECT_CALL(callback,BinaryPacket(::testing::_,1)).Times(4);

    for (int i = 0; i < data.size(); i++)
    receiver.Receive(reinterpret_cast<char* > (&data[i]), 1);

}

TEST_F(TestReceiverFixture, ShouldParseSimpleTxtPacket)
{
    std::vector<unsigned char> data = constructTextPacket(1);

    EXPECT_CALL(callback,TextPacket(::testing::_,1));

    receiver.Receive(reinterpret_cast<char* > (data.data()), data.size());

}


TEST_F(TestReceiverFixture, ShouldParseEmptyTxtPacket)
{
    std::vector<unsigned char> data = constructTextPacket(1);

    EXPECT_CALL(callback,TextPacket(::testing::_,1));

    receiver.Receive(reinterpret_cast<char* > (data.data()), data.size());

}



TEST_F(TestReceiverFixture, ShouldParse4SimpleTextPacket)
{
    std::vector<unsigned char> data = constructTextPacket(1);

    for (int i = 0; i < 2 ; i++)
        data.insert(data.end(),data.begin(),data.end());
    EXPECT_CALL(callback,TextPacket(::testing::_,1)).Times(4);

    receiver.Receive(reinterpret_cast<char* > (data.data()), data.size());

}


TEST_F(TestReceiverFixture, ShouldParse4EmptyTextPacket)
{
    std::vector<unsigned char> data = constructTextPacket(0);

    for (int i = 0; i < 2 ; i++)
        data.insert(data.end(),data.begin(),data.end());
    EXPECT_CALL(callback,TextPacket(::testing::_,0)).Times(4);

    receiver.Receive(reinterpret_cast<char* > (data.data()), data.size());
}

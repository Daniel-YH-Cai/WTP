//
// Created by daniel-cai on 4/4/22.
//

#include "PacketHeader.h"
#include "crc32.h"

const int START=0;
class Packet{
    //const int for type
    PacketHeader header;
    const char data[1024]={0};
    int packet_size;
public:
    Packet(){
        header={4,0,0,0};
        packet_size=1024;
    }
    //construct a data packet
    Packet(const char* data);
    //create a ack packet
    Packet(int seqNum);
    //start
    static Packet StartPacket(int initialSeq);
    //end
    static Packet EndPacket();
    //check sum
    bool checkSum();
    int type();
    //how to serialize?

};

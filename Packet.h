//
// Created by daniel-cai on 4/4/22.
//

#include "PacketHeader.h"
#include "crc32.h"
#include <string>
#define DATA_SIZE 1024
#define START 0;
class Packet{
    //const int for type
    PacketHeader header;
    const char data[1024]={0};

    int length;
public:
    Packet(){
        header={4,0,0,0};
        //size of payload of data packet
        length=0;
    }
    //construct a data packet
    Packet(const char* data,int seqNum);
    //create an ack packet
    Packet(int seqNum);
    //create a start packet
    static Packet StartPacket(int initialSeq);
    //create an end packet
    static Packet EndPacket();

    //create a packet from raw bytes received from socket
    static void deserialize(Packet* p);
    //serialize the packet into bytes and store them in buffer
    int serialize(char* buffer);
    //check sum
    bool checkSum();

    //getters
    std::string get_type();
    unsigned int get_seqNum(){
        return header.seqNum;
    }
    unsigned int get_length(){
        return length;
    }
    unsigned int get_checksum(){
        return header.checksum;
    }

    //check if a ack packet is valid
    bool isValidACK(){
        if(checkSum()&&get_type()=="ACK"&&get_length()==0){
            return true;
        }
        return false;
    }

    void reset(){
        header={4,0,0,0};
        //size of payload of data packet
        length=0;
    }
};

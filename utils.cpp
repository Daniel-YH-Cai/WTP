#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include "crc32.h"
#include "PacketHeader.h"

class Header{
    unsigned int type=-1;     // 0: START; 1: END; 2: DATA; 3: ACK
    unsigned int seqNum=-1;   // Described below
    unsigned int length=-1;   // Length of data; 0 for ACK packets
    unsigned int checksum=-1; // 32-bit CRC
public:
    Header();
    Header(unsigned int type, unsigned int seqNum, unsigned int length, unsigned int checksum) : type(type),seqNum(seqNum),length(length),checksum(checksum) {}
};
class UDPPacket{
    Header h;
    char* payload= nullptr;
    int size=-1;
public:
    UDPPacket(Header h){
        this->h=h;
    }
    static void dataPacket();
    static void startPacket();
    static void ackPacket();
    bool checkSum();
};
class Address{
    sockaddr_in addr{};
public:
    Address(uint16_t port){
        sockaddr_in addr;
        memset(&addr,0,sizeof (sockaddr_in));
        addr.sin_family=AF_INET;
        addr.sin_port= htons(port);
        addr.sin_addr.s_addr= htonl(INADDR_ANY);
        this->addr=addr;
    }
    Address(uint16_t port, char* host){
        sockaddr_in addr;
        memset(&addr,0,sizeof (sockaddr_in));
        addr.sin_family=AF_INET;
        struct hostent* sp= gethostbyname(host);
        memcpy(&addr.sin_addr,sp->h_addr,sp->h_length);
        this->addr=addr;
    }
    sockaddr_in getStruct(){
        return addr;
    }
};
class UDPSocket{
    Address addr_me;
    Address destination;
    int fd;
public:
    UDPSocket(Address destination){
        this->addr_me=Address(8888);
        this->fd= socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        bind(this->fd,addr_me.getStruct(),sizeof(addr_me.getStruct()));
        this->destination=destination;
    }
    void send(char* buff,int size){
        sendto(fd,buff,size,0, destination.getStruct(), sizeof(destination.getStruct()));
    }
    void sendPacket(UDPPacket p){

    }
    void recv(char buff[], int size) {
        recvfrom(fd,buff,size,0, nullptr, nullptr);
    }
    void recvPacket(UDPPacket p){

    }

};

int main(){

    return 0;
}
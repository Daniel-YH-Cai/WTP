#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include "Packet.h"

class UDPSocket{
    struct sockaddr_in si_other;
    struct sockaddr_in si_me;
    socklen_t len_other;
    int fd;
public:
    UDPSocket(){
        memset(&si_other,0,addrLen);
        memset(&si_me,0,addrLen);
        len_other=addrLen;
        fd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    static const socklen_t addrLen=sizeof(sockaddr_in);
    void bind_me(int port){
        si_me.sin_family = AF_INET;
        si_me.sin_port = htons(port);
        si_me.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(fd , (struct sockaddr*)&si_me, sizeof(si_me) );
    }
    void set_other(int port,const char* server){
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(port);
        struct hostent* sp = gethostbyname(server);
        memcpy(&si_other.sin_addr, sp->h_addr, sp->h_length);
    }
    int send(char* message){
        std::cout<<"Sending to host: "<<get_other_addr()<<" port: "<<get_other_port()<<"\n";
        return sendto(fd, message, strlen(message) , 0 , (struct sockaddr *) &si_other, addrLen);
    }
    int receive(char* message,int buff_size){
        int bytes= recvfrom(fd, message, buff_size, 0, (struct sockaddr *) &si_other, &len_other);
        std::cout<<"Received from host: "<<get_other_addr()<<" port: "<<get_other_port()<<"\n";
        return bytes;
    }
    int get_other_port(){
        return ntohs(si_other.sin_port);
    }
    const char* get_other_addr(){
        return inet_ntoa(si_other.sin_addr);
    }
    void sendPacket(Packet p);
    //bool: false if time out
    bool receivePacket(Packet p);
    ~UDPSocket(){
        close(fd);
    }
};


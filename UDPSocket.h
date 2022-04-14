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

class UDPSocket
{
    int timeout = 10;
    struct sockaddr_in si_other;
    struct sockaddr_in si_me;
    socklen_t len_other;
    int fd;

public:
    UDPSocket()
    {
        memset(&si_other, 0, addrLen);
        memset(&si_me, 0, addrLen);
        len_other = addrLen;
        fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    static const socklen_t addrLen = sizeof(sockaddr_in);
    // bind to a port; should only be used in wReceiver
    void bind_me(int port)
    {
        si_me.sin_family = AF_INET;
        si_me.sin_port = htons(port);
        si_me.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(fd, (struct sockaddr *)&si_me, sizeof(si_me));
    }
    // set the host to which the packets will be sent to
    // should only be used in wSender
    void set_other(int port, const char *server)
    {
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(port);
        struct hostent *sp = gethostbyname(server);
        memcpy(&si_other.sin_addr, sp->h_addr, sp->h_length);
    }
    // sent message to host;
    // after receiving, si_other will automatically be set to the sender
    // so set_other is not necessary while sending acks in wReceiver
    void send(const char *message, int buffer_size)
    {
        std::cout << "Sending to host: " << get_other_addr() << " port: " << get_other_port() << "\n";
        // this use strlen is error ,must use number
        int n = sendto(fd, message, buffer_size, 0, (struct sockaddr *)&si_other, addrLen);
        while (n != buffer_size)
        {
            message += n;
            buffer_size -= n;
            return sendto(fd, message, buffer_size, 0, (struct sockaddr *)&si_other, addrLen);
        }
    }
    void receive(char *message, int buff_size)
    {
        std::cout << "Received from host: " << get_other_addr() << " port: " << get_other_port() << "\n";
        int n = recvfrom(fd, message, buff_size, 0, (struct sockaddr *)&si_other, &len_other);
        while (n != buff_size)
        {
            message += n;
            buff_size -= n;
            n = recvfrom(fd, message, buff_size, 0, (struct sockaddr *)&si_other, &len_other);
        }
    }

    // receiving  a maximum of buff_size bytes into message in 500ms
    // return -1 if do not receive any data in this period
    // else return the amount of data received
    int receiveTimeout(char *message, int buff_size)
    {
        int result = 0;
        bool getSomeData = false;
        int total_received = 0;
        for (int i = 0; i < 20; i++)
        {
            result = recvfrom(fd, message + total_received, buff_size - total_received, MSG_DONTWAIT, (struct sockaddr *)&si_other, &len_other);
            std::cout << "Received: " << result << "\n";
            if (result == -1)
                ;
            else
            {
                getSomeData = true;
                total_received = total_received + result;
                if (total_received = buff_size)
                {
                    break;
                }
            }
            // unit:milliseconds
            usleep(25000);
        }
        if (!getSomeData)
        {
            return -1;
        }
        std::cout << "Received: " << total_received << "\n";
        return total_received;
    }
    int get_other_port()
    {
        return ntohs(si_other.sin_port);
    }
    const char *get_other_addr()
    {
        return inet_ntoa(si_other.sin_addr);
    }

    void sendPacket(Packet p)
    {
        int buffer_size = 16 + p.get_length(); // header + data
        char *buffer = (char *)malloc(sizeof(char) * (16 + p.length));
        p.serialize(p, buffer);
        this->send(buffer, buffer_size);
    }

    // bool: false if time out;
    bool receivePacket(Packet *p)
    {
        char buffer[1472] = {0};
        this.receive(buffer, 16);
        Packet::deserializeHeader(p, buffer);
        this->receive(buffer, p->header.length);
        Packet::deserializeBody(p, buffer);
        return true;
    }
    ~UDPSocket()
    {
        close(fd);
    }
};

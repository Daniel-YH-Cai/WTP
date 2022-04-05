#include "UDPSocket.h"
#include <iostream>
#include <fstream>
#define MAX_START_ACK 2048
using namespace std;

class BatchSender{
    UDPSocket* s;
    int length;
    ifstream file;
    ofstream logfile;
public:
    static const int chunk_size=DATA_SIZE;
    BatchSender(const char* filename,const char* log){
        s= nullptr;
        logfile=ofstream(log);
        file=ifstream (filename,ios::binary);
        file.seekg(0,ios::end);
        length= file.tellg();
        file.seekg(0,ios::beg);
    };
    //send the index th chunck of the file
    void set_UDPSocket(UDPSocket* sock){
        cout<<"Socket set!\n";
        s=sock;
    }
    void send_packet(int index,int seqNumber){
        if(chunk_size*index>length){
            cout<<"Invalid index!\n";
        }
        else{
            char buffer[1024]={0};
            file.seekg(index*chunk_size,ios::beg);
            file.read(buffer,chunk_size);
            Packet p(buffer,seqNumber);
            s->sendPacket(p);
            logfile<<p.get_type()<<" "<<p.get_seqNum()
            <<" "<<p.get_length()<<" "<<p.get_checksum()<<"\n";
        }
    }

    //return the new sequence number after sending the packets
    //can send less then window_size packets
    int send_window(int seqNumber, int index,int window_size){
        if(chunk_size*index>length){
            cout<<"Invalid index!\n";
            return seqNumber;
        }
        file.seekg(index*chunk_size,ios::beg);
        char buffer[1024]={0};
        for(int i=0;i<window_size;i++){
            if((index+i)*chunk_size<=length){
                file.read(buffer,chunk_size);
                Packet p(buffer,seqNumber);
                s->sendPacket(p);
                seqNumber++;
                logfile<<p.get_type()<<" "<<p.get_seqNum()
                       <<" "<<p.get_length()<<" "<<p.get_checksum()<<"\n";
            }
        }
        return seqNumber;
    }
    bool finished();
    ~BatchSender(){
        file.close();
    }

};

//TODO: finish while loop
int main(int argc, char* argv[]){
    if(argc!=6){
        cout<<"Usage: ./wSender\n";
        return 0;
    }
    int port= atoi(argv[2]);
    int window_size= atoi(argv[3]);
    UDPSocket udp;
    udp.set_other(port,argv[1]);
    //send start packet
    int initial_seq=random()%MAX_START_ACK;
    Packet start=Packet::StartPacket(initial_seq);
    Packet response;
    while(true){
        udp.sendPacket(start);
        if(udp.receivePacket(&response)){
            if(response.isValidACK()&&response.get_seqNum()==initial_seq){
                break;
            }
        }
        response.reset();
    }
    BatchSender bsender(argv[4],argv[5]);
    int cur_seq=0;
    bsender.set_UDPSocket(&udp);
    while(!bsender.finished()){
        bsender.send_window(cur_seq,cur_seq,window_size);
        udp.receivePacket(&response);
    }
    return 0;
}
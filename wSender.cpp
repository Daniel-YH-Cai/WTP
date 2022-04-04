#include "UDPSocket.h"
#include <iostream>
using namespace std;
//for loop create packet and send
//Array to keep pointer to all packets
//while(have_document_left){
//    for i in window size
//        send_packets
//        seq+=window_size
//    recv_ack
//    if time_out
//        resend
//    else
//        increment ack
//
//}
int main(){
    UDPSocket sender;
    sender.set_other(8888,"127.0.0.1");
    cout<<"Start Sending\n";
    char buffer[256]={0};
    sprintf(buffer,"hello_message");
    sender.send(buffer);
    cout<<"message sent\n";
    //receive a reply and print it
    //clear the buffer by filling null, it might have previously received data
    memset(buffer,'\0', 256);
    //try to receive some data, this is a blocking call
    sleep(3);
    sender.receive(buffer,256);
    cout<<buffer;
    return 0;
}
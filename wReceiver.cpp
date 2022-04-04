#include "UDPSocket.h"
#include <iostream>

using namespace std;
int main(){
    UDPSocket receiver;
    receiver.bind_me(8888);
    cout<<"Start receicing\n";
    while(true)
    {
        printf("Waiting for data on port %d ...", 8888);
        fflush(stdout);

        //try to receive some data, this is a blocking call
        char buffer[256]={0};

        receiver.receive(buffer,256);
        cout<<"Data:\n"<<buffer;
        cout<<"\nSending the data back";
        receiver.send(buffer);
    }
    return 0;
}
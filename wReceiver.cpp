#include "UDPSocket.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class BatchReceiver
{
    UDPSocket *s;
    // windows base: the first chunk in the windows;
    // the next packet to be send and to wait for ack
    string file;
    ofstream logfile;
    int window_size;
    Packet **window;
    int fileCount;
    // the number of packets already sent and are waiting for ack

public:
    BatchReceiver(int ws, const char *filename, const char *log)
    {
        s = nullptr;
        window_size = ws;
        window = (Packet **)malloc(ws * sizeof(Packet *));
        for (int i = 0; i < ws; i++)
        {
            window[i] = NULL;
        }
        logfile = ofstream(log);
        file = filename;
        file.seekg(0, ios::end);
        // length = file.tellg();
        file.seekg(0, ios::beg);
        fileCount = 0;
    }

    void set_UDPSocket(UDPSocket *sock)
    {
        cout << "Socket set!\n";
        s = sock;
    }

    void writeFile(ofstream file, BatchReceiver receiver)
    {
        file.write(receiver.window[0].data, receiver.window[0].get_length());
        for (int i = 0; i < receiver.window_size - 1; i++)
        {
            receiver.window[i] = receiver.window[i + 1];
        }
        receiver.window[receiver.window_size - 1] = NULL;
    }

}

int
main()
{
    if (argc != 5)
    {
        cout << "Usage: ./wSender\n";
        return 0;
    }
    int port = atoi(argv[1]);
    int window_size = atoi(argv[2]);
    UDPSocket udp;
    udp.bind_me(port);
    // receiver.bind_me(8888);
    cout << "Start receicing\n";
    BatchReceiver receiver = new BatchReceiver(atoi(argv[2]), argv[3], argv[4]);
    receiver.set_UDPSocket(udp);

    while (true)
    {
        Packet p = new Packet();
        receiver.s->receivePacket(&p);
        logfile << p.get_type() << " " << p.get_seqNum()
                << " " << p.get_length() << " " << p.get_checksum() << "\n";
        if (p.get_type() == 0)
        {
            //ack for start
            Packet newP = new Packet(p.get_seqNum());
            s->sendPacket(newP);
            ofstream file = ofstream(receiver.file + "/FILE-" + to_string(receiver.fileCount++) + "out");
            int num = 0;
            int max = 0;

            while (true)
            {
                Packet pData = new Packet();
                receiver.s->receivePacket(&pData);
                logfile << pData.get_type() << " " << pData.get_seqNum()
                        << " " << pData.get_length() << " " << pData.get_checksum() << "\n";
                // For each packet received
                if (pData.get_type() == 1 && pData.get_seqNum() == p.get_seqNum())
                {
                    //ack for end
                    Packet newP = new Packet(pData.get_seqNum());
                    s->sendPacket(newP);
                    break;
                }
                if (pData.checkSum())
                {
                    if (pData.get_seqNum() == num)
                    {
                        // if (pData.get_seqNum() > max)
                        // {
                        //     max = pData.get_seqNum();
                        // }

                        receiver.window[0] = &pData;
                        while (receiver.window[0] != NULL)
                        {
                            receiver.writeFile(file, receiver);
                            num += 1;
                        }
                        Packet newP = new Packet(num);
                        s->sendPacket(newP);
                    }
                    else
                    {
                        if (pData.get_seqNum() > num && pData.get_seqNum() <= num + receiver.window_size)
                        {
                            receiver.window[pData.get_seqNum() - num] = &pData;
                        }
                        Packet newP = new Packet(num);
                        s->sendPacket(newP);
                    }
                }
            }
        }
    }
    return 0;
}

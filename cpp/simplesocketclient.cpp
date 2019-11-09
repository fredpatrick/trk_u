
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include "simplesocketclient.h"
#include "packetbuffer.h"
#include "event_device_error.h"

trk::SimpleSocketClient::
SimpleSocketClient()
{
}

trk::SimpleSocketClient::
~SimpleSocketClient()
{
}

void
trk::SimpleSocketClient::
connect(const std::string& host_ip_addr, int portno)
{
    socket_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "SimpleSocketClient.connect, socket_fd = " << socket_fd_ << std::endl;
    struct sockaddr_in     serv_addr;
    ::memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host_ip_addr.c_str());
    serv_addr.sin_port        = htons(portno);

    int ie = ::connect(socket_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr) );
    std::cout << "SimpleSocketClient.connect-connected, ie = " << ie << std::endl;
}

int
trk::SimpleSocketClient::
write(PacketBuffer* pbfr)
{
    int bfrlen = pbfr->bfrlen();
    char* bfr  = pbfr->bfr();
                                          // This solely for the purpose of recording
    std::string tag = bfr;                //  kind of packet is still being written
    std::cout << "SocketServer.write, tag = " << tag << std::endl;

    int nl = ::write( socket_fd_, &bfrlen, sizeof(int) );
    if ( nl != sizeof(int) ) {
        std::ostringstream ost;
        ost << "SocketServer:write, error writing bfrlen, ns = " << nl;
        throw event_device_error(ost.str() );
    }
    int ns = ::write( socket_fd_, bfr, bfrlen);
    if ( ns != bfrlen) {
        std::ostringstream ost;
        ost << "SocketServer:write, error writing bfr, ns = " << ns;
        throw event_device_error(ost.str() );
    }
    return ns;
}

trk::PacketBuffer*
trk::SimpleSocketClient::
read()
{
    int bfrlen;
    int nl;
    nl = ::read(socket_fd_, &bfrlen, sizeof(int));
    if ( nl == 0 )  {
        std::ostringstream ost;
        ost << "SocketServer:read, socket_fd = " << socket_fd_ << 
                                     ", error reading bfrlen, end of file";
        throw event_device_error(ost.str() );
    }
    char* bfr = new char[bfrlen];
    int ns = ::read( socket_fd_, bfr, bfrlen);
    if ( ns != bfrlen ) {
        std::ostringstream ost;
        ost << "SocketServer:read, error reading bfr, bfrlen = " << bfrlen << ", ns = " << ns;
        throw event_device_error(ost.str() );
    }

    std::string tag = bfr;
    PacketBuffer* pbfr = new PacketBuffer(bfrlen, bfr); 
    delete[] bfr;
    return pbfr;
}

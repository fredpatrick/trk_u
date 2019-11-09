#ifndef TRK__SIMPLESOCKETCLIENT_HH
#define TRK__SIMPLESOCKETCLIENT_HH

#include "eventdevice.h"

namespace trk
{
    class PacketBuffer;

    class SimpleSocketClient : public EventDevice
    {
        public:
            SimpleSocketClient();
            ~SimpleSocketClient();

            void                connect(const std::string& host_ip_addr, 
                                        int                portno);
            int                 write(PacketBuffer* pbfr);

            PacketBuffer*       read();
        private:
            int                 socket_fd_;
    };

}


#endif

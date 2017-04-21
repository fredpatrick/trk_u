#ifndef TRK_TRKUTL_HH
#define TRK_TRKUTL_HH

#define DEBUG_SCKT 0

#include "debugcntl.h"
#include <iostream>
#include <string>

namespace trk
{

enum SW_DIRECTION {THRU, OUT, NOVAL };
enum TRK_STATE { IDLE, BUSY };
enum BLK_STATE { GO, STOP};

struct SWKey {
    int             num; 
    SW_DIRECTION    swd;

    bool
    operator< (const SWKey& k2) const
    {
        int a = num * 2 + static_cast<int>(swd);;
        int b = k2.num * 2 + static_cast<int>(k2.swd);;

        if ( a < b) return true;
        else        return false;
    }

    bool
    operator== (const SWKey& k2) const
    {
        if ( num == k2.num && swd == k2.swd ) return true;
        else                                  return false;
    }
};

std::string dattime();

std::string get_yesno(const std::string& question);

SWKey      get_switch();

int         make_socket_server(int portno);

std::ostream&
operator<<( std::ostream& ostrm, const trk::SWKey& kwy);

std::ostream&
operator<<( std::ostream& ostrm, const trk::SW_DIRECTION& idirec);

std::ostream&
operator<<( std::ostream& ostrm, const trk::TRK_STATE& trkstate);

std::ostream&
operator<<( std::ostream& ostrm, const trk::BLK_STATE& blkstate);

trk::DebugCntl&
operator<<( trk::DebugCntl& dbg, const trk::SW_DIRECTION& idirec);

trk::DebugCntl&
operator<<( trk::DebugCntl& dbg, const trk::TRK_STATE& trkstate);

trk::DebugCntl&
operator<<( trk::DebugCntl& dbg, const trk::BLK_STATE& blkstate);

SW_DIRECTION to_swstate(int i);
}

std::istream&
operator>>( std::istream& is, trk::SW_DIRECTION& idirec);

#endif

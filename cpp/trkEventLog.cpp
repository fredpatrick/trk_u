/*
 * ============================================================================
 *                   The XyloComp Software License, Version 1.1
 * ============================================================================
 * 
 *    Copyright (C) 2016 XyloComp Inc. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modifica-
 * tion, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of  source code must  retain the above copyright  notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. The end-user documentation included with the redistribution, if any, must
 *    include  the following  acknowledgment:  "This product includes  software
 *    developed  by  XyloComp Inc.  (http://www.xylocomp.com/)." Alternately, 
 *    this  acknowledgment may  appear in the software itself,  if
 *    and wherever such third-party acknowledgments normally appear.
 * 
 * 4. The name "XyloComp" must not be used to endorse  or promote  products 
 *    derived  from this  software without  prior written permission. 
 *    For written permission, please contact fred.patrick@xylocomp.com.
 * 
 * 5. Products  derived from this software may not  be called "XyloComp", 
 *    nor may "XyloComp" appear  in their name,  without prior written 
 *    permission  of Fred Patrick
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS  FOR A PARTICULAR  PURPOSE ARE  DISCLAIMED.  IN NO  EVENT SHALL
 * XYLOCOMP INC. OR ITS CONTRIBUTORS  BE LIABLE FOR  ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLU-
 * DING, BUT NOT LIMITED TO, PROCUREMENT  OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR  PROFITS; OR BUSINESS  INTERRUPTION)  HOWEVER CAUSED AND ON
 * ANY  THEORY OF LIABILITY,  WHETHER  IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING  NEGLIGENCE OR  OTHERWISE) ARISING IN  ANY WAY OUT OF THE  USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "trkutl.h"
#include "JobClock.h"
#include "CmdPacket.h"
#include "EventFactory.h"
#include "InputEvent.h"
#include "BreakEvent.h"
#include "TrackEvent.h"
#include "SocketClient.h"
#include "Zones.h"
#include "trkutl.h"

using namespace trk;

int main() {

    std::cout << "Display TRK event state as matrix, zone_name vs time" << std::endl;
    static pthread_mutex_t write_event_; 
    JobClock* job_clock = trk::JobClock::instance();
    std::cout << *job_clock << std::endl;

    SocketClient* esc    = new SocketClient("192.168.1.167", 17303);
    SocketClient* cmd_fd = new SocketClient("192.168.1.167", 17300);
    int                 ns = 6;
    std::pair<int, int> item;
    CmdPacket           cp("set",
                           "switch",
                           ns);
    for ( int i = 0; i < ns; i++) {
        item.first = i;
        item.second = THRU;
        cp.item(i, item);
    }
    cp.write(cmd_fd);


    Zones*  zones = new Zones();


    EventFactory* event_factory = EventFactory::instance();
    InputEvent*   event;

    std::cout << "trkEventLog: Entering event loop" << std::endl;
    bool done = false;
    std::cout << "  Time  | O72A   | O72B  | O72C  |" << std::endl;
    while ( !done ) {
        InputEvent* event = event_factory->get_next_event(esc);
        if ( event == 0 ) {
            std::cout << "trkEventLog: Error getting next event" << std::endl;
            done = true;
            break;
        } else {
            //event->print(0);
            if ( event->tag() == "BRK" ) break;

            if ( event->tag() == "TRK" ) {
                TrackEvent* evt = dynamic_cast<TrackEvent*>(event);
                zones->active_zones(evt);
                LAYOUT_STATE current_state = zones->current_state(evt);
                zones->print_log_entry(evt, current_state);
            }
        }
    }

    return 1;
}

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
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "trkutl.h"
#include "JobClock.h"
#include "EventFactory.h"
#include "InputEvent.h"
#include "BreakEvent.h"
#include "BlockEvent.h"
#include "SwitchEvent.h"
#include "TrackEvent.h"
#include "EventPipe.h"
#include "EventSocketServer.h"
#include "trkutl.h"

using namespace trk;
pthread_mutex_t write_event_ = PTHREAD_MUTEX_INITIALIZER;
void*
make_events(void* efd)
{
    std::cout << "make_events-begin" << std::endl;
    JobClock* job_clock = JobClock::instance();
    int n = 10;
    while ( n > 0 ) {
        double      tm_event  = job_clock->job_time();
        std::string blk_name  = "O72C";
        BLK_STATE   blk_state = STOP;
        BlockEvent* blk_event = new BlockEvent(tm_event,
                                               blk_name,
                                               blk_state);
        std::cout << "make_events-1" << std::endl;
        int ier = pthread_mutex_lock(&write_event_);
        if ( ier != 0 ) std::cout << "BlockEvent.event, couldn't lock mutex, ier = " <<
                                         ier << std::endl;
        blk_event->write_event(static_cast<EventDevice*>(efd) );
        ier = pthread_mutex_unlock(&write_event_);
        if ( ier != 0 ) std::cout << "BlockEvent.event, couldn't unlock mutex, ier = " <<
                                    ier << std::endl;
        std::cout << "make_events-2" << std::endl;
        blk_event->print(50);
        sleep(2);
        delete blk_event;
                     tm_event = job_clock->job_time();
        int          sw_num   = 4;
        SW_DIRECTION sw_direc = OUT;
        SwitchEvent* sw_event = new SwitchEvent(tm_event,
                                                sw_num,
                                                sw_direc);
        ier = pthread_mutex_lock(&write_event_);
        if ( ier != 0 ) std::cout << "SwitchEvent.event, couldn't lock mutex, ier = " <<
                                         ier << std::endl;
        sw_event->write_event( static_cast<EventDevice*>(efd) );
        ier = pthread_mutex_unlock(&write_event_);
        if ( ier != 0 ) std::cout << "SwitchEvent.event, couldn't unlock mutex, ier = " <<
                                    ier << std::endl;
        sw_event->print(50);
        sleep(2);
        delete sw_event;

                      tm_event  = job_clock->job_time();
        std::string   zone_name = "O60A";
        TRK_STATE     trk_state = IDLE;
        TrackEvent*   trk_event = new TrackEvent(tm_event,
                                                 zone_name,
                                                 trk_state);
        ier = pthread_mutex_lock(&write_event_);
        if ( ier != 0 ) std::cout << "TrackEvent.event, couldn't lock mutex, ier = " <<
                                         ier << std::endl;
        trk_event->write_event( static_cast<EventDevice*>(efd) );
        ier = pthread_mutex_unlock(&write_event_);
        if ( ier != 0 ) std::cout << "TrackEvent.event, couldn't unlock mutex, ier = " <<
                                    ier << std::endl;
        sw_event->print(50);
        sleep(2);
        delete trk_event;
        
        n = n - 1;
    }
    double tm_event = job_clock->job_time();
    BreakEvent* brk_event = new BreakEvent(tm_event);
    brk_event->write_event( static_cast<EventDevice*>(efd) );

    return NULL;
}

int main() {

    std::cout << "BeagleBoneBlack driver for exercising trk cicuits" <<std::endl;
    static pthread_mutex_t write_event_; 
    JobClock* job_clock = trk::JobClock::instance();
    std::cout << *job_clock << std::endl;

    
    int n_event = 0;            // when sensors write to sensor_fds[1], n_event is
                                // incremented, when code trkDriver readfs from sensor_fds[0]
                                // n_event is decremented until pipe is empty.

    EventPipe* efd = new EventPipe();
    EventSocketServer* ess = new EventSocketServer(17303);

    ::pthread_t thread;
    if ( ::pthread_create(&thread, NULL,
                          &make_events,
                          static_cast<void*>(efd) ) ) {
        ::perror(" Couldnt create pthread" );
        return 1;
    }

    EventFactory* event_factory = EventFactory::instance();
    InputEvent*   event;

    std::cout << "tstEventsServer: Entering event loop" << std::endl;
    bool done = false;
    while ( !done ) {
        int ier = pthread_mutex_lock(&write_event_);
        if ( ier != 0 ) std::cout << "tstEventsServer.event, couldn't lock mutex, ier = " <<
                                         ier << std::endl;
        InputEvent* event = event_factory->get_next_event(efd);
        ier = pthread_mutex_unlock(&write_event_);
        if ( ier != 0 ) std::cout << "tstEventsServer.event, couldn't unlock mutex, ier = " <<
                                    ier << std::endl;
        if ( event == 0 ) {
            std::cout << "tstEventsServer: Error getting next event" << std::endl;
            done = true;
            break;
        } else {
            std::cout << "| tstEventsServer: " << event->tag() << 
                            " received at " << *job_clock << std::endl;
            event->print(0);
            event->write_event(ess);
            if ( event->tag() == "BRK" ) {
                done = true;
            }
        }
    }

    return 1;
}

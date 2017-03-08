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

#include <string>

#include "packetbuffer.h"
#include "eventfactory.h"
#include "eventdevice.h"
#include "breakevent.h"
#include "blockevent.h"
#include "switchevent.h"
#include "trackevent.h"
#include "event_device_error.h"

trk::EventFactory* trk::EventFactory::instance_ = 0;

trk::EventFactory*
trk::EventFactory::
instance()
{
    if ( !instance_ ) {
        instance_ = new EventFactory();
    }
    return instance_;
}

trk::EventFactory::
EventFactory()
{
}

trk::EventFactory::
~EventFactory()
{
}

trk::InputEvent*
trk::EventFactory::
get_next_event(EventDevice* efd)
{
    InputEvent*  event;
    PacketBuffer* ebfr;

    try {
        ebfr = efd->read();
    } catch (event_device_error r) {
        std::cout << r.reason() << std::endl;
        return 0;
    }
    std::string tag = ebfr->tag();

    if        ( tag == "BRK" ) {
        event = new BreakEvent(ebfr);
    } else if ( tag == "BLK" ) {
        event = new BlockEvent(ebfr);
    } else if ( tag == "SW " ) {
        event = new SwitchEvent(ebfr);
    } else if ( tag == "TRK" ) {
        event = new TrackEvent(ebfr);
    } else {
        std::cout << "EventFactory.get_next_event, invalid tag = " << tag << std::endl;
        return 0;
    }
    return event;
}


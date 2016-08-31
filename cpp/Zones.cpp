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

 * 
 */

#include <iostream>
#include <iomanip>

#include "LayoutConfig.h"
#include "Zones.h"
#include "Zone.h"
#include "TrackEvent.h"

trk::Zones::
Zones()
{
    LayoutConfig* layout_config = LayoutConfig::instance();
    zone_indexes_  = layout_config->zone_indexes();
    
    ix_[0] = O72A;
    ix_[1] = O72B;
    ix_[2] = O72C;
    ix_[3] = O72X;
    ix_[4] = O36X;
    ix_[5] = O60A;
    ix_[6] = O60B;
    ix_[7] = O60C;
    ix_[8] = O60X;
    ls_[0] = LOWERCW;
    ls_[1] = LOWERCCW;
    ls_[2] = UPPERCW;
    ls_[3] = UPPERCCW;
    ls_[4] = TRANSITIONCW;
    ls_[5] = TRANSITIONCCW;
    zone_data_[O72A] = entry(LOWER,      O72B, XXXX, O72C, XXXX);
    zone_data_[O72B] = entry(LOWER,      O72C, XXXX, O72A, O72X);
    zone_data_[O72C] = entry(UPPER,      O72A, O36X, O72B, O72X);
    zone_data_[O72X] = entry(TRANSITION, O72B, O72C, XXXX, XXXX);
    zone_data_[O36X] = entry(TRANSITION, O72C, XXXX, O60A, XXXX);
    zone_data_[O60A] = entry(UPPER,      O60B, O60X, O60C, O36X);
    zone_data_[O60B] = entry(UPPER,      O60C, O60X, O60A, XXXX);
    zone_data_[O60C] = entry(UPPER,      O60A, XXXX, O60B, XXXX);
    zone_data_[O60X] = entry(TRANSITION, XXXX, XXXX, O60A, O60B);
}

trk::Zones::
~Zones() {
    for ( int i = 0; i < zone_indexes_.size(); i++) {
        delete zones_[i];
    }
}

int 
trk::Zones::
active_zones(TrackEvent* event)
{
    std::pair<std::map<std::string, int>::iterator, bool> r;
    
    std::string zone_name   = event->zone_name();
    TRK_STATE   track_state = event->track_state();
    int ia = zone_indexes_[zone_name];
    std::pair<std::string, int> val(zone_name, ia);
    if ( track_state == BUSY) {
        r =active_indexes_.insert(val);
        if ( !r.second) return -1;
    } else if ( track_state == IDLE ) {
        active_indexes_.erase(zone_name);
    }
    return active_indexes_.size();
}

trk::LAYOUT_STATE
trk::Zones::
current_state(TrackEvent* event)
{
    std::string zone_name   = event->zone_name();
    TRK_STATE   track_state = event->track_state();
    int         zone_index  = zone_indexes_[zone_name];
    
    LEVEL       level        = zone_data_[zone_index].level;
    ZN          current_zone = ix_[zone_index];
    ROTATION    rot;
    if ( previous_zone_ ==  zone_data_[zone_index].cw0 ||
         previous_zone_ == zone_data_[zone_index].cw1 ) {
            rot = CW;
    } else if (previous_zone_ == zone_data_[zone_index].ccw0  ||
               previous_zone_ == zone_data_[zone_index].ccw1 ) { 
        rot = CCW;
    } else {
        //throw illegal_event;
    }
    previous_zone_  = current_zone;
    previous_event_ = event;
    return ls_[level + rot];
}

void
trk::Zones::
print_log_entry(TrackEvent* event, const LAYOUT_STATE& current_state)
{
    std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    std::cout.precision(4);
    std::string zone_name = event->zone_name();
    double      tm_event   = event->tm_event();
    TRK_STATE   trkstate  = event->track_state();
    int zone_index = zone_indexes_[zone_name];
    std::cout << std::setw(8) << tm_event;
    int n = zone_indexes_.size();
    for (int i = 0; i < n; i++ ) {
        if ( zone_index == i ) {
            if ( trkstate == BUSY ) {
                std::cout << "| BUSY  ";
            } else if ( trkstate == IDLE ) {
                std::cout << "| IDLE  ";
            } else {
                std::cout << "|XXXXXXX";
            }
        } else {
            for (AZI p = active_indexes_.begin(); p != active_indexes_.end(); p++ ) {
                if ( p->second ==  i ) {
                    std::cout << "|   *   ";
                }
            }
        }
    }
    std::cout << "| " << current_state;
    std::cout << std::endl;
}

std::ostream&
trk::operator<<( std::ostream& ostrm, const trk::LAYOUT_STATE& layout_state)
{
    if (      layout_state == trk::LOWERCW       ) ostrm << "lower loop cw";
    else if ( layout_state == trk::LOWERCCW      ) ostrm << "lower loop ccw";
    else if ( layout_state == trk::UPPERCW       ) ostrm << "upper loop cw";
    else if ( layout_state == trk::UPPERCCW      ) ostrm << "upper loop ccw";
    else if ( layout_state == trk::TRANSITIONCCW ) ostrm << "transition ccw";
    else if ( layout_state == trk::TRANSITIONCW  ) ostrm << "transition cw";
    return ostrm;
}

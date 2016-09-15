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

#ifndef TRK_ZONES_HH
#define TRK_ZONES_HH

#include <map>
#include <vector>
#include <string>

namespace trk {
    enum ZN {O72A, O72B, O72C, O72X, O36X, O60A, O60B, O60C, O60X, XXXX};
    enum ROTATION { CW = 0, CCW = 1};
    enum LEVEL    { LOWER = 0, UPPER = 2, TRANSITION = 4};
    enum LAYOUT_STATE { LOWERCW = 0, LOWERCCW = 1, UPPERCW = 2, UPPERCCW = 3,
                          TRANSITIONCW = 4, TRANSITIONCCW = 5};


    class TrackEvent;
    class Zone;

    class Zones {
        public:
            Zones();
            ~Zones();

            int         active_zones (TrackEvent* event);
            LAYOUT_STATE  current_state(TrackEvent* event);
            void          print_log_entry(TrackEvent* event,
                                          const LAYOUT_STATE& current_state);
            void          print_log_header();
            std::string   zone_name(int i);
        private:
            std::map<std::string, int>         zone_indexes_;
            std::vector<Zone*>                 zones_;
            std::map<std::string, int>         active_indexes_;
            typedef std::map<std::string, int>::const_iterator AZI;
            ZN           ix_[9];
            LAYOUT_STATE ls_[6];

            struct entry {
                entry() {}
                entry(LEVEL a, int b, int c, int d, int e) :
                                      level(a), cw0(b), cw1(c), ccw0(d), ccw1(e) {}
                LEVEL       level;
                int         cw0;
                int         cw1;
                int         ccw0;
                int         ccw1;
            }; 
            entry        zone_data_[9];
            ZN           previous_zone_;
            TrackEvent*  previous_event_;
    }; 

    std::ostream&
    operator<<( std::ostream& ostrm, const trk::LAYOUT_STATE& layout_state);
}

#endif

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

#include "trkutl.h"
#include "JobClock.h"
#include "CmdPacket.h"
#include "SocketClient.cpp"

using namespace trk;

int main() {

    std::cout << "trkScannerClient: Scan trk sensors for current state of layout" << std::endl;

    SocketClient* cmd_fd = new SocketClient("192.168.1.167", 17300);

    std::string type;
    bool done = false;
    while ( !done ) {
        std::cout << "trkScannerClient: Enter type to scan " <<
              " ((t)track, (s)switch, (b)block, done)?:";
        std::cin >> type;
        if (        type == "t" || type == "track" ) {
            int ns = 9;
            std::pair<int, int> item;
            CmdPacket cp("scan",
                         "track",
                        ns);
            for ( int i = 0; i < ns; i++) {
                item.first = i;
                item.second = 0;
                cp.item(i, item);
            }
            try {
                cp.write(cmd_fd);
            } catch ( event_device_error r) {
                std::cout << "trkScannerClient: event_device_error writing cmd packet" << 
                                               std::endl;
                std::cout << "trkScannerClient: " << r.reason() << std::endl;
                std::cout << "trkScannerClient: exiting" << std::endl;
                done = true;
            }
            EventBuffer* cbfr;
            try {
                cbfr = cmd_fd->read();
            } catch ( event_device_error r ) {
                std::cout << "trkScannerClient: event_device_error reading cbfrt" << 
                                               std::endl;
                std::cout << "trkScannerClient: " << r.reason() << std::endl;
                std::cout << "trkScannerClient: exiting" << std::endl;
                done = true;
            }
            std::cout << "trkScannerClient, read cbfr" << std::endl;
                
            CmdPacket* cr;
            try {
                cr = new CmdPacket(cbfr);
            } catch ( event_device_error r) {
                std::cout << "trkScannerClient: event_device_errori creating cmd packet" << 
                                              std::endl;
                std::cout << "trkScannerClient: " << r.reason() << std::endl;
                std::cout << "trkScannerClient: exiting" << std::endl;
                done = true;
            }
            std::cout << "trkScannerClient: command = " << cr->command() << std::endl;
            std::cout << "trkScannerClient: type = " << cr->type() << std::endl;
            int n = cr->n_item();
            std::cout << "trkScannerClient: nitem = " << n << std::endl;
            for (int i = 0; i < n; i++) {
                item = cr->item(i);
                std::cout << "trkScannerClient: " << i << 
                            " <" << item.first << ", " << item.second << ">" << std::endl;
            }
        } else if ( type == "s" || type == "switch" ) {
            //std::cout << *sw_drivers;
        } else if ( type == "b" || type == "block" ) {
            //std::cout << *blk_drivers;
        } else if ( type == "done" ) {
            done = true;
        } else {
            std::cout << " invalid type entered";
        }
        std::cout << std::endl;
    }
    delete cmd_fd;
}

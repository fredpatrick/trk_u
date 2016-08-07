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

#include "JobClock.h"
#include "trkutl.h"
#include "SimpleSocketSrvr.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

using namespace trk;

int main() {

    std::cout << "test program for sockets, server" <<std::endl;

    JobClock* job_clock = trk::JobClock::instance();
    std::cout << *job_clock << std::endl;

    int socket_fd = 0;
    std::string yesno = "";
    yesno = get_yesno( "Create socket for TCP connection?" );
    std::cout <<  "tstSocketSrvr: yesno = " << yesno << std::endl;
    if ( yesno == "yes" ) {
        std::cout << "tstSocketSrvr: Creating socket" << std::endl;
        SimpleSocketSrvr* sckt = new SimpleSocketSrvr(17303);
        socket_fd = sckt->ss_accept();
        double t0 = job_clock->base_time();
        int n;
        n = write(socket_fd, &t0, sizeof(double) );
        std::cout << "tstSocketSrvr, write t0 = " << t0 << std::endl;
        if ( n < 0 ) perror("write");
        double t1 = job_clock->job_time();
        write(socket_fd, &t1, sizeof(double) );
        std::cout << "tstSocketSrvr, write t1 = " << t1 << std::endl;
        if ( n < 0 ) perror("write");
    }
    yesno = get_yesno("Close socket??" );
    if (yesno != "never" ) {
        close(socket_fd);
    }
    return 0;
}



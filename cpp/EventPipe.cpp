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

#include "EventPipe.h"
#include "EventBuffer.h"

#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

trk::EventPipe::
EventPipe()
{
    if ( ::pipe( event_fds_ ) == -1) {
        perror("EventPipe- pipe" );
        //throw ....
    }
}

trk::EventPipe::
~EventPipe()
{
}

int
trk::EventPipe::
write(EventBuffer* ebfr)
{
    int bfrlen = ebfr->bfrlen();
    char* bfr  = ebfr->bfr();
    char ctag[4];
    ::memcpy(ctag, bfr, 4);
    std::string tag = ctag;
    std::cout << "EventPipe::write- tag = " << tag << std::endl;
    ::write( event_fds_[1], &bfrlen, sizeof(int) );
    int ns = ::write( event_fds_[1], bfr, bfrlen);
    return ns;
}

trk::EventBuffer* 
trk::EventPipe::
read()
{
    int bfrlen;
    int ns = ::read(event_fds_[0], &bfrlen, sizeof(int) );
    std::cout << "EventPipe::read- bfrlen = " << bfrlen << std::endl;
    char* bfr = new char[bfrlen];
    ns = ::read(event_fds_[0], bfr, bfrlen  );
    char ctag[4];
    ::memcpy(ctag, bfr, 4);
    std::string tag = ctag;
    std::cout << "EventPipe::read - tag = " << tag << std::endl;
    EventBuffer* ebfr = new EventBuffer(bfrlen, bfr); 
    delete[] bfr;
    return ebfr;
}

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

#include "SimpleSocketClnt.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

trk::SimpleSocketClnt::
SimpleSocketClnt(const std::string& ipaddr, int portno)
{
    int socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if ( socket_fd_ == -1) {
        perror("socket");
        // need to throw exception here
    }
    struct sockaddr_in      serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr) );
    serv_addr.sin_family        = AF_INET;
    serv_addr.sin_port          = htons(portno);
    int ierr = inet_pton(AF_INET, ipaddr.c_str(), &serv_addr.sin_addr);
    if (ierr == 0 ) {
        std::cout << "inet_pton error occured" << std::endl;
        // need to throw exception here
    }
    ierr = connect(socket_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr) );
    if ( ierr < 0 ) {
        perror("connect");
        // need to throw exception here
    }
}

trk::SimpleSocketClnt::
~SimpleSocketClnt()
{
    close ( socket_fd_);
}

int 
trk::SimpleSocketClnt::
ss_read(char* buf, int nbyte)
{
    int  nr   = nbyte;
    int  n    = 0;
    bool done = false;
    while ( !done ) {
        n = read(socket_fd_, (void*)buf[n], nr);
        if ( n == -1 ) {
            std::cout << "SimpleSocketClnt::read error" << std:: endl;
            return -1;
        }
        if ( n == nr ) done = true;
        nr = nbyte - n;
    }
    return nbyte;
}


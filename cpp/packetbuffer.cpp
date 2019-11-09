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

#include "packetbuffer.h"
#include "enum_out_of_range.h"
#include <string.h>
#include <iostream>

trk::PacketBuffer::
PacketBuffer(const std::string& tag)
{
    tag_    = tag;
    ::memcpy( bfr_, tag_.c_str(), tag_.length() +1 );
    bfrndx_ = tag_.length() + 1;;
}

trk::PacketBuffer::
PacketBuffer(int                bfrlen,
            const char*        bfr)
{
    bfrlen_ = bfrlen;
    ::memcpy( bfr_, bfr, bfrlen_);
    bfrndx_ = 0;
    tag_    = &bfr_[bfrndx_];
    bfrndx_ += (tag_.length()+1);
}

trk::PacketBuffer::
~PacketBuffer()
{
}

void
trk::PacketBuffer::
reset()
{
   bfrndx_ = tag_.length() + 1;
}

std::string
trk::PacketBuffer::
tag()
{
    return tag_;;
}

int
trk::PacketBuffer::
bfrlen()
{
    return bfrlen_;
}

char*
trk::PacketBuffer::
bfr()
{
    return bfr_;
}

void
trk::PacketBuffer::
strdat(const std::string& sdat)
{
    int ls = sdat.length() + 1;
    ::memcpy(&bfr_[bfrndx_], sdat.c_str(), ls);
    bfrndx_ +=  ls;
    bfrlen_ = bfrndx_;
}

std::string
trk::PacketBuffer::
strdat()
{
    std::string s = &bfr_[bfrndx_];
    bfrndx_ += (s.length() + 1);
    return s;
}

void 
trk::PacketBuffer::
intdat(int i)
{
    ::memcpy(&bfr_[bfrndx_], &i, sizeof(int) );
    bfrndx_ += sizeof(int);
    bfrlen_ = bfrndx_;
}

int
trk::PacketBuffer::
intdat()
{
    int i;
    ::memcpy(&i, &bfr_[bfrndx_], sizeof(int) );
    bfrndx_ += sizeof(int);
    return i;
}
    
void
trk::PacketBuffer::
dbldat(double d)
{
    ::memcpy(&bfr_[bfrndx_], &d, sizeof(double) );
    bfrndx_ += sizeof(double);
    bfrlen_ = bfrndx_;
}

double
trk::PacketBuffer::
dbldat()
{
    double d;
    ::memcpy(&d, &bfr_[bfrndx_], sizeof(double) );
    bfrndx_ += sizeof(double);
    return d;
}

void
trk::PacketBuffer::
pairdat(std::pair<int, int> pdat)
{
    ::memcpy(&bfr_[bfrndx_], &pdat.first  , sizeof(int) );
    bfrndx_ += sizeof(int);
    ::memcpy(&bfr_[bfrndx_], &pdat.second , sizeof(int) );
    bfrndx_ += sizeof(int);
    bfrlen_ = bfrndx_;
}

std::pair<int, int>
trk::PacketBuffer::
pairdat()
{
    std::pair<int, int> p;
    ::memcpy(&p.first , &bfr_[bfrndx_], sizeof(int));
    bfrndx_ += sizeof(int);
    ::memcpy(&p.second, &bfr_[bfrndx_], sizeof(int));
    bfrndx_ += sizeof(int);
    return p;
}


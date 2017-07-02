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

#include <sstream>
#include "cmdpacket.h"
#include "packetbuffer.h"
#include "eventdevice.h"
#include "illegal_cmdpacket.h"
#include "debugcntl.h"

int trk::CmdPacket::cmd_seqno_ = 0;

trk::CmdPacket::
CmdPacket(const std::string& command,
          const std::string& type,
          int                n_item)
{
    dbg_ = DebugCntl::instance();
    tag_     = "CMD";
    command_ = command;
    type_    = type;
    n_item_  = n_item;
    cmd_seqno_++;
    items_   = new std::pair<int,int>[n_item];
    cbfr_    = new PacketBuffer(tag_);
    cbfr_->strdat(type_);
    cbfr_->strdat(command_);
    cbfr_->intdat(cmd_seqno_);
    cbfr_->intdat(n_item_);
}

trk::CmdPacket::
CmdPacket(PacketBuffer* cbfr)
{
    dbg_ = DebugCntl::instance();
    cbfr_    = cbfr;
    tag_     = cbfr_->tag();
    if ( tag_ != "CMD" ) {
        std::stringstream ost;
        ost << "CmdPacket.ctor,  tag " << tag_;
        throw illegal_cmdpacket(ost.str() );
    }
    type_      = cbfr_->strdat();
    command_   = cbfr_->strdat();
    cmd_seqno_ = cbfr_->intdat();
    n_item_    = cbfr_->intdat();
    items_     = new std::pair<int,int>[n_item_];
    for ( int i = 0; i < n_item_; i++) {
        items_[i] = cbfr_->pairdat();
    }
    if ( dbg_->check(5) ) {
        std::cout << "CmdPacket.ctor-2, type = " << type_ << 
                                    ", command = " << command_ << 
                                    ", cmd_seqno = " << cmd_seqno_ <<  std::endl;
    }
}

trk::CmdPacket::
~CmdPacket()
{
    delete[] items_;
    delete   cbfr_;
}

void
trk::CmdPacket::
write( EventDevice* cmd_fd)
{
    cbfr_->reset();
    cbfr_->strdat(type_);
    cbfr_->strdat(command_);
    cbfr_->intdat(cmd_seqno_);
    cbfr_->intdat(n_item_);
    for ( int i = 0; i < n_item_; i++) {
        cbfr_->pairdat(items_[i]);
    }
    if ( dbg_->check(5) ) {
        std::cout << "CmdPacket.write, type = " << type_ << 
                                    ", command = " << command_ << 
                                    ", cmd_seqno = " << cmd_seqno_ <<  std::endl;
    }
    cmd_fd->write(cbfr_);
}

std::string
trk::CmdPacket::
command()
{
    return command_;
}

std::string
trk::CmdPacket::
type()
{
    return type_;
}

int
trk::CmdPacket::
cmd_seqno()
{
    return cmd_seqno_;
}

int
trk::CmdPacket::
n_item()
{
    return n_item_;
}

std::pair<int, int>
trk::CmdPacket::
item(int i)
{
    return items_[i];
}

void
trk::CmdPacket::
item(int i, std::pair<int, int> p)
{
    items_[i] = p;
}

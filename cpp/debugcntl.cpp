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

#include <stdlib.h>
#include <string>
#include <iostream>

#include "debugcntl.h"

trk::DebugCntl* trk::DebugCntl::instance_ = 0;

trk::DebugCntl*
trk::DebugCntl::
instance()
{
    if ( instance_ == 0 ) {
        instance_ = new DebugCntl();
    }
    return instance_;
}

trk::DebugCntl::
DebugCntl()
{
    level_ = 0;
}

void
trk::DebugCntl::
parse_argv(int argc, char* argv[])
{
    for ( int i = 1; i < argc; i++ ) {
        std::string arg = argv[i];
        std::cout << "DebugCntl.parse_argv, arg = " << arg << std::endl;
        std::string t(arg, 0, 6);
        std::cout << "DebugCntl.parse_argv, t   = " << t   << std::endl;
        if ( t == "-debug" ) {
            std::string sl(arg,7, 1);
            std::cout << "DebugCntl.parse_argv, sl  = " << sl  << std::endl;
            level_ = ::atoi(sl.c_str() );
            std::cout << "DebugCntl.parse_argv, level set to " << level_ << std::endl;
        }
    }
}

bool
trk::DebugCntl::
check(int l)
{
    if (l <= level_ ) return true;
    else              return false;
}

void
trk::DebugCntl::level(int l)
{
    level_ = l;
}

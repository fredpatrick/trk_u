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

#ifndef TRK_DEBUGCNTL_HH
#define TRK_DEBUGCNTL_HH
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

namespace trk
{
    enum FMTCNTL {jbtime, boolalpha, endl};
    
    class DebugCntl;
    trk::DebugCntl& operator<<(trk::DebugCntl& dbg, const std::string& str);
    trk::DebugCntl& operator<<(trk::DebugCntl& dbg, int);
    trk::DebugCntl& operator<<(trk::DebugCntl& dbg, double);
    trk::DebugCntl& operator<<(trk::DebugCntl& dbg, const FMTCNTL&);

    class DebugCntl
    {
        public:
            static DebugCntl* instance();
            ~DebugCntl();

            void       parse_argv(int argc, char* argv[]);
            bool       check(int l);

            void       level(int l);
            std::string bools(int i) { return bools_[i]; }

            friend trk::DebugCntl& trk::operator<<(trk::DebugCntl&, const std::string&);
            friend trk::DebugCntl& trk::operator<<(trk::DebugCntl&, int);
            friend trk::DebugCntl& trk::operator<<(trk::DebugCntl&, double);
            friend trk::DebugCntl& trk::operator<<(trk::DebugCntl&, const FMTCNTL&);

        protected:
            DebugCntl(const std::string& dbgfil);

        private:
            std::ofstream     dbgstrm_;
            int               level_;
            static DebugCntl* instance_;
            std::vector<std::string> bools_;
    };
}
#endif

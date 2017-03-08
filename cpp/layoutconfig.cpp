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
#include "layoutconfig.h"

trk::LayoutConfig* trk::LayoutConfig::instance_ = 0;

trk::LayoutConfig*
trk::
LayoutConfig::instance()
{
    std::string cfgfil = "./layout_config";
    if ( !instance_ ) {
        instance_ = new LayoutConfig(cfgfil);
    }
    return instance_;
}

trk::
LayoutConfig::LayoutConfig(const std::string& cfgfil)
{
    std::ifstream from(cfgfil.c_str() );
//  std::cout << "LayoutConfig::LayoutConfig " << cfgfil << " opened" << std::endl;
    char        cline[120];

    while (!from.eof() ) {
        std::string tag = "";
        from >> tag;
        if ( !from.good() ) {
            break;
        }
        if  (tag[0] == '#' ) {
            from.getline(cline, 120);
        } else if ( tag == "SW")   {
            std::string sw_name;
            int         sw_num;
            from >> sw_name >> sw_num;
            sw_names_[sw_name] = sw_num;
        } else if ( tag == "TRK")  {
            std::string zone_name;
            int    zone_index;
            from >> zone_name >> zone_index;
            zone_indexes_[zone_name] = zone_index;
        } else if ( tag == "BLK" ) {
            std::string  blk_name;
            int          blk_index;
            from >> blk_name >> blk_index;
            blk_map_[blk_name] = blk_index;
        }
    }
    from.close();
}

std::vector<std::string>
trk::LayoutConfig::
blk_names()
{
    std::vector<std::string> tnms;
    tnms.reserve(blk_map_.size() ); 
    typedef std::map<std::string,int>::const_iterator CI;
    for ( CI p = blk_map_.begin(); p != blk_map_.end(); p++ ) {
        tnms[p->second] = p->first;
    }
    return tnms;
}

std::map<std::string, int>
trk::LayoutConfig::
zone_indexes()
{
    return zone_indexes_;;
}

std::map<std::string, int>
trk::LayoutConfig::
sw_names()
{
    return sw_names_;;
}

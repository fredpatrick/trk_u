
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
#include <unistd.h>
#include "layoutconfig.h"
#include "filestore.h"

using namespace trk;

trk::LayoutConfig* trk::LayoutConfig::instance_ = 0;

trk::LayoutConfig*
trk::LayoutConfig::
instance()
{
    if ( !instance_ ) {
        FileStore* fs = FileStore::instance();
        instance_ = new LayoutConfig(fs->cfgfil() );
    }
    return instance_;
}

trk::
LayoutConfig::
LayoutConfig(const std::string& cfgfil)
{
    std::ifstream from(cfgfil.c_str() );
    if ( from.is_open() ) {
        std::cout << "LayoutConfig.ctor, cfgfil opened" << std::endl;
    } else {
        std::cout << "LayoutConfig, open for " << cfgfil << " failed" << std::endl;
    }
    std::string   pin_name;
    GPIOData      data;
    char         cline[120];

    while (!from.eof() ) {
        std::string tag = "";
        from >> tag;
        if  (tag[0] == '#' ) {
            from.getline(cline, 120);
        } else if ( tag == "HDR" ) {
            from >> pin_name >> data.gpio_num >> data.gpio_pin >> data.direction >> data.color;
            header_pins_[pin_name] = data;
        } else if ( tag == "ADDR") {
            std::string dmx_a;
            from >> dmx_a >> pin_name;
            address_pins_[dmx_a] = pin_name;
        } else if ( tag == "SW")   {
            SWKey key;
            SWData     swd;
            from >> swd.switch_name >> swd.switch_index >> swd.direc >> swd.pin_name;
            key.num = swd.switch_index;
            key.swd = swd.direc;
            switch_sensors_[key] = swd;
        } else if ( tag == "TRK")  {
            TRKData      td;
            std::string sensor_name;
            from >> sensor_name >> td.sensor_index >> td.pin_name;
            track_sensors_[sensor_name] = td;
        } else if ( tag == "PCB")  {
            from >> pcb_power_pin_;
        } else if ( tag == "BRKSIG") {
            from >> brk_event_pin_;
        } else if ( tag == "BLK" ) {
            BLKData bd;
            std::string  blk_name;
            from >> blk_name;
            if ( blk_name == "Base" ) {
                from >> block_base_index_;
            } else {
                from >> bd.sensor_index >> bd.pin_name;
                block_sensors_[blk_name] = bd;
            }
        }
    }
    track_sensor_count_ = track_sensors_.size();
    block_sensor_count_ = block_sensors_.size();
    switch_count_       = switch_sensors_.size() / 2;
    switch_names_.resize(switch_count_);

    typedef std::map<SWKey, SWData>::const_iterator CI;
    for ( CI p = switch_sensors_.begin(); p != switch_sensors_.end(); p++) {
        if ( p->second.direc == THRU ) {
            switch_names_[p->second.switch_index] = p->second.switch_name;
        }
    }
}

std::vector<std::string>
trk::LayoutConfig::
track_sensor_names()
{
    std::vector<std::string> track_sensor_names;
    track_sensor_names.resize(track_sensors_.size() );
    typedef std::map<std::string, TRKData>::const_iterator CI;
    for ( CI p = track_sensors_.begin(); p != track_sensors_.end(); p++) {
        track_sensor_names[p->second.sensor_index] = p->first;
    }
    return track_sensor_names;
}

int
trk::LayoutConfig::
track_sensor_gpio_num(const std::string& sensor_name)
{
    std::string pin_name = track_sensors_[sensor_name].pin_name;
    return header_pins_[pin_name].gpio_num;
}
/*
void
trk::LayoutConfig::
clear_gpios()
{
    typedef std::map<std::string, LayoutData>::const_iterator CI;
    for (CI p = header_pins_.begin(); p!= header_pins_.end(); ++p) {
        LayoutData data = p->second;
        Layout* gpio = new GPIO( data.gpio_num);
        delete gpio;
    }
}
*/

std::vector<std::string>
trk::LayoutConfig::
block_sensor_names()
{
    std::cout << "LayoutConfig::block_sensor_names" << std::endl;
    std::vector<std::string> bns;
    bns.resize(block_sensors_.size() );

    typedef std::map<std::string, BLKData>::const_iterator CI;
    for ( CI p = block_sensors_.begin(); p != block_sensors_.end(); p++) {
        std::cout << "LayoutConfig::block_sensor_names, " << p->first << std::endl;
        bns[p->second.sensor_index] = p->first;
        std::cout << "LayoutConfig::block_sensor_names, " << p->first << std::endl;
    }
    return bns;
}

int
trk::LayoutConfig::
block_sensor_gpio_num(const std::string& sensor_name)
{
    std::string pin_name = block_sensors_[sensor_name].pin_name;
    return header_pins_[pin_name].gpio_num;
}

int
trk::LayoutConfig::
switch_sensor_index(const std::string& switch_name)
{
    typedef std::map<SWKey, SWData>::const_iterator CI;
    for ( CI p = switch_sensors_.begin(); p != switch_sensors_.end(); p++) {
        if ( p->second.switch_name == switch_name ) {
            //sns[p->second.switch_index] = p->second.switch_name;
            return p->second.switch_index;
        }
    }
    return 999;
}


int
trk::LayoutConfig::
switch_sensor_gpio_num(const SWKey& key)
{
    std::string pin_name = switch_sensors_[key].pin_name;
    return header_pins_[pin_name].gpio_num;
}

void
trk::LayoutConfig::
list_header_pins(std::ostream& ostrm)
{
    ostrm << "LayoutConfig::list_header_pins" << std::endl;
    typedef std::map<std::string, GPIOData>::const_iterator CI;
    for (CI p = header_pins_.begin(); p!= header_pins_.end(); ++p) {
        ostrm << p->first << '\t' << p->second << '\n';
    }
}

void
trk::
LayoutConfig::list_demux_address_pins(std::ostream& ostrm)
{
    ostrm << "LayoutConfig::list_demux_address_pins" << std::endl;
    typedef std::map<std::string, std::string>::const_iterator CI;
    for (CI p = address_pins_.begin(); p!= address_pins_.end(); ++p) {
        ostrm << p->first << '\t' << p->second << '\n';
    }
}

void
trk::
LayoutConfig::list_switch_sensor_pins(std::ostream& ostrm)
{
    ostrm << "LayoutConfig::list_switch_sensor_pins" << std::endl;
    typedef std::map<SWKey, SWData>::const_iterator CI;
    for (CI p = switch_sensors_.begin(); p!= switch_sensors_.end(); ++p) {
        ostrm << p->first << '\t' << p->second.pin_name << '\n';
    }
}

void
trk::
LayoutConfig::list_track_sensor_pins(std::ostream& ostrm)
{
    ostrm << "LayoutConfig::list_track_sensor_pins" << std::endl;
    typedef std::map<std::string, TRKData>::const_iterator CI;
    for (CI p = track_sensors_.begin(); p!= track_sensors_.end(); ++p) {
        ostrm << p->first << '\t' << p->second.sensor_index << 
                             '\t' << p->second.pin_name << '\n';
    }
}

void
trk::LayoutConfig::
list_pcb_power_pin(std::ostream& ostrm)
{
    ostrm << "LayoutConfig::pcb_power_pin: ";
    ostrm << pcb_power_pin_ << std::endl;
}

std::ostream&
trk::operator<<( std::ostream& ostrm, const trk::GPIOData& data)
{
    ostrm << data.gpio_num << '\t' << data.gpio_pin << '\t' << 
                                     data.direction << '\t' << data.color;
    return ostrm;
}

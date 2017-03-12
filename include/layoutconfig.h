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

#ifndef TRK_LATOUTCONFIG_HH
#define TRK_LATOUTCONFIG_HH

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#include "trkutl.h"

namespace trk {

struct GPIOData {
    int         gpio_num;
    int         gpio_pin;
    std::string direction;
    std::string color;
};

struct SWData {
    std::string  switch_name;
    int          switch_index;
    SW_DIRECTION direc;       
    std::string  pin_name;
};

struct BLKData {
    int         sensor_index;
    std::string pin_name;
};

struct TRKData {
    int         sensor_index;
    std::string pin_name;
};

class LayoutConfig {
    public: 
        static LayoutConfig* instance();

        ~LayoutConfig();

        int                      gpio_num(const std::string& pin_name)
                                    { return header_pins_[pin_name].gpio_num; }
        std::string              pcb_power_gpio()
                                    { return pcb_power_pin_; }
        int                      demux_address_gpio_num(const std::string& a)
                                    { return header_pins_[ address_pins_[a] ].gpio_num; }
        int                      break_sensor_gpio_num()
                                    { return header_pins_[brk_event_pin_].gpio_num; }
        int                      switch_sensor_gpio_num(const SWKey& key);
        std::string              switch_name(const SWKey& key) 
                                    { return switch_sensors_[key].switch_name; }

        std::vector<std::string> track_sensor_names();
        int                      track_sensor_gpio_num(const std::string& sensor_name);
        int                      track_sensor_index(const std::string& sensor_name)
                                    { return track_sensors_[sensor_name].sensor_index; }

        std::vector<std::string> block_sensor_names();
        int                      block_sensor_gpio_num(const std::string& sensor_name);
        int                      block_base_addr()
                                    { return block_base_index_; }
        int                      block_sensor_index(const std::string& sensor_name)
                                    { return block_sensors_[sensor_name].sensor_index; }
        std::vector<std::string> switch_names();

        void        clear_gpios();
        std::vector<std::string> zone_names();

        void    list_header_pins(std::ostream& ostrm);
        void    list_demux_address_pins(std::ostream& ostrm);
        void    list_switch_sensor_pins(std::ostream& ostrm);
        void    list_track_sensor_pins(std::ostream& ostrm);
        void    list_pcb_power_pin(std::ostream& ostrm);
    protected:
        LayoutConfig(const std::string& cfgfil);
    private:

        std::map<std::string, GPIOData>     header_pins_;
        std::map<std::string, std::string>  address_pins_;
        std::map<SWKey, SWData>             switch_sensors_;
        std::map<std::string, TRKData>      track_sensors_;
        std::map<std::string, BLKData>      block_sensors_;
        int                                 block_base_index_;
        std::string                         pcb_power_pin_;
        std::string                         brk_event_pin_;

        static LayoutConfig* instance_;
};

std::ostream&
operator<<( std::ostream& ostrm, const trk::GPIOData& data);
}

#endif

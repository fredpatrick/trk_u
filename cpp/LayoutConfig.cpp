
#include <iostream>
#include "LayoutConfig.h"

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
            blk_names_[blk_name] = blk_index;
        }
    }
    from.close();
}

std::map<std::string, int>
trk::LayoutConfig::
blk_names()
{
    return blk_names_;;
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

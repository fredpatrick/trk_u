#include "filestore.h"
#include "layoutconfig.h"
#include <iostream>

int main ( int argc, char* argv[])
{
    trk::FileStore* fs = trk::FileStore::instance();
    fs->vtxfil("/Users/fredpatrick/wrk/cfg/layout24c.vtx");
    fs->cfgfil("/Users/fredpatrick/wrk/cfg/layout_config.txt");
    fs->pthfil("/Users/fredpatrick/wrk/cfg/path_definition.txt");
    std::cout << "tstconfig" << std::endl;
    std::cout << *fs << std::endl;

    trk::LayoutConfig* layout_config = trk::LayoutConfig::instance();
    std::cout << "tstconfig, got LayoutConfig instance" << std::endl;
    return 0;
}

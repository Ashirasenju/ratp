#include "help.h"
std::string help() {
    return "Usage : ratp command [options]\nCommands :\nratp install : install a package from distant webserveur. Options avaliable : \n --local : install a package from local tarball. Usage : ratp install --local /path/to/file\nratp update  : download all the new packages and put them into the cache\nratp purge   : delete a package";
}

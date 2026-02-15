#include "help.h"
std::string help() {
    return "Usage : ratp command [options]\nCommands :\nratp install : install a package from distant webserveur\nratp update  : download all the new packages and put them into the cache\nratp purge   : delete a package";
}

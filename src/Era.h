#include"Watchy.h"
#include "services/bt/config_bt_server.h"

class Era : public Watchy {
    public:
        void init(String datetime = "");
        virtual void showMenu(byte menuIndex, bool partialRefresh);
        void handleButtonPress();
        void drawWatchFace();
        ConfigBtServer* configBtServer;
};
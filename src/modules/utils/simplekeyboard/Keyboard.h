#include "libs/Kernel.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "libs/Pin.h"
#include <string>

class Keyboard : public Module {
    public:
        Keyboard();

        void on_module_loaded();

        uint32_t keyboard_tick(uint32_t dummy);

        void send_gcode(string msg, StreamOutput* stream);

        bool       send_command;
        std::string   g_code;
//        std::string   off_m_code;
        //Pin*       button;
        Pin*       xinc;		
        Pin*       xdec;		
        Pin*       yinc;		
        Pin*       ydec;		
        Pin*       zinc;		
        Pin*       zdec;		
        Pin*       ainc;		
        Pin*       adec;		

		SerialMessage new_message;
        StreamOutput* dummy_stream;
};



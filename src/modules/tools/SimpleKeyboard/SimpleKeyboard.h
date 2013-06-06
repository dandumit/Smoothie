#include "libs/Pin.h"
#include <math.h>

class SimpleKeyboard : public Module {
    public:
        SimpleKeyboard();
		
        void on_module_loaded();
		void on_main_loop( void* argument );

        uint32_t keyboard_tick(uint32_t dummy);

        Pin       xinc;		
		volatile bool      send_command;
        void send_gcode(string msg, StreamOutput* stream);
};

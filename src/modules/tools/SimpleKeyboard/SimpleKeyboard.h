#include "libs/Pin.h"
#include <math.h>

#define x_axis_max_speed_checksum              CHECKSUM("x_axis_max_speed")
#define y_axis_max_speed_checksum              CHECKSUM("y_axis_max_speed")
#define z_axis_max_speed_checksum              CHECKSUM("z_axis_max_speed")

class SimpleKeyboard : public Module {
    public:
        SimpleKeyboard();
		
        void on_module_loaded();
		void on_main_loop( void* argument );

        uint32_t keyboard_tick(uint32_t dummy);

        Pin       xinc;		
        Pin       xdec;		
        Pin       yinc;		
        Pin       ydec;		
        Pin       zinc;		
        Pin       zdec;		
        Pin       ainc;		
        Pin       adec;		
        Pin       pot_pin;
		
		//string g_code;
		//volatile bool send_command;
		volatile unsigned int      command_code;
        void send_gcode(string msg, StreamOutput* stream);
		double max_speed;
};

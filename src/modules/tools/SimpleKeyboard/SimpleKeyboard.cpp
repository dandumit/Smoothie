
#include "libs/Module.h"
#include "libs/Kernel.h"
#include "libs/SerialMessage.h"
#include <math.h>
#include "SimpleKeyboard.h"
#include "libs/Pin.h"
#include "modules/robot/Conveyor.h"

#include "MRI_Hooks.h"

SimpleKeyboard::SimpleKeyboard(){}

void SimpleKeyboard::on_module_loaded(){
	this->xinc.from_string("0.10v")->as_input();
    this->kernel->slow_ticker->attach( 100, this, &SimpleKeyboard::keyboard_tick );
	this->register_for_event(ON_MAIN_LOOP);
	this->send_command=false;	
}
uint32_t SimpleKeyboard::keyboard_tick(uint32_t dummy){
	if(this->xinc.get() == true) {
//		this->send_gcode( this->input_on_command, &(StreamOutput::NullStream) );
		this->send_command=true;
	}
    return 0;
}

void SimpleKeyboard::send_gcode(std::string msg, StreamOutput* stream) {
    struct SerialMessage message;
    message.message = msg;
    message.stream = stream;
	this->kernel->conveyor->wait_for_queue(2);
    this->kernel->call_event(ON_CONSOLE_LINE_RECEIVED, &message );
}

void SimpleKeyboard::on_main_loop(void* argument){
	//string new_message = *static_cast<string*>(argument);
	if(	this->send_command) {	
		this->send_gcode( "G1 X17", &(StreamOutput::NullStream) );	
		this->send_command=false;	
	}
}
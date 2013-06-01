#include "libs/Kernel.h"
#include "libs/SerialMessage.h"
#include "Keyboard.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include <string>
using namespace std;

Keyboard::Keyboard(){}


void Keyboard::on_module_loaded(){
	this->xinc->from_string("0.10^")->as_input();;
	this->xdec->from_string("0.1^")->as_input();;
	this->yinc->from_string("0.0^")->as_input();;
	this->ydec->from_string("1.28^")->as_input();;
	this->zinc->from_string("1.26^")->as_input();;
	this->zdec->from_string("1.24^")->as_input();;
	this->ainc->from_string("1.21^")->as_input();;
	this->adec->from_string("3.25^")->as_input();;
	
    // Settings
    //this->on_config_reload(this);
    this->kernel->slow_ticker->attach( 100, this, &Keyboard::keyboard_tick );
}

//TODO: Make this use InterruptIn
//Check the state of the button and act accordingly
uint32_t Keyboard::keyboard_tick(uint32_t dummy){
    
	send_command=false;
	
	if(!xinc) {
		g_code = " X100";
		send_command=true;
	} else if (!xdec) {
		g_code = " X-100";
		send_command=true;
	}
	
	if(!yinc) {
		g_code = g_code + " Y100";
		send_command=true;
	} else if (!ydec) {
		g_code = g_code + " Y-100";
		send_command=true;
	}

	if(!zinc) {
		g_code = g_code + " Z100";
		send_command=true;
	} else if (!zdec) {
		g_code = g_code + " Z-100";
		send_command=true;
	}
	
	if(!ainc) {
		g_code = g_code + " A100";
		send_command=true;
	} else if (!adec) {
		g_code = g_code + " A-100";
		send_command=true;
	}	
	
	if(send_command) {
		g_code="G01" + g_code + "\r\n";
		//printf("dispatch '%s' ...",g_code);
		this->send_gcode( g_code, dummy_stream );
	}
    return 0;
}

void Keyboard::send_gcode(std::string msg, StreamOutput* stream) {
    struct SerialMessage message;
    message.message = msg;
    message.stream = stream;
    this->kernel->call_event(ON_CONSOLE_LINE_RECEIVED, &message );
}


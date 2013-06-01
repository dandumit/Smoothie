#include "libs/Kernel.h"
#include "SimpleKeyboard.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "libs/SerialMessage.h"
#include "libs/StreamOutput.h"
//using namespace std;

SimpleKeyboard::SimpleKeyboard(){}


void SimpleKeyboard::on_module_loaded(){

	this->send_command=false;
	//this->current_stream = &(StreamOutput::NullStream);

	this->xinc.from_string("0.10")->as_input()->pull_down();
/*	this->xdec.from_string("0.1")->as_input()->pull_down();
	this->yinc.from_string("0.0")->as_input()->pull_down();
	this->ydec.from_string("1.28")->as_input()->pull_down();
	this->zinc.from_string("1.26")->as_input()->pull_down();
	this->zdec.from_string("1.24")->as_input()->pull_down();
	this->ainc.from_string("1.21")->as_input()->pull_down();
	this->adec.from_string("3.25")->as_input()->pull_down();
*/
    this->kernel->slow_ticker->attach( 100, this, &SimpleKeyboard::keyboard_tick );
}

//TODO: Make this use InterruptIn
//Check the state of the button and act accordingly
uint32_t SimpleKeyboard::keyboard_tick(uint32_t dummy){
    string   g_code;
	g_code.reserve(20);
	this->send_command=false;
	
	if(this->xinc.get() ) {
		g_code = " X100";
		this->send_command=true;
	}
/*	else if (this->xdec.get()) {
		g_code = " X-100";
		this->send_command=true;
	}

	if(this->yinc.get() ) {
		g_code = g_code + " Y100";
		this->send_command=true;
	} else if (this->ydec.get() ) {
		g_code = g_code + " Y-100";
		this->send_command=true;
	}

	if(this->zinc.get() ) {
		g_code = g_code + " Z100";
		this->send_command=true;
	} else if (this->zdec.get() ) {
		g_code = g_code + " Z-100";
		this->send_command=true;
	}
	
	if(this->ainc.get() ) {
		g_code = g_code + " A100";
		this->send_command=true;
	} else if (this->adec.get() ) {
		g_code = g_code + " A-100";
		this->send_command=true;
	}	
	*/
	
	if(this->send_command) {
		//struct SerialMessage message;
		this->current_stream->printf("%s\n", g_code.c_str());
		//message.message = "G1" + g_code + ";\r\n";
		//message.stream = this->dummy_stream;
		//this->kernel->call_event(ON_CONSOLE_LINE_RECEIVED, &message );
	}
    return 0;
}

/*
void SimpleKeyboard::send_gcode(std::string msg, StreamOutput* stream) {
    struct SerialMessage message;
    message.message = msg;
    message.stream = stream;
    this->kernel->call_event(ON_CONSOLE_LINE_RECEIVED, &message );
}
*/

		//g_code="G01" + g_code + "\r\n";
		//printf("dispatch '%s' ...",g_code);
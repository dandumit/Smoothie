
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
	this->xdec.from_string("0.1v")->as_input();
	this->yinc.from_string("0.0v")->as_input();
	this->ydec.from_string("1.28v")->as_input();
	this->zinc.from_string("1.26v")->as_input();
	this->zdec.from_string("1.24v")->as_input();
	this->ainc.from_string("1.21v")->as_input();
	this->adec.from_string("3.25v")->as_input();
	this->max_speed = this->kernel->config->value(x_axis_max_speed_checksum)->by_default(60000)->as_number();
    // Thermistor pin for ADC readings
    this->pot_pin.from_string("0.23");
    this->kernel->adc->enable_pin(&pot_pin);	
	
	this->command_code=0;	
	this->register_for_event(ON_MAIN_LOOP);
	
    this->kernel->slow_ticker->attach( 500, this, &SimpleKeyboard::keyboard_tick );
}
uint32_t SimpleKeyboard::keyboard_tick(uint32_t dummy){
	if(this->command_code == 0){
		if(this->xinc.get()) {
			this->command_code=1<<7;
		} else if(this->xdec.get()) {
			this->command_code=1<<6;
		}

		if(this->yinc.get()) {
			this->command_code+=1<<5;
		} else if(this->ydec.get()) {
			this->command_code+=1<<4;
		}

		if(this->zinc.get()) {
			this->command_code+=1<<3;
		} else if(this->zdec.get()) {
			this->command_code+=1<<2;
		}

		if(this->ainc.get()) {
			this->command_code+=1<<1;
		} else if(this->adec.get()) {
			this->command_code+=1<<0;
		}
		
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
	if(this->command_code > 0) {
		string g_code;

		int last_raw = 4099-(this->kernel->adc->read(&pot_pin));
		//this->max_speed = (int)(this->max_speed /(1+last_raw));
		int spd;
		spd=(int)((this->max_speed /4098)*(1+last_raw));
		char speed[5];
		//sprintf(speed,"%d",this->max_speed);
		sprintf(speed,"%d",spd);

		double dst;
		dst=spd/1000.0F;
		char dist[6];
		snprintf(dist,6,"%6.4f",dst);
		
		g_code = "G91 G1";
		
		if(this->command_code&(1<<7)) {
			g_code = g_code + " X" + dist;
		} else if(this->command_code&(1<<6)) {
			g_code = g_code + " X-" + dist;
		}

		if(this->command_code&(1<<5)) {
			g_code = g_code + " Y" + dist;
		} else if(this->command_code&(1<<4)) {
			g_code = g_code + " Y-" + dist;
		}

		if(this->command_code&(1<<3)) {
			g_code = g_code + " Z" + dist;			
		} else if(this->command_code&(1<<2)) {
			g_code = g_code + " Z-" + dist;			
		}

		if(this->command_code&(1<<1)) {
			g_code = g_code + " Z" + dist;			
		} else if(this->command_code&(1<<0)) {
			g_code = g_code + " Z-" + dist;			
		}
		
		g_code = g_code + " F" + speed + " \r\n";

		kernel->streams->printf("Trimit : %s",((char*)g_code.c_str()));		
		
		//this->send_gcode( g_code , &(StreamOutput::NullStream) );	
		    struct SerialMessage message;
			message.message = g_code;
			message.stream = &(StreamOutput::NullStream);
			this->kernel->conveyor->wait_for_queue(2);
			this->kernel->call_event(ON_CONSOLE_LINE_RECEIVED, &message );
			
		this->command_code=0;	
	}
}
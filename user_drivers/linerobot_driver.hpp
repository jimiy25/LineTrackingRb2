/*
 * robocart_driver.hpp
 *
 *  Created on: July 23, 2015
 *      Author: Daniella Niyonkuru
 */

#ifndef robocart_driver_HPP_
#define robocart_driver_HPP_

#include "sensors_port.hpp"
#include "motors_port.hpp"
#include "mbed.h"
#include "SeeedStudioShieldBot.h"

using namespace std;
using namespace boost::simulation;
using namespace boost::simulation::pdevs;
using namespace boost::simulation::pdevs::basic_models;

DigitalIn hwbtn(PC_13);
SeeedStudioShieldBot bot(
    D8, D9, D11,              // Left motor pins
    D12, D10, D13,            // Right motor pins
    A0, A1, A2, A3, D4        // Sensors pins
);

bool startRight = true;
bool startLeft = true;
bool isStarted = false;

/* INPUT PORTS DRIVERS */
template<class TIME, class MSG>
bool START_IN<TIME, MSG>::pDriver(Value &v) const noexcept {
	  if(!hwbtn){
		if(!isStarted){
			isStarted = true;
			v = 10;
		}
		else{
			isStarted = false;
			v = 11;
		}
		//printf("Button Pressed %d \n",v);
		return true;
	  }
	  else{
		return false;
	  }
}



//template<class TIME, class MSG>
//bool LIGHT_IN<TIME, MSG>::pDriver(Value &v) const noexcept {
////    if(bot.getCentreSensor() == 0)    was before modification
//    if(bot.getAnySensor() == 0)
//    	v = BRIGHT;
//    else
//    	v = DARK;
//	return true;
//}

template<class TIME, class MSG>
bool LIGHT_IN<TIME, MSG>::pDriver(Value &v) const noexcept {
	if(bot.getCentreSensor() == 0 && bot.getLeftSensor() == 0 && bot.getRightSensor() == 0)
		v = 15;
	else if(bot.getCentreSensor() == 0 && (bot.getLeftSensor() != 0 || bot.getRightSensor() != 0))
		v = 1;
    else if(bot.getRightSensor() == 0 || bot.outRightSensor() == 0)	//added
    	v = 3;
    else if(bot.getLeftSensor() == 0 || bot.outLeftSensor() == 0)		//added
    	v = 4;
    else if(bot.getCentreSensor() != 0 && bot.getLeftSensor() != 0 && bot.getRightSensor() != 0)
    	v = 2;
    else
    	v = 0;
	//printf("Light value = %d  \n",v);
	return true;
}


/* OUTPUT PORTS DRIVERS */
template<class TIME, class MSG>
bool MOVER_OUT<TIME, MSG>::pDriver(Value& v) const noexcept{ //motor left
	// This will be moved into the initHardware function
	if(startLeft){
		bot.enable_left_motor();
		startLeft = false;
	}
	if(v == 1){
	  bot.left_motor(0.40); //FWD the left motor
	  //printf("M1 - FWD \n");
	}
	else if(v == 2){
		  bot.left_motor(0.11); //FWD the left motor
		  //printf("M1 - FWD \n");
		}
	else if(v == 3){
	  bot.left_motor(-0.08);//REV the left motor
	  //printf("M1 - BKWD \n");
	}
	else if(v == 4){
		float i = 0.01;
		while (i <= 0.20 && bot.getCentreSensor() != 0 && bot.getLeftSensor() != 0 && bot.getRightSensor() != 0 && bot.outLeftSensor() != 0 && bot.outRightSensor() != 0){
			bot.left_motor(i);
			i = i + 0.0000001 ;

		}
		while (i >= 0.02 && bot.getCentreSensor() != 0 && bot.getLeftSensor() != 0 && bot.getRightSensor() != 0 && bot.outLeftSensor() != 0 && bot.outRightSensor() != 0){
					bot.left_motor(i);
					i = i - 0.0000001 ;

				}
		  //REV the left motor
		  //printf("M1 - BKWD \n");
		}
	else if(v == 0){
	  bot.left_motor(0); //Stop the left motor
	  //printf("M1 - STOP \n");
	}
	else{
	  //printf("Unchanged \n");
	}
	return true;

}

template<class TIME, class MSG>
bool MOVEL_OUT<TIME, MSG>::pDriver(Value& v) const noexcept{ // motor right
	// This will be moved into the initHardware function
	if(startRight){
		bot.enable_right_motor();
		startRight = false;
	}

	if(v == 1){
	  bot.right_motor(0.36);
	  //printf("M2 - FWD \n");
	}
	else if(v == 2){
		  bot.right_motor(0.08);
		  //printf("M2 - FWD \n");
		}
	else if(v == 3){
	  bot.right_motor(-0.08);
	  //printf("M2 - BKWD \n");
	}
	else if(v == 4){
		bot.right_motor(0.15);
	}
	else if(v == 0){
	  bot.right_motor(0);
	  //printf("M2 - STOP \n");
	}
	else{
	  //printf("Unchanged \n");
	}
	return true;
}

#endif /* robocart_driver_HPP_ */

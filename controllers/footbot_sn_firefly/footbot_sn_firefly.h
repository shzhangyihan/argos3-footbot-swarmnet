#ifndef FOOTBOT_SN_FIREFLY_H
#define FOOTBOT_SN_FIREFLY_H

/*
 * Include some necessary headers.
 */
/* Definition of the CCI_Controller class. */
#include <argos3/core/control_interface/ci_controller.h>
/* Definition of the differential steering actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
/* Definition of the foot-bot proximity sensor */
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
/* Definition of the range and bearing actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
/* Definition of the range and bearing sensor */
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
/* Definition of the LEDs actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
/* Definitions for random number generation */
#include <argos3/core/utility/math/rng.h>
/* Include driver interface class */
#include "footbot_sn_driver.h"

/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;

#define FOOTBOT_CLASS_MACRO CFootBotFirefly
#define FOOTBOT_CONTROLLER_MACRO "footbot_firefly_controller"

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class FOOTBOT_CLASS_MACRO : public CCI_Controller {

public:

   /* Class constructor. */
   FOOTBOT_CLASS_MACRO();

   /* Class destructor. */
   virtual ~FOOTBOT_CLASS_MACRO() {}

   /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_diffusion_controller> section.
    */
   virtual void Init(TConfigurationNode& t_node);

   /*
    * This function is called once every time step.
    * The length of the time step is set in the XML file.
    */
   virtual void ControlStep();

   /*
    * This function resets the controller to its state right after the
    * Init().
    * It is called when you press the reset button in the GUI.
    * In this example controller there is no need for resetting anything,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Reset() {}

   /*
    * Called to cleanup what done by Init() when the experiment finishes.
    * In this example controller there is no need for clean anything up,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
   virtual void Destroy() {}

   /*
    * Callback for get clock
    */
   uint16_t get_footbot_clock();

   /*
    * Callback for get rand
    */
   unsigned char get_footbot_rand();

   /*
    * Callback for set LED
    */
   void set_LED(uint8_t r, uint8_t g, uint8_t b);

   /*
    * Callback for set motor
    */
   void set_motor(int left, int right);


private:
   /* Pointer to the differential steering actuator */
   CCI_DifferentialSteeringActuator* m_pcWheels;
   /* Pointer to the LEDs actuator */
   CCI_LEDsActuator* m_pcLEDs;
   /* Pointer to the range and bearing actuator */
   CCI_RangeAndBearingActuator*  m_pcRABA;
   /* Pointer to the range and bearing sensor */
   CCI_RangeAndBearingSensor* m_pcRABS;
   /* Used as a range for uniform number generation */
   CRange<Real> ProbRange;
   /* The random number generator */
   CRandom::CRNG* m_pcRNG;
   /* random id */
   uint8_t my_id;
   /* loop clock */
   uint16_t my_clock;
   /* driver interface object */
   my_footbot_driver_interface * footbot_driver;
};

#endif

/* Include the controller definition */
#include "footbot_simple_swarmnet.h"
/* Function definitions for XML parsing */
#include <argos3/core/utility/configuration/argos_configuration.h>

/****************************************/
/****************************************/

CFootBotSimpleSwarmnet::CFootBotSimpleSwarmnet() :
   m_pcRABA(NULL),
   m_pcRABS(NULL),
   ProbRange(0.0f, 1.0f),
   m_pcRNG(NULL) {}

/****************************************/
/****************************************/

void CFootBotSimpleSwarmnet::Init(TConfigurationNode& t_node) {
   try {
      /*
       * Initialize sensors/actuators
       */
      m_pcRABA      = GetActuator<CCI_RangeAndBearingActuator     >("range_and_bearing"    );
      m_pcRABS      = GetSensor  <CCI_RangeAndBearingSensor       >("range_and_bearing"    );
      m_pcLEDs      = GetActuator<CCI_LEDsActuator                >("leds"                 );
      m_pcWheels    = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
      m_pcRNG = CRandom::CreateRNG("argos");

      my_id = uint8_t(255 * m_pcRNG->Uniform(ProbRange));
      my_clock = 0;
      // printf("buf size = %d\n", m_pcRABA->GetSize());
      // printf("id = %d\n", my_id);
      m_pcRABA->SetData(0, my_id);
      m_pcRABA->SetData(1, my_clock);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing the foot-bot foraging controller for robot \"" << GetId() << "\"", ex);
   }
   footbot_driver = new my_footboot();
   footbot_driver->register_footbot_clock(std::bind(&CFootBotSimpleSwarmnet::get_footbot_clock, this));
   footbot_driver->register_footbot_rand(std::bind(&CFootBotSimpleSwarmnet::get_footbot_rand, this));
   footbot_driver->register_footbot_set_LED(std::bind(&CFootBotSimpleSwarmnet::set_LED, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
   footbot_driver->register_footbot_set_motor(std::bind(&CFootBotSimpleSwarmnet::set_motor, this, std::placeholders::_1, std::placeholders::_2));
   footbot_driver->driver_setup();
}

uint16_t CFootBotSimpleSwarmnet::get_footbot_clock() {
   return my_clock;
}

unsigned char CFootBotSimpleSwarmnet::get_footbot_rand() {
   return (unsigned char)(255 * m_pcRNG->Uniform(ProbRange));;
}

void CFootBotSimpleSwarmnet::set_LED(uint8_t r, uint8_t g, uint8_t b) {
   m_pcLEDs->SetAllColors(CColor(r, g, b));
}

void CFootBotSimpleSwarmnet::set_motor(int left, int right) {
   /*
    * Forward 10 10
    * Left 0 10
    * Right 10 0
    */
   m_pcWheels->SetLinearVelocity(left, right);
}

/****************************************/
/****************************************/

void CFootBotSimpleSwarmnet::ControlStep() {
   // printf("%d recved: [", my_id);
   // for(size_t i = 0; i < tPackets.size(); ++i) {
   //    printf("%d ", tPackets[i].Data[1]);
   // }
   // printf("]\n");
   // m_pcRABA->SetData(1, my_clock);
   // if(my_clock % 10 == 0) m_pcLEDs->SetAllColors(CColor(255, 0, 0));
   // else m_pcLEDs->SetAllColors(CColor(0, 255, 0));
   // printf("%d recved: [", my_id);
   // for(size_t i = 0; i < tPackets.size(); ++i) {
   //    printf("(recv id %d range %f), ", tPackets[i].Data[0], tPackets[i].Range);
   // }
   // printf("]\n");

   /* fetch new packet */
   unsigned char pkt[PKT_SIZE];
   int ret = footbot_driver->next_pkt(pkt);
   for(size_t i = 0; i < ret; i ++) {
      m_pcRABA->SetData(i, pkt[i]);
   }

   /* receive all incoming packets */
   const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
   unsigned char recv_pkt[PKT_SIZE];
   for(size_t i = 0; i < tPackets.size(); ++i) {
      for(size_t j = 0; j < PKT_SIZE; j++) {
         recv_pkt[j] = tPackets[i].Data[j];
      }
      Meta_t meta;
      meta.dist = (int) tPackets[i].Range;
      footbot_driver->recv_pkt(recv_pkt, PKT_SIZE, &meta);
   }
   /* run control loop */
   footbot_driver->driver_loop();
   my_clock++;
}

/****************************************/
/****************************************/

/*
 * This statement notifies ARGoS of the existence of the controller.
 * It binds the class passed as first argument to the string passed as
 * second argument.
 * The string is then usable in the configuration file to refer to this
 * controller.
 * When ARGoS reads that string in the configuration file, it knows which
 * controller class to instantiate.
 * See also the configuration files for an example of how this is used.
 */
REGISTER_CONTROLLER(CFootBotSimpleSwarmnet, "footbot_simple_swarmnet_controller")

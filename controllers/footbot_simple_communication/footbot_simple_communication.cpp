/* Include the controller definition */
#include "footbot_simple_communication.h"
/* Function definitions for XML parsing */
#include <argos3/core/utility/configuration/argos_configuration.h>

/****************************************/
/****************************************/

CFootBotSimpleCommunication::CFootBotSimpleCommunication() :
   m_pcRABA(NULL),
   m_pcRABS(NULL),
   ProbRange(0.0f, 1.0f),
   m_pcRNG(NULL) {}

/****************************************/
/****************************************/

void CFootBotSimpleCommunication::Init(TConfigurationNode& t_node) {
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
      printf("buf size = %d\n", m_pcRABA->GetSize());
      printf("id = %d\n", my_id);
      m_pcRABA->SetData(0, my_id);
      m_pcRABA->SetData(1, my_clock);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing the foot-bot foraging controller for robot \"" << GetId() << "\"", ex);
   }
   m_pcWheels->SetLinearVelocity(0, 10);
}

/****************************************/
/****************************************/

void CFootBotSimpleCommunication::ControlStep() {
   const CCI_RangeAndBearingSensor::TReadings& tPackets = m_pcRABS->GetReadings();
   printf("%d recved: [", my_id);
   for(size_t i = 0; i < tPackets.size(); ++i) {
      printf("%d ", tPackets[i].Data[0]);
   }
   printf("]\n");
   // m_pcRABA->SetData(1, my_clock);
   // if(my_clock % 10 == 0) m_pcLEDs->SetAllColors(CColor(255, 0, 0));
   // else m_pcLEDs->SetAllColors(CColor(0, 255, 0));
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
REGISTER_CONTROLLER(CFootBotSimpleCommunication, "footbot_simple_communication_controller")

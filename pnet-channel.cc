#include <algorithm>
#include "pnet-channel.h"
#include "pnet-phy.h"
#include "pnet-net-device.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-module.h"
#include <cmath>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PnetChannel");

NS_OBJECT_ENSURE_REGISTERED (PnetChannel);



TypeId 
PnetChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PnetChannel")
    .SetParent<Channel> ()
    .AddConstructor<PnetChannel> ()
    .AddAttribute ("Delay", "Transmission delay through the channel",
                   TimeValue (MilliSeconds (2)),
                   MakeTimeAccessor (&PnetChannel::m_delay),
                   MakeTimeChecker ())
      ;
  return tid;
}

PnetChannel::PnetChannel ()
{
  NS_LOG_FUNCTION (this);
}


void
PnetChannel::Send (Ptr<PnetPhy> sender, Ptr<Packet> p, Time duration,double TxPower, double Frequency, double NoisePower)
{
  Ptr<MobilityModel> senderMobility = sender->GetDevice()->GetNode()->GetObject<MobilityModel> ();
 
  NS_LOG_FUNCTION (this );
  uint32_t j = 0;
  for (PhyList::const_iterator i = m_phyList.begin (); i != m_phyList.end (); i++,j++)
    {
      NS_LOG_INFO ("value of j: "<<j);
      Ptr<PnetPhy> tmp = *i;
      if (tmp != sender)
        {
				  NS_LOG_INFO ("node id"<<tmp->GetDevice()->GetNode ()->GetId ());
				  
				  Ptr<MobilityModel> receiverMobility = tmp->GetDevice()->GetNode()->GetObject<MobilityModel>();
				  double dist= senderMobility->GetDistanceFrom (receiverMobility);
                                  double rxPowerDbm = (TxPower*((300000000/Frequency)*(300000000/Frequency)))/((4*3.14*dist)*(4*3.14*dist));
                                  double Sinr= rxPowerDbm/NoisePower;//this is done for the task 3
				  Time delay= Seconds(dist/300000000);//to get the propogation delay for the task 1
				  Simulator::ScheduleWithContext (tmp->GetDevice()->GetNode ()->GetId (), m_delay+delay,
				                                  &PnetChannel::Receive, this, j, p->Copy (), duration,rxPowerDbm,Sinr);
				  
				  
				  NS_LOG_INFO ("additional propagation delay is"<< delay);
				  NS_LOG_FUNCTION ("additional propagation delay is"<< delay);
        }
    }
}

void
PnetChannel::Receive (uint32_t i, Ptr<Packet> packet, Time rxDuration,double rxPowerDbm, double Sinr) const
{
  m_phyList[i]->StartReceivePacket (packet, rxDuration, rxPowerDbm, Sinr);
}

void
PnetChannel::Add (Ptr<PnetPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  m_phyList.push_back (phy);
}

uint32_t
PnetChannel::GetNDevices (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phyList.size ();
}

Ptr<NetDevice>
PnetChannel::GetDevice (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  return m_phyList[i]->GetDevice ()->GetObject<NetDevice> ();
}


} // namespace ns3

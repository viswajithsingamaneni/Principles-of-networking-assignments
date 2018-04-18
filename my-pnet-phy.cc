
#include "my-pnet-phy.h"
#include "pnet-channel.h"
#include "pnet-net-device.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/pointer.h"
#include "ns3/net-device.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/boolean.h"
#include "ns3/node.h"
#include <cmath>
#include <queue>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MyPnetPhy");

NS_OBJECT_ENSURE_REGISTERED (MyPnetPhy);

TypeId
MyPnetPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MyPnetPhy")
    .SetParent<PnetPhy> ()
    .AddConstructor<MyPnetPhy> ()   
    .AddAttribute ("DataRate",
                   "The default data rate for point to point links. Zero means infinite",
                   DataRateValue (DataRate ("0b/s")),
                   MakeDataRateAccessor (&MyPnetPhy::m_bps),
                   MakeDataRateChecker ()) 
    .AddAttribute ("TxPower",
                   "The default Transmission Power",
                   DoubleValue (10),
                   MakeDoubleAccessor (&MyPnetPhy::m_TxPower),
                   MakeDoubleChecker<double> ()) 
    .AddAttribute ("NoisePower",
                   "The default Noise Power",
                   DoubleValue (-174),
                   MakeDoubleAccessor (&MyPnetPhy::m_NoisePower),
                   MakeDoubleChecker<double> ())  
    .AddAttribute ("Frequency",
                   "The Operating Frequency",
                   DoubleValue (10),
                   MakeDoubleAccessor (&MyPnetPhy::m_Frequency),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}
int x=0;
MyPnetPhy::MyPnetPhy ()
{
  NS_LOG_FUNCTION (this);
  
}

MyPnetPhy::~MyPnetPhy ()
{
  NS_LOG_FUNCTION (this);
}

void
MyPnetPhy::SetDevice (Ptr<PnetNetDevice> device)
{
  NS_LOG_FUNCTION (this);
  m_device = device;
}

Ptr<PnetNetDevice>
MyPnetPhy::GetDevice (void) const
{
  return m_device;
}


Ptr<PnetChannel>
MyPnetPhy::GetChannel (void) const
{
  return m_channel;
}

void
MyPnetPhy::SetChannel (Ptr<PnetChannel> channel)
{
  m_channel = channel;
  m_channel->Add (this);
}

void
MyPnetPhy::SetReceiveCallback (PnetPhy::RxCallback callback)
{
  m_rxCallback= callback;
}


void
MyPnetPhy::SendPacket (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this);
  Time txDuration = CalculateTxDuration (packet);
  NS_ASSERT (txDuration > NanoSeconds (0));
  NS_LOG_INFO("Transmitting");
   
  x++;
  m_channel->Send (this, packet, txDuration, m_TxPower, m_Frequency,m_NoisePower);
  Simulator::Schedule (txDuration, &MyPnetPhy::TransmitComplete, this);
}

void
MyPnetPhy::TransmitComplete ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_INFO("IDLE");
  x--;
  return;
}


void
MyPnetPhy::StartReceivePacket (Ptr<Packet> packet, Time rxDuration, double rxPowerDbm, double Sinr)
{
   NS_LOG_FUNCTION (this);
   NS_LOG_INFO("Receiving");

   x++;
   if(x==0)
{
NS_LOG_INFO("no collision");
}
else 
{
NS_LOG_INFO("collision");
NS_LOG_INFO("the received power is "<<rxPowerDbm<<"and the SINR is"<<Sinr);
if (Sinr>10000000000)
{
Simulator::Schedule (rxDuration, &MyPnetPhy::ReceiveComplete, this, packet);
}
}  
}

void
MyPnetPhy::ReceiveComplete (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_INFO("IDLE");
  m_rxCallback (packet);
  return;
}

Time
MyPnetPhy::CalculateTxDuration(Ptr<const Packet> packet)
{
  Time txTime = Time (0);
  if (m_bps > DataRate (0))
    {
      txTime = m_bps.CalculateBytesTxTime (packet->GetSize ());
    }
  return txTime;
}


} //namespace ns3

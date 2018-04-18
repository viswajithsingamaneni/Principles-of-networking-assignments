#include "ns3/uinteger.h"
//#include "ns3/boolean.h"
#include "ns3/assert.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/double.h"
#include "my-pnet-mac.h"
#include "pnet-phy.h"
#include "my-pnet-phy.h"



namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MyPnetMac");

NS_OBJECT_ENSURE_REGISTERED (MyPnetMac);

  

/* static */
TypeId
MyPnetMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MyPnetMac")
    .SetParent<PnetMac> ()
    .AddConstructor<MyPnetMac> ()
    .AddAttribute ("DataRetryLimit",
                   "Maximum Limit for Data Retransmission",
                   UintegerValue (5),
                   MakeUintegerAccessor (&MyPnetMac::m_dataRetryLimit),
                   MakeUintegerChecker<uint16_t> ())
  ;
  return tid;
}
EventId Id;//for task2
MyPnetMac::MyPnetMac ()
{
  NS_LOG_FUNCTION (this);
}

MyPnetMac::~MyPnetMac ()
{
  NS_LOG_FUNCTION (this);
}

void
MyPnetMac::SetPhy (Ptr<PnetPhy> phy)
{
  NS_LOG_FUNCTION (this);
  m_phy = phy;
  m_phy->SetReceiveCallback (MakeCallback (&MyPnetMac::ReceiveFrame, this));
}

Ptr<PnetPhy>
MyPnetMac::GetPhy (void) const
{
  return m_phy;
}

void
MyPnetMac::SetAddress (Mac48Address ad)
{
  NS_LOG_FUNCTION (ad);
  m_address = ad;
}


Mac48Address
MyPnetMac::GetAddress (void) const
{
  return m_address;
}

void
MyPnetMac::SetForwardUpCallback (PnetMac::UpCallback upCallback)
{
  m_forwardUpCb = upCallback;
}

void
MyPnetMac::Enqueue (Ptr<Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this);
  PnetMacHeader header = PnetMacHeader (m_address, to, THZ_PKT_TYPE_DATA);
  packet->AddHeader (header);
  m_queue.push(packet); 
  SendFrame();
}

void
MyPnetMac::SendFrame (void)
{
  NS_LOG_FUNCTION (this);
  
  m_phy->SendPacket (m_queue.front ());
  EventId Id = Simulator::Schedule( Seconds(10),&MyPnetMac::TimeOut,this,10,5);//for task2
  m_queue.pop ();
  return;  
}


  

void
MyPnetMac::ReceiveFrame (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this);
  
  PnetMacHeader header;
  packet->RemoveHeader (header); 
  double check=header.GetType();  
  m_forwardUpCb (packet, header.GetSource (), header.GetDestination ());
  if (check==3)
{

  SendAck(header.GetDestination(), header.GetSource());
}
  else 
{
  NS_LOG_INFO("Acknowledgement is received");
  Id.Cancel();
}	
  return;
}

void
MyPnetMac::SendAck(Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION(this);
  PnetMacHeader header = PnetMacHeader (from, to, THZ_PKT_TYPE_ACK);
  Ptr<Packet> pac = Create<Packet>(6);
  pac->AddHeader (header);
  m_queue.push(pac);
  SendFrame();
}

void 
MyPnetMac::TimeOut(int arg0, int arg1)
{
  NS_LOG_FUNCTION(this);
  NS_LOG_INFO("Time out is called arg0"<<arg0<<"arg1"<<arg1);
  SendFrame();
}
} //namespace ns3

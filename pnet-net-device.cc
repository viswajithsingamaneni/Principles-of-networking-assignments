/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "pnet-net-device.h"
#include "pnet-mac.h"
#include "pnet-phy.h"
#include "pnet-channel.h"
#include "ns3/llc-snap-header.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/error-model.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/simulator.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PnetNetDevice");

NS_OBJECT_ENSURE_REGISTERED (PnetNetDevice);

TypeId 
PnetNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PnetNetDevice")
    .SetParent<NetDevice> ()
    .SetGroupName("Network") 
    .AddConstructor<PnetNetDevice> ()
    .AddAttribute ("PointToPointMode",
                   "The device is configured in Point to Point mode",
                   BooleanValue (false),
                   MakeBooleanAccessor (&PnetNetDevice::m_pointToPointMode),
                   MakeBooleanChecker ())
    
    .AddTraceSource ("PhyRxDrop",
                     "Trace source indicating a packet has been dropped "
                     "by the device during reception",
                     MakeTraceSourceAccessor (&PnetNetDevice::m_phyRxDropTrace),
                     "ns3::Packet::TracedCallback")
  ;
  return tid;
}

PnetNetDevice::PnetNetDevice ()
  : 
    m_mac (0),
    m_phy (0),
    m_node (0),
    m_mtu (0xffff),
    m_ifIndex (0),
    m_configComplete (false),
    m_linkUp (false)   
{
  NS_LOG_FUNCTION (this);
}

PnetNetDevice::~PnetNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
}
void
PnetNetDevice::ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to)
{
  NS_LOG_FUNCTION ("pkt" << packet << "src" << from << "dest" << to);
  LlcSnapHeader llc;
  packet->RemoveHeader (llc);
  enum NetDevice::PacketType packetType;

  if (to == m_mac->GetAddress ())
    {
      packetType = NetDevice::PACKET_HOST;
      NS_LOG_INFO ("packet for me");
    }
  else if (to.IsBroadcast ())
    {
      NS_LOG_INFO ("broadcast packet");
      packetType = NetDevice::PACKET_BROADCAST;
    }
  else if (to.IsGroup ())
    {
      packetType = NetDevice::PACKET_MULTICAST;
    }
  else 
    {
      packetType = NetDevice::PACKET_OTHERHOST;
    }

  if (packetType != NetDevice::PACKET_OTHERHOST)
    {
      m_rxCallback (this, packet, llc.GetType (), from);
    }

  if (!m_promiscCallback.IsNull ())
    {
      NS_LOG_INFO ("llc type: "<<llc.GetType ());
      m_promiscCallback (this, packet, llc.GetType (), from, to, packetType);
    }
}

void
PnetNetDevice::CompleteConfig (void)
{
  NS_LOG_FUNCTION (this);
  if (m_mac == 0
      || m_phy == 0
      || m_node == 0
      || m_channel == 0
      || m_configComplete)
    {
      return;
    }
  m_mac->SetPhy (m_phy);
  m_mac->SetForwardUpCallback (MakeCallback (&PnetNetDevice::ForwardUp, this));
  //m_phy->SetChannel (m_channel);
  m_configComplete = true;
  NS_LOG_INFO ("Everything set");
}

void
PnetNetDevice::SetMac (Ptr<PnetMac> mac)
{
  NS_LOG_FUNCTION (this);
  m_mac = mac;
  CompleteConfig ();
}

void
PnetNetDevice::SetPhy (Ptr<PnetPhy> phy)
{
  NS_LOG_FUNCTION (this);
  m_phy = phy;
  m_phy->SetDevice (Ptr<PnetNetDevice> (this));
  CompleteConfig ();
}

void 
PnetNetDevice::SetChannel (Ptr<PnetChannel> channel)
{
  NS_LOG_FUNCTION (this);
  m_channel = channel;
  CompleteConfig ();
}

void 
PnetNetDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  m_ifIndex = index;
}
uint32_t 
PnetNetDevice::GetIfIndex (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ifIndex;
}
Ptr<Channel> 
PnetNetDevice::GetChannel (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy->GetChannel();
}
void
PnetNetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION (this << address);
  m_address = Mac48Address::ConvertFrom (address);
}
Address 
PnetNetDevice::GetAddress (void) const
{
  //
  // Implicit conversion from Mac48Address to Address
  //
  NS_LOG_FUNCTION (m_mac->GetAddress ());
  return m_mac->GetAddress ();
}
bool 
PnetNetDevice::SetMtu (const uint16_t mtu)
{
  NS_LOG_FUNCTION (this << mtu);
  m_mtu = mtu;
  return true;
}
uint16_t 
PnetNetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_INFO ("The mtu size is"<<m_mtu);
  return m_mtu;
}
bool 
PnetNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION (this);
  return m_linkUp;
}
void 
PnetNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
 NS_LOG_FUNCTION (this << &callback);
 m_linkChangeCallbacks.ConnectWithoutContext (callback);
}
bool 
PnetNetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  if (m_pointToPointMode)
    {
      return false;
    }
  return true;
}
Address
PnetNetDevice::GetBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}
bool 
PnetNetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION (this);
  if (m_pointToPointMode)
    {
      return false;
    }
  return true;
}
Address 
PnetNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_LOG_FUNCTION (this << multicastGroup);
  return Mac48Address::GetMulticast (multicastGroup);
}

Address PnetNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);
  return Mac48Address::GetMulticast (addr);
}

bool 
PnetNetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION (this);
  if (m_pointToPointMode)
    {
      return true;
    }
  return false;
}

bool 
PnetNetDevice::IsBridge (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool 
PnetNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION ("protocol number: "<< protocolNumber);
  
  NS_ASSERT (Mac48Address::IsMatchingType (dest));

  Mac48Address realTo = Mac48Address::ConvertFrom (dest);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  m_mac->Enqueue (packet, realTo);
  return true;
}

bool
PnetNetDevice::SendFrom (Ptr<Packet> p, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << p << source << dest << protocolNumber);
  if (p->GetSize () > GetMtu ())
    {
      NS_LOG_INFO ("The packet is bigger than mtu size");
      return false;
    }
  NS_LOG_INFO ("packet size is alright");
  Ptr<Packet> packet = p->Copy ();

  //Mac48Address to = Mac48Address::ConvertFrom (dest);
  //Mac48Address from = Mac48Address::ConvertFrom (source);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  //m_mac->Enqueue (packet, to, from);    
  NS_LOG_INFO ("mac layer called");    
  return true;
}

Ptr<Node> 
PnetNetDevice::GetNode (void) const
{
  NS_LOG_FUNCTION (this);
  return m_node;
}
void 
PnetNetDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  m_node = node;
  CompleteConfig ();
}
bool 
PnetNetDevice::NeedsArp (void) const
{
  NS_LOG_FUNCTION (this);
  if (m_pointToPointMode)
    {
      return false;
    }
  return true;
}
void 
PnetNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  NS_LOG_FUNCTION (this << &cb);
  m_rxCallback = cb;
}

void
PnetNetDevice::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  //m_channel = 0;
  m_node = 0;
  
  if (TransmitCompleteEvent.IsRunning ())
    {
      TransmitCompleteEvent.Cancel ();
    }
  NetDevice::DoDispose ();
}


void
PnetNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION (this << &cb);
  m_promiscCallback = cb;
}

bool
PnetNetDevice::SupportsSendFrom (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

} // namespace ns3

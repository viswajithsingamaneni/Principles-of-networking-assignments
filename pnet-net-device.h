/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef PNET_NET_DEVICE_H
#define PNET_NET_DEVICE_H

#include <stdint.h>
#include <string>
#include "pnet-mac.h"
#include "ns3/traced-callback.h"
#include "ns3/net-device.h"
#include "ns3/data-rate.h"
#include "ns3/event-id.h"

#include "ns3/mac48-address.h"

namespace ns3 {

class PnetChannel;
class PnetPhy;
class PnetMac;
class Node;
class ErrorModel;



class PnetNetDevice : public NetDevice
{
public:
  
  
  static TypeId GetTypeId (void);
  PnetNetDevice ();

  virtual ~PnetNetDevice ();
  void ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to);
  
  void SetMac (Ptr<PnetMac> mac);
  void SetPhy (Ptr<PnetPhy> phy);
  void SetChannel (Ptr<PnetChannel> channel);

  

  // inherited from NetDevice base class.
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual void SetAddress (Address address);
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void AddLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool IsBridge (void) const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

  virtual Address GetMulticast (Ipv6Address addr) const;

  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;

protected:
  virtual void DoDispose (void);
private:
  void CompleteConfig (void);
  Ptr<PnetMac> m_mac;
  Ptr<PnetPhy> m_phy;
  Ptr<Node> m_node; //!< Node this netDevice is associated to
  Ptr<PnetChannel> m_channel; //!< the channel the device is connected to
  NetDevice::ReceiveCallback m_rxCallback; //!< Receive callback
  NetDevice::PromiscReceiveCallback m_promiscCallback; //!< Promiscuous receive callback
  
  uint16_t m_mtu;   //!< MTU
  uint32_t m_ifIndex; //!< Interface index
  bool m_configComplete;
  Mac48Address m_address; //!< MAC address
  
  TracedCallback<Ptr<const Packet> > m_phyRxDropTrace;

  bool m_linkUp; //!< Flag indicating whether or not the link is up

  bool m_pointToPointMode;

  EventId TransmitCompleteEvent; //!< the Tx Complete event

  TracedCallback<> m_linkChangeCallbacks;
};

} // namespace ns3

#endif /* PNET_NET_DEVICE_H */

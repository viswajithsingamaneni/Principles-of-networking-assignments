#ifndef MY_PNET_PHY_H
#define MY_PNET_PHY_H

#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/data-rate.h"
#include "ns3/mobility-model.h"

#include "pnet-phy.h"
#include "pnet-channel.h"


namespace ns3 {

class PnetNetDevice;

class MyPnetPhy : public PnetPhy
{
public:

  static TypeId GetTypeId (void);

  MyPnetPhy ();
  virtual ~MyPnetPhy ();

  void SetReceiveCallback (PnetPhy::RxCallback callback);
  void SendPacket (Ptr<Packet> packet);
  void TransmitComplete (); 
  void StartReceivePacket (Ptr<Packet> packet, Time rxDuration, double rxPowerDbm,double Sinr);
  void ReceiveComplete (Ptr<Packet> packet);
  
  void SetChannel (Ptr<PnetChannel> channel); 
  void SetDevice (Ptr<PnetNetDevice> device);
  Ptr<PnetNetDevice> GetDevice (void) const;
  Ptr<PnetChannel> GetChannel (void) const;

  Time CalculateTxDuration (Ptr<const Packet> packet);

private:
  Ptr<PnetNetDevice> m_device;
  Ptr<PnetChannel> m_channel;
  Ptr<MobilityModel>   m_mobility;       //!< Pointer to the mobility model
  DataRate m_bps; //!< The physical layer nominal Data rate. Zero means infinite
  double m_TxPower;
  double m_NoisePower;
  double m_Frequency;
  int x;
  PnetPhy::RxCallback m_rxCallback;
  
};

} //namespace ns3

#endif /* MY_PNET_PHY_H */

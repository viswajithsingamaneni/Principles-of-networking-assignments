#ifndef PNET_PHY_H
#define PNET_PHY_H

#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/data-rate.h"
#include "ns3/mobility-model.h"

#include "pnet-channel.h"


namespace ns3 {

class PnetNetDevice;

class PnetPhy : public Object
{
public:
  typedef Callback<void, Ptr<Packet> > RxCallback;
  virtual void SetReceiveCallback (RxCallback callback) =0;
  virtual void SendPacket (Ptr<Packet> packet)=0;
  virtual void TransmitComplete ()=0; 
  virtual void StartReceivePacket (Ptr<Packet> packet, Time rxDuration, double rxPowerDbm,double Sinr)=0;
  virtual void ReceiveComplete (Ptr<Packet> packet)=0;
  
  virtual void SetChannel (Ptr<PnetChannel> channel)=0;
  virtual void SetDevice (Ptr<PnetNetDevice> device)=0;
  virtual Ptr<PnetNetDevice> GetDevice (void) const=0;
  virtual Ptr<PnetChannel> GetChannel (void) const=0;

  virtual Time CalculateTxDuration (Ptr<const Packet> packet)=0;
  
};

} //namespace ns3

#endif /* PNET_PHY_H */

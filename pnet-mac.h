#ifndef PNET_MAC_H
#define PNET_MAC_H

#include "ns3/callback.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "pnet-phy.h"
//#include "ns3/mac48-address.h"

namespace ns3 {

class PnetMac : public Object
{
public:  
  typedef Callback<void,Ptr<Packet>, Mac48Address, Mac48Address> UpCallback;
  virtual void Enqueue (Ptr<Packet> packet, Mac48Address to)= 0;
  
  virtual void SetPhy (Ptr<PnetPhy> phy)= 0;
  virtual void SetForwardUpCallback (UpCallback upCallback)= 0;
  
  virtual void SetAddress (Mac48Address ad)= 0;

  virtual Mac48Address GetAddress (void) const= 0;
  virtual Ptr<PnetPhy> GetPhy (void) const= 0;
  
  virtual void SendFrame (void)= 0; 
  virtual void ReceiveFrame (Ptr<Packet> packet)= 0;

};

} // namespace ns3

#endif /* PNET_MAC_H */

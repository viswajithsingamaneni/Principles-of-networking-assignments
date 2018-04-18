
#ifndef PNET_CHANNEL_H
#define PNET_CHANNEL_H

#include "ns3/channel.h"
#include "ns3/nstime.h"
#include "ns3/mac48-address.h"
#include <vector>
#include <map>

namespace ns3 {

class PnetNetDevice;
class PnetPhy;
class Packet;


class PnetChannel : public Channel
{
public:
  
  static TypeId GetTypeId (void);
  PnetChannel ();

  void Send (Ptr<PnetPhy> sender, Ptr<Packet> p, Time duration,double TxPower, double Frequency,double NoisePower);
  void Receive (uint32_t i, Ptr<Packet> packet, Time rxDuration, double rxPowerDbm,double Sinr) const;

 
  void Add (Ptr<PnetPhy> phy);
  
  // inherited from ns3::Channel
  uint32_t GetNDevices (void) const;
  Ptr<NetDevice> GetDevice (uint32_t i) const;

private:
  typedef std::vector<Ptr<PnetPhy> > PhyList;
  PhyList m_phyList;
  Time m_delay; //!< The assigned speed-of-light delay of the channel
 
};

} // namespace ns3

#endif /* PNET_CHANNEL_H */

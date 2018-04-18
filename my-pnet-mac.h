
#ifndef MY_PNET_MAC_H
#define MY_PNET_MAC_H

#include "ns3/callback.h"
#include "pnet-mac.h"
#include "pnet-mac-header.h"
#include "ns3/nstime.h"
#include "ns3/mac48-address.h"
//#include "ns3/queue.h"
#include <queue>
#include <vector>

namespace ns3 {

class PnetPhy;


class MyPnetMac : public PnetMac
{
public:
  
  static TypeId GetTypeId (void);
  MyPnetMac ();
  virtual~MyPnetMac ();
 
  void Enqueue (Ptr<Packet> packet, Mac48Address to); 
  void SetForwardUpCallback (PnetMac::UpCallback upCallback);
  void SendFrame (void); 
  void ReceiveFrame (Ptr<Packet> packet);
  void SendAck (Mac48Address from, Mac48Address to);//for task2
  void TimeOut (int arg0, int arg1);//for task2
  

  void SetAddress (Mac48Address ad);
  Mac48Address GetAddress (void) const;
  void SetPhy (Ptr<PnetPhy> phy);
  Ptr<PnetPhy> GetPhy (void) const;
  
private:
 
  Ptr<PnetPhy> m_phy;
  Mac48Address m_address;//source address
  
  uint16_t m_dataRetryLimit;
  std::queue<Ptr<Packet> > m_queue;

  PnetMac::UpCallback m_forwardUpCb;
protected:
};

} // namespace ns3

#endif /* MY_PNET_MAC_H */

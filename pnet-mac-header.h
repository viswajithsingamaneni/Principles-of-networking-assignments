/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */


#ifndef PNET_MAC_HEADER_H
#define PNET_MAC_HEADER_H

#include "ns3/header.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/mac48-address.h"

#define THZ_PKT_TYPE_RTS   0
#define THZ_PKT_TYPE_CTS   1
#define THZ_PKT_TYPE_ACK   2
#define THZ_PKT_TYPE_DATA  3

namespace ns3 {

class PnetMacHeader : public Header
{
public:
  PnetMacHeader ();
  
  PnetMacHeader (const Mac48Address srcAddr, const Mac48Address dstAddr, uint8_t type);
  virtual ~PnetMacHeader ();
  
  static TypeId GetTypeId (void);

  void SetSource (Mac48Address addr);
  void SetDestination (Mac48Address addr);
  void SetType (uint8_t type);
  void SetSequence (uint16_t seq);
  
  Mac48Address GetSource () const;
  Mac48Address GetDestination () const;
  uint8_t GetType () const;
  uint32_t GetSize () const;
  uint16_t GetSequence () const;

  // Inherrited methods
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  virtual TypeId GetInstanceTypeId (void) const;
  
private:
  Mac48Address m_srcAddr;
  Mac48Address m_dstAddr;
  uint8_t m_type;
  uint16_t m_sequence;

};

}

#endif // PNET_MAC_HEADER_H

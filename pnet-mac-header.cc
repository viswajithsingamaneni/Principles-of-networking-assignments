/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/address-utils.h"
#include "pnet-mac-header.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("PnetMacHeader");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PnetMacHeader);

PnetMacHeader::PnetMacHeader ()
{
}

PnetMacHeader::PnetMacHeader (const Mac48Address srcAddr, const Mac48Address dstAddr, uint8_t type)
  : Header (),
    m_srcAddr (srcAddr),
    m_dstAddr (dstAddr),
    m_type (type)
{
}

TypeId
PnetMacHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PnetMacHeader")
    .SetParent<Header> ()
    .AddConstructor<PnetMacHeader> ()
  ;
  return tid;
}

TypeId
PnetMacHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

PnetMacHeader::~PnetMacHeader ()
{}

void
PnetMacHeader::SetSource (Mac48Address addr)
{
  m_srcAddr = addr;
}
void
PnetMacHeader::SetDestination (Mac48Address addr)
{
  m_dstAddr = addr;
}
void
PnetMacHeader::SetType (uint8_t type)
{
  m_type = type;
}

void
PnetMacHeader::SetSequence (uint16_t seq)
{
  m_sequence = seq;
}

Mac48Address
PnetMacHeader::GetSource (void) const
{
  return m_srcAddr;
}
Mac48Address
PnetMacHeader::GetDestination (void) const
{
  return m_dstAddr;
}

uint8_t
PnetMacHeader::GetType (void) const
{
  return m_type;
}

uint16_t
PnetMacHeader::GetSequence (void) const
{
  return m_sequence;
}
uint32_t 
PnetMacHeader::GetSize (void) const
{
  uint32_t size = 0;
  switch (m_type)
    {
    case THZ_PKT_TYPE_RTS:
    case THZ_PKT_TYPE_CTS:
      size = sizeof(m_type)  + sizeof(Mac48Address) * 2 ; //+ sizeof(m_duration)+ sizeof(m_rcvCap)
      break; 
    case THZ_PKT_TYPE_ACK:
      size = sizeof(m_type)  + sizeof(Mac48Address) * 2 + sizeof(m_sequence);//+ sizeof(m_duration)
      break;
    case THZ_PKT_TYPE_DATA:
      size = sizeof(m_type)  + sizeof(Mac48Address) * 2 + sizeof(m_sequence);//+ sizeof(m_duration)
      break;
    }
  return size;
}

// Inherrited methods

uint32_t
PnetMacHeader::GetSerializedSize (void) const
{
  return GetSize ();
}

void
PnetMacHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 (m_type);
  //i.WriteHtolsbU16 (m_duration);
  switch (m_type)
    {
    case THZ_PKT_TYPE_RTS:
    case THZ_PKT_TYPE_CTS:
      WriteTo (i, m_srcAddr);
      WriteTo (i, m_dstAddr);
      //i.WriteU16 (m_sequence);
      //i.WriteU32 (m_rcvCap);
      break;
    case THZ_PKT_TYPE_ACK:
      WriteTo (i, m_srcAddr);
      WriteTo (i, m_dstAddr);
      i.WriteU16 (m_sequence);
      break;
    case THZ_PKT_TYPE_DATA:
      WriteTo (i, m_srcAddr);
      WriteTo (i, m_dstAddr);
      i.WriteU16 (m_sequence);
      break;
    }
}

uint32_t
PnetMacHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  
  m_type = i.ReadU8 ();
  //m_duration = i.ReadLsbtohU16 ();
  switch (m_type)
    {
    case THZ_PKT_TYPE_RTS:
    case THZ_PKT_TYPE_CTS:
      ReadFrom (i, m_srcAddr);
      ReadFrom (i, m_dstAddr);
      //m_sequence = i.ReadU16 ();
      //m_rcvCap = i.ReadU32 ();
      break;
    case THZ_PKT_TYPE_ACK:
      ReadFrom (i, m_srcAddr);
      ReadFrom (i, m_dstAddr);
      m_sequence = i.ReadU16 ();
      break;
    case THZ_PKT_TYPE_DATA:
      ReadFrom (i, m_srcAddr);
      ReadFrom (i, m_dstAddr);
      m_sequence = i.ReadU16 ();
      break;
    }
  
  return i.GetDistanceFrom (start);
}

void
PnetMacHeader::Print (std::ostream &os) const
{
  os << "THZ src=" << m_srcAddr << " dest=" << m_dstAddr << " type=" << (uint32_t) m_type;
}


} // namespace ns3

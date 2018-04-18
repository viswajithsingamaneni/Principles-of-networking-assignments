/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 10.1.2.0


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 1;
  uint32_t nCsma2 = 2;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("nCsma2", "Number of \"extra\" CSMA nodes/devices", nCsma2);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;
  nCsma2 = nCsma2 == 0 ? 1 : nCsma2;

  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (0));
  csmaNodes.Create (nCsma);
  
  NodeContainer csmaNodes2;
  csmaNodes2.Add (p2pNodes.Get (1));
  csmaNodes2.Create (nCsma2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("1ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("50Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (5000)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);
  
  NetDeviceContainer csmaDevices2;
  csmaDevices2 = csma.Install (csmaNodes2);

  InternetStackHelper stack;
  stack.Install (csmaNodes);
  
  InternetStackHelper stack2;
  stack2.Install (csmaNodes2);

  Ipv4AddressHelper address;
  address.SetBase ("128.170.2.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("128.170.1.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);
  
  address.SetBase ("128.170.3.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces2;
  csmaInterfaces2 = address.Assign (csmaDevices2);
 
  UdpEchoServerHelper echoServer (17);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes2.Get (nCsma2));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (csmaInterfaces2.GetAddress (nCsma2), 17);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (2));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (4.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (csmaNodes.Get (1));
  clientApps.Start (Seconds (3.0));
  clientApps.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  
  pointToPoint.EnablePcapAll ("point2point");
  csma.EnablePcap ("spy", csmaDevices2.Get (1), true);
  csma.EnablePcap ("Non-promiscuous", csmaDevices.Get (1), false);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

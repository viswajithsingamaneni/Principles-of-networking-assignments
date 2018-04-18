
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/pnet-module.h"
#include "ns3/mobility-module.h"//for task 1
#include "ns3/my-pnet-mac.h"
#include "ns3/my-pnet-phy.h"
#include "ns3/pnet-channel.h"
#include "ns3/my-pnet-mac-helper.h"
#include "ns3/my-pnet-phy-helper.h"
#include "ns3/pnet-channel-helper.h"

#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  Time::SetResolution (Time::NS);
  //LogComponentEnable("Node", LOG_LEVEL_ALL);
  //LogComponentEnable("PnetNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable("MyPnetMac", LOG_LEVEL_ALL);
  LogComponentEnable("MyPnetPhy", LOG_LEVEL_ALL);
  LogComponentEnable("PnetChannel", LOG_LEVEL_ALL);
  
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PnetChannelHelper channel;
  channel.Set ("Delay", StringValue ("2ms"));
  
  MyPnetMacHelper mac = MyPnetMacHelper::Default ();

  MyPnetPhyHelper phy = MyPnetPhyHelper::Default ();
  phy.Set ("DataRate", StringValue ("5Mbps"));
  //PnetMacHelper mac;

  PnetHelper pnet;
  NetDeviceContainer devices;
  devices = pnet.Install (nodes, channel.Create(), phy, mac);

 
  MobilityHelper mobility;//for task 1

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (2),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-100, 100, -100, 100)));
  mobility.Install (nodes);



  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (256));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

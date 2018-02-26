/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Magister Solutions Ltd.
 *
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
 *
 * Author: Mathias Ettinger <mettinger@toulouse.viveris.com>
 */

#include <ns3/log.h>
#include <ns3/simulator.h>
#include <ns3/boolean.h>

#include "satellite-phy-rx-carrier-marsala.h"

NS_LOG_COMPONENT_DEFINE ("SatPhyRxCarrierMarsala");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SatPhyRxCarrierMarsala);

SatPhyRxCarrierMarsala::SatPhyRxCarrierMarsala (uint32_t carrierId,
                                                Ptr<SatPhyRxCarrierConf> carrierConf,
                                                Ptr<SatWaveformConf> waveformConf,
                                                bool randomAccessEnabled)
  : SatPhyRxCarrierPerFrame (carrierId, carrierConf, waveformConf, randomAccessEnabled)
{
  NS_LOG_FUNCTION (this);
}


SatPhyRxCarrierMarsala::~SatPhyRxCarrierMarsala ()
{
  NS_LOG_FUNCTION (this);
}


TypeId
SatPhyRxCarrierMarsala::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SatPhyRxCarrierMarsala")
    .SetParent<SatPhyRxCarrierPerFrame> ()
    .AddTraceSource ("MarsalaCorrelationRx",
                     "Correlate a CRDSA packet replica through Random Access",
                     MakeTraceSourceAccessor (&SatPhyRxCarrierMarsala::m_marsalaCorrelationRxTrace),
                     "ns3::SatPhyRxCarrierPacketProbe::RxStatusCallback")
  ;
  return tid;
}


void
SatPhyRxCarrierMarsala::PerformSicCycles (
  std::vector<SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s>& combinedPacketsForFrame)
{
  NS_LOG_FUNCTION (this);

  do
    {
      SatPhyRxCarrierPerFrame::PerformSicCycles (combinedPacketsForFrame);
    }
  while (PerformMarsala (combinedPacketsForFrame));
}


bool
SatPhyRxCarrierMarsala::CheckReplicaInSlot (
  const std::list<SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s>& slotContent,
  const SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s& packet) const
{
  bool replicaFound = false;

  for (const SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s& currentPacket : slotContent)
    {
      if (IsReplica (packet, currentPacket))
        {
          if (replicaFound)
            {
              NS_FATAL_ERROR ("Found more than one replica in the same slot!");
            }
          replicaFound = true;
        }
    }

  return replicaFound;
}


bool
SatPhyRxCarrierMarsala::PerformMarsala (
  std::vector<SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s>& combinedPacketsForFrame)
{
  NS_LOG_FUNCTION (this);

  const uint32_t nbSlots = GetCrdsaPacketContainer ().size ();
  NS_LOG_INFO ("Number of slots: " << nbSlots);

  std::map<uint32_t, std::list<SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s> >::iterator iter;
  for (iter = GetCrdsaPacketContainer ().begin (); iter != GetCrdsaPacketContainer ().end (); ++iter)
    {
      NS_LOG_INFO ("Iterating slot: " << iter->first);

      std::list<SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s>& slotContent = iter->second;
      if (slotContent.size () < 1)
        {
          NS_FATAL_ERROR ("No packet in slot! This should not happen");
        }

      std::list<SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s>::iterator currentPacket;
      for (currentPacket = slotContent.begin (); currentPacket != slotContent.end (); ++currentPacket)
        {
          NS_LOG_INFO ("Iterating packet in slot: " << currentPacket->ownSlotId);

          // process the packet
          uint32_t packetsInSlotsCount = slotContent.size ();

          for (uint16_t& replicaSlotId : currentPacket->slotIdsForOtherReplicas)
            {
              NS_LOG_INFO ("Processing replica in slot: " << replicaSlotId);

              std::map<uint32_t, std::list<SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s> >::iterator replicaSlot;
              replicaSlot = GetCrdsaPacketContainer ().find (replicaSlotId);
              if (replicaSlot == GetCrdsaPacketContainer ().end ())
                {
                  NS_FATAL_ERROR ("Slot " << replicaSlotId << " not found in frame!");
                }

              if (!CheckReplicaInSlot (replicaSlot->second, *currentPacket))
                {
                  NS_FATAL_ERROR ("Slot " << replicaSlotId << " does not contain a replica of the current packet!");
                }

              packetsInSlotsCount += replicaSlot->second.size ();
            }

          uint32_t otherReplicasCount = currentPacket->slotIdsForOtherReplicas.size ();
          uint32_t replicasCount = 1 + otherReplicasCount;
          // Account for the fact that we use the size of each slot so we must remove each replica
          // ratio = N_interferent / N_replicas = (N_packets_in_slots - N_replicas) / N_replicas
          double ratioOfInterferentPerReplica = (double (packetsInSlotsCount) / replicasCount) - 1;

          double sinr = CalculatePacketCompositeSinr (*currentPacket);
          /*
           * Update link specific SINR trace for the RETURN_FEEDER link. The RETURN_USER
           * link SINR is already updated at the SatPhyRxCarrier::EndRxDataTransparent ()
           * method!
           */
          m_linkSinrTrace (SatUtils::LinearToDb (sinr));

          double correlatedSinr = replicasCount / (ratioOfInterferentPerReplica + (1 / sinr));

          NS_LOG_INFO ("MARSALA correlation computation, Replicas: " << replicasCount <<
                       " Interferents: " << (packetsInSlotsCount - replicasCount) <<
                       " Packet SINR: " << sinr <<
                       " Correlated SINR: " << correlatedSinr);

          currentPacket->phyError = CheckAgainstLinkResults (correlatedSinr, currentPacket->rxParams);

          uint32_t correlations = 1;
          for (uint32_t i = nbSlots - otherReplicasCount; i < nbSlots; ++i)
            {
              correlations *= i;
            }
          m_marsalaCorrelationRxTrace (correlations, currentPacket->sourceAddress, currentPacket->phyError);

          NS_LOG_INFO ("Packet error: " << currentPacket->phyError);

          if (!currentPacket->phyError)
            {
              // Save packet for further processing
              SatPhyRxCarrierPerFrame::crdsaPacketRxParams_s processedPacket = *currentPacket;
              NS_LOG_INFO ("Packet successfully received, removing its interference and processing the replicas");

              slotContent.erase (currentPacket);
              EliminateInterference (iter, processedPacket);
              FindAndRemoveReplicas (processedPacket);
              combinedPacketsForFrame.push_back (processedPacket);

              return true;
            }
        }
    }

  return false;
}


}

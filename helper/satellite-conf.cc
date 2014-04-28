/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions Ltd
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
 * Author: Jani Puttonen <jani.puttonen@magister.fi>
 */

#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/enum.h"
#include "ns3/simulator.h"
#include "ns3/satellite-wave-form-conf.h"
#include "satellite-conf.h"


NS_LOG_COMPONENT_DEFINE ("SatConf");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SatConf);

TypeId
SatConf::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::SatConf")
      .SetParent<Object> ()
      .AddConstructor<SatConf> ()
      .AddAttribute ("FwdFeederLinkBandwidth", "Bandwidth of the forward link.",
                      DoubleValue (2.0e9),
                      MakeDoubleAccessor (&SatConf::m_fwdFeederLinkBandwidthHz),
                      MakeDoubleChecker<double> ())
      .AddAttribute ("FwdFeederLinkBaseFrequency", "Base frequency of the forward feeder link band.",
                      DoubleValue (27.5e9),
                      MakeDoubleAccessor (&SatConf::m_fwdFeederLinkFreqHz),
                      MakeDoubleChecker<double> ())
      .AddAttribute ("RtnFeederLinkBandwidth", "Bandwidth of the return feeder link band.",
                      DoubleValue (2.0e9),
                      MakeDoubleAccessor (&SatConf::m_rtnFeederLinkBandwidthHz),
                      MakeDoubleChecker<double> ())
      .AddAttribute ("RtnFeederLinkBaseFrequency", "Base frequency of the return feeder link band.",
                      DoubleValue (17.7e9),
                      MakeDoubleAccessor (&SatConf::m_rtnFeederLinkFreqHz),
                      MakeDoubleChecker<double> ())
      .AddAttribute ("FwdUserLinkBandwidth", "Bandwidth of the forward user link band.",
                      DoubleValue (0.5e9),
                      MakeDoubleAccessor (&SatConf::m_fwdUserLinkBandwidthHz),
                      MakeDoubleChecker<double> ())
      .AddAttribute ("FwdUserLinkBaseFrequency", "Base frequency of the forward user link band.",
                      DoubleValue (19.7e9),
                      MakeDoubleAccessor (&SatConf::m_fwdUserLinkFreqHz),
                      MakeDoubleChecker<double> ())
      .AddAttribute ("RtnUserLinkBandwidth", "Bandwidth of the return user link band.",
                      DoubleValue (0.5e9),
                      MakeDoubleAccessor (&SatConf::m_rtnUserLinkBandwidthHz),
                      MakeDoubleChecker<double> ())
      .AddAttribute ("RtnUserLinkBaseFrequency", "Base frequency of the return user link band.",
                      DoubleValue (29.5e9),
                      MakeDoubleAccessor (&SatConf::m_rtnUserLinkFreqHz),
                      MakeDoubleChecker<double> ())
      .AddAttribute ("UserLinkChannels", "The number of channels in user link",
                      UintegerValue (4),
                      MakeUintegerAccessor (&SatConf::m_userLinkChannelCount),
                      MakeUintegerChecker<uint32_t> (1))
      .AddAttribute ("FeederLinkChannels", "The number of channels in feeder link",
                      UintegerValue (16),
                      MakeUintegerAccessor (&SatConf::m_feederLinkChannelCount),
                      MakeUintegerChecker<uint32_t> (1))
      .AddAttribute ("SuperFrameConfForSeq0",
                     "Super frame configuration used for super frame sequence 0.",
                      EnumValue (SatSuperframeConf::SUPER_FRAME_CONFIG_0),
                      MakeEnumAccessor (&SatConf::m_SuperFrameConfForSeq0),
                      MakeEnumChecker (SatSuperframeConf::SUPER_FRAME_CONFIG_0, "Configuration_0",
                                       SatSuperframeConf::SUPER_FRAME_CONFIG_1, "Configuration_1",
                                       SatSuperframeConf::SUPER_FRAME_CONFIG_2, "Configuration_2",
                                       SatSuperframeConf::SUPER_FRAME_CONFIG_3, "Configuration_3"))
      .AddAttribute ("FwdCarrierAllocatedBandwidth",
                     "The allocated carrier bandwidth for forward link carriers [Hz].",
                     DoubleValue (0.125e9),
                     MakeDoubleAccessor (&SatConf::m_fwdCarrierAllocatedBandwidthHz),
                     MakeDoubleChecker<double> ())
      .AddAttribute ("FwdCarrierRollOff",
                     "The roll-off factor for forward link carriers.",
                     DoubleValue (0.20),
                     MakeDoubleAccessor (&SatConf::m_fwdCarrierRollOffFactor),
                     MakeDoubleChecker<double> (0.00, 1.00))
      .AddAttribute ("FwdCarrierSpacing",
                     "The carrier spacing factor for forward link carriers.",
                     DoubleValue (0.00),
                     MakeDoubleAccessor (&SatConf::m_fwdCarrierSpacingFactor),
                     MakeDoubleChecker<double> (0.00, 1.00))
;
    return tid;
}

TypeId
SatConf::GetInstanceTypeId (void) const
{
  NS_LOG_FUNCTION (this);

  return GetTypeId();
}

SatConf::SatConf()
 : m_beamCount (0),
   m_gwCount (0),
   m_fwdFeederLinkFreqHz (0.0),
   m_fwdFeederLinkBandwidthHz (0.0),
   m_fwdUserLinkFreqHz (0.0),
   m_fwdUserLinkBandwidthHz (0.0),
   m_rtnFeederLinkFreqHz (0.0),
   m_rtnFeederLinkBandwidthHz (0.0),
   m_rtnUserLinkFreqHz (0.0),
   m_rtnUserLinkBandwidthHz (0.0),
   m_userLinkChannelCount (0),
   m_feederLinkChannelCount (0),
   m_SuperFrameConfForSeq0 (SatSuperframeConf::SUPER_FRAME_CONFIG_0),
   m_fwdCarrierAllocatedBandwidthHz (0.0),
   m_fwdCarrierRollOffFactor (0.0),
   m_fwdCarrierSpacingFactor (0.0)
{
  NS_LOG_FUNCTION (this);

  // Nothing done here
}

void SatConf::Initialize (std::string path, std::string satConf, std::string gwPos, std::string satPos, std::string wfConf)
{
  NS_LOG_FUNCTION (this);

  // Load satellite configuration file
  LoadSatConf (path + satConf);

  // Load GW positions
  LoadGwPos (path + gwPos);

  // Load satellite position
  LoadGeoSatPos (path + satPos);

  Configure (path + wfConf);
}

void
SatConf::Configure (std::string wfConf)
{
  NS_LOG_FUNCTION (this);

  // *** configure forward link ***

  // currently only one carrier in forward link is used.
  double fwdFeederLinkChannelBandwidthHz = m_fwdFeederLinkBandwidthHz / m_feederLinkChannelCount;
  double fwdUserLinkChannelBandwidthHz = m_fwdUserLinkBandwidthHz / m_userLinkChannelCount;

  // channel bandwidths for the forward feeder and user links is expected to be equal
    if ( fwdFeederLinkChannelBandwidthHz != fwdUserLinkChannelBandwidthHz )
      {
        NS_FATAL_ERROR ("Channel bandwidths for forward feeder and user links are not equal!!!");
      }

    if ( m_fwdCarrierAllocatedBandwidthHz > fwdFeederLinkChannelBandwidthHz )
      {
        NS_FATAL_ERROR ("Fwd Link carrier bandwidth exceeds channel bandwidth!!!");
      }

  // create forward link carrier configuration and one carrier pushing just one carrier to container
  // only one carrier supported in forward link currently
  Ptr<SatFwdCarrierConf> fwdCarrierConf = Create<SatFwdCarrierConf> (m_fwdCarrierAllocatedBandwidthHz, m_fwdCarrierRollOffFactor, m_fwdCarrierSpacingFactor );
  m_forwardLinkCarrierConf.push_back (fwdCarrierConf);

  // *** configure return link ***

  double rtnFeederLinkBandwidthHz = m_rtnFeederLinkBandwidthHz / m_feederLinkChannelCount;
  double rtnUserLinkBandwidthHz = m_rtnUserLinkBandwidthHz / m_userLinkChannelCount;

  // bandwidths of the return feeder and user links is expected to be equal
  if ( rtnFeederLinkBandwidthHz != rtnUserLinkBandwidthHz )
    {
      NS_FATAL_ERROR ( "Bandwidths of return feeder and user links are not equal!!!");
    }

  // Create super frame sequence
  m_superframeSeq = CreateObject<SatSuperframeSeq> ();

  // Create a waveform configuration and add it to super frame sequence.
  Ptr<SatWaveformConf> waveFormConf = CreateObject<SatWaveformConf> (wfConf);
  m_superframeSeq->AddWaveformConf (waveFormConf);

  Ptr<SatSuperframeConf> superFrameConf = SatSuperframeConf::CreateSuperframeConf (m_SuperFrameConfForSeq0 );

  superFrameConf->Configure ( rtnUserLinkBandwidthHz, m_superframeSeq->GetTargetDuration(), waveFormConf);
  m_superframeSeq->AddSuperframe (superFrameConf);
}

double
SatConf::GetCarrierFrequencyHz ( SatEnums::ChannelType_t chType, uint32_t freqId, uint32_t carrierId )
{
  NS_LOG_FUNCTION (this << chType << freqId << carrierId);

  double centerFrequencyHz = 0.0;
  double baseFreqHz = 0.0;
  double channelBandwidthHz = 0.0;
  double carrierBandwidthHz = 0.0;

  switch (chType)
  {
    case SatEnums::FORWARD_FEEDER_CH:
      if ( carrierId >= m_forwardLinkCarrierConf.size ())
        {
          NS_FATAL_ERROR ("Fwd Carrier id out of the range!!");
        }

      channelBandwidthHz = m_fwdFeederLinkBandwidthHz / m_feederLinkChannelCount;
      carrierBandwidthHz = m_forwardLinkCarrierConf[carrierId]->GetAllocatedBandwidthInHz ();
      baseFreqHz = m_fwdFeederLinkFreqHz + ( channelBandwidthHz * (freqId - 1) );
      centerFrequencyHz = baseFreqHz + (carrierBandwidthHz * carrierId) + (carrierBandwidthHz / 2);
      break;

    case SatEnums::FORWARD_USER_CH:
      if ( carrierId >= m_forwardLinkCarrierConf.size ())
        {
          NS_FATAL_ERROR ("Fwd Carrier id out of the range!!");
        }

      channelBandwidthHz = m_fwdUserLinkBandwidthHz / m_userLinkChannelCount;
      carrierBandwidthHz = m_forwardLinkCarrierConf[carrierId]->GetAllocatedBandwidthInHz ();
      baseFreqHz = m_fwdUserLinkFreqHz + ( channelBandwidthHz * (freqId - 1) );
      centerFrequencyHz = baseFreqHz + (carrierBandwidthHz * carrierId) + (carrierBandwidthHz / 2);
      break;

    case SatEnums::RETURN_FEEDER_CH:
      channelBandwidthHz = m_rtnFeederLinkBandwidthHz / m_feederLinkChannelCount;
      baseFreqHz = m_rtnFeederLinkFreqHz + ( channelBandwidthHz * (freqId - 1) );
      centerFrequencyHz = baseFreqHz + m_superframeSeq->GetCarrierFrequencyHz (carrierId);
      break;

    case SatEnums::RETURN_USER_CH:
      channelBandwidthHz = m_rtnUserLinkBandwidthHz / m_userLinkChannelCount;
      baseFreqHz = m_rtnUserLinkFreqHz + ( channelBandwidthHz * (freqId - 1) );
      centerFrequencyHz = baseFreqHz + m_superframeSeq->GetCarrierFrequencyHz (carrierId);
      break;

    default:
      NS_ASSERT (false);
      break;
  }

  return centerFrequencyHz;
}

double
SatConf::GetCarrierBandwidthHz ( SatEnums::ChannelType_t chType, uint32_t carrierId, SatEnums::CarrierBandwidthType_t bandwidthType )
{
  NS_LOG_FUNCTION (this << chType << carrierId << bandwidthType);

  double carrierBandwidthHz = 0.0;

  switch (chType)
  {
    case SatEnums::FORWARD_FEEDER_CH:
      carrierBandwidthHz = GetFwdLinkCarrierBandwidthHz (carrierId, bandwidthType);
      break;

    case SatEnums::FORWARD_USER_CH:
      carrierBandwidthHz = GetFwdLinkCarrierBandwidthHz (carrierId, bandwidthType);
      break;

    case SatEnums::RETURN_FEEDER_CH:
      carrierBandwidthHz = m_superframeSeq->GetCarrierBandwidthHz (carrierId, bandwidthType);
      break;

    case SatEnums::RETURN_USER_CH:
      carrierBandwidthHz = m_superframeSeq->GetCarrierBandwidthHz (carrierId, bandwidthType);
      break;

    default:
      NS_ASSERT (false);
      break;
  }

  return carrierBandwidthHz;
}

std::ifstream* SatConf::OpenFile (std::string filePathName)
{
  NS_LOG_FUNCTION (this << filePathName);

  // READ FROM THE SPECIFIED INPUT FILE
  std::ifstream *ifs = new std::ifstream (filePathName.c_str (), std::ifstream::in);

  if (!ifs->is_open ())
    {
      // script might be launched by test.py, try a different base path
      delete ifs;
      filePathName = "../../" + filePathName;
      ifs = new std::ifstream (filePathName.c_str (), std::ifstream::in);

      if (!ifs->is_open ())
        {
          NS_FATAL_ERROR ("The file " << filePathName << " is not found.");
        }
    }
    return ifs;
}

void
SatConf::LoadSatConf (std::string filePathName)
{
  NS_LOG_FUNCTION (this << filePathName);

  // READ FROM THE SPECIFIED INPUT FILE
  std::ifstream *ifs = OpenFile (filePathName);

  uint32_t beamId, userChannelId, gwId, feederChannelId;
  *ifs >> beamId >> userChannelId >> gwId >> feederChannelId;

  while (ifs->good ())
    {
      NS_LOG_DEBUG (this <<
                    " beamId = " << beamId <<
                    ", userChannelId = " << userChannelId <<
                    ", gwId = " << gwId <<
                    ", feederChannelId = " << feederChannelId);

      // Store the values
      std::vector <uint32_t> beamConf;

      beamConf.push_back (beamId);
      beamConf.push_back (userChannelId);
      beamConf.push_back (gwId);
      beamConf.push_back (feederChannelId);

      m_conf.push_back(beamConf);

      // get next row
      *ifs >> beamId >> userChannelId >> gwId >> feederChannelId;
    }

  m_beamCount = m_conf.size ();

  ifs->close ();
  delete ifs;
}

void
SatConf::LoadGwPos (std::string filePathName)
{
  NS_LOG_FUNCTION (this << filePathName);

  // READ FROM THE SPECIFIED INPUT FILE
  std::ifstream *ifs = OpenFile (filePathName);

  double lat, lon, alt;
  *ifs >> lat >> lon >> alt;

  while (ifs->good ())
    {
      NS_LOG_DEBUG (this <<
                    " latitude [deg] = " << lat <<
                    ", longitude [deg] = " << lon <<
                    ", altitude [m] = ");

      // Store the valuesNS_LOG_FUNCTION (this);
      GeoCoordinate coord(lat, lon, alt);
      m_gwPositions.push_back (coord);

      // get next row
      *ifs >> lat >> lon >> alt;
    }

  m_gwCount = m_gwPositions.size ();

  ifs->close ();
  delete ifs;
}

void
SatConf::LoadGeoSatPos (std::string filePathName)
{
  NS_LOG_FUNCTION (this << filePathName);

  // READ FROM THE SPECIFIED INPUT FILE
  std::ifstream *ifs = OpenFile (filePathName);

  double lat, lon, alt;
  *ifs >> lat >> lon >> alt;

  if (ifs->good ())
    {
      NS_LOG_DEBUG (this <<
                    " latitude [deg] = " << lat <<
                    ", longitude [deg] = " << lon <<
                    ", altitude [m] = ");

      m_geoSatPosition = GeoCoordinate (lat,lon,alt);
    }

  ifs->close ();
  delete ifs;
}

uint32_t
SatConf::GetBeamCount() const
{
  NS_LOG_FUNCTION (this);

  return m_beamCount;
}

uint32_t
SatConf::GetGwCount() const
{
  NS_LOG_FUNCTION (this);

  return m_gwCount;
}

std::vector <uint32_t>
SatConf::GetBeamConfiguration (uint32_t beamId) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT((beamId > 0) && (beamId <=  m_beamCount));

  return m_conf[beamId - 1];
}

uint32_t
SatConf::GetRtnLinkCarrierCount () const
{
  NS_LOG_FUNCTION (this);

  return m_superframeSeq->GetCarrierCount();
}

uint32_t
SatConf::GetFwdLinkCarrierCount () const
{
  NS_LOG_FUNCTION (this);

  return m_forwardLinkCarrierConf.size();
}

double
SatConf::GetFwdLinkCarrierBandwidthHz (uint32_t carrierId, SatEnums::CarrierBandwidthType_t bandwidthType) const
{
  NS_LOG_FUNCTION (this);

  double bandwidtHz = 0.0;

  if ( carrierId >= m_forwardLinkCarrierConf.size ())
    {
      NS_FATAL_ERROR ("Fwd Carrier id out of the range!!");
    }

  switch (bandwidthType)
  {
    case SatEnums::ALLOCATED_BANDWIDTH:
      bandwidtHz = m_forwardLinkCarrierConf[carrierId]->GetAllocatedBandwidthInHz ();
      break;

    case SatEnums::OCCUPIED_BANDWIDTH:
      bandwidtHz = m_forwardLinkCarrierConf[carrierId]->GetOccupiedBandwidthInHz ();
      break;

    case SatEnums::EFFECTIVE_BANDWIDTH:
      bandwidtHz = m_forwardLinkCarrierConf[carrierId]->GetEffectiveBandwidthInHz ();
      break;

    default:
      NS_FATAL_ERROR ("Invalid bandwidth type");
      break;
  }

  return bandwidtHz;
}

GeoCoordinate
SatConf::GetGwPosition (uint32_t gwId) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT((gwId > 0) && (gwId <=  m_gwCount));

  return m_gwPositions[gwId - 1];
}

GeoCoordinate
SatConf::GetGeoSatPosition () const
{
  NS_LOG_FUNCTION (this);

  return m_geoSatPosition;
}

} // namespace ns3




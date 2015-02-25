/*
 * Copyright 2015 Aldebaran
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <alvalue/alvalue.h>

#include <ros/serialization.h>
#include <std_msgs/String.h>

#include "info.hpp"

namespace alros
{
namespace publisher
{

InfoPublisher::InfoPublisher( const std::string& name, const std::string& topic, float frequency, const qi::SessionPtr& session )
  : BasePublisher( name, topic, frequency, session ),
    p_memory_( session->service("ALMemory") ),
    has_published_( false )
{
  keys_.push_back("RobotConfig/Head/FullHeadId");
  keys_.push_back("Device/DeviceList/ChestBoard/BodyId");
  keys_.push_back("Device/DeviceList/BatteryFuelGauge/SerialNumber");
  keys_.push_back("Device/DeviceList/BatteryFuelGauge/FirmwareVersion");
  keys_.push_back("RobotConfig/Body/Type");
  keys_.push_back("RobotConfig/Body/BaseVersion");
  keys_.push_back("RobotConfig/Body/Device/LeftArm/Version");
  keys_.push_back("RobotConfig/Body/Device/RightArm/Version");
  keys_.push_back("RobotConfig/Body/Device/Hand/Left/Version");
  keys_.push_back("RobotConfig/Body/Device/Platform/Version");
  keys_.push_back("RobotConfig/Body/Device/Brakes/Version");
  keys_.push_back("RobotConfig/Body/Device/Wheel/Version");
  keys_.push_back("RobotConfig/Body/Version");
  keys_.push_back("RobotConfig/Body/SoftwareRequirement");
  keys_.push_back("RobotConfig/Body/Device/Legs/Version");
  keys_.push_back("RobotConfig/Mode/Slave");

  alvalues_.arraySetSize(keys_.size());
  size_t i = 0;
  for(std::vector<std::string>::const_iterator it = keys_.begin(); it != keys_.end(); ++it, ++i)
    alvalues_[i] = *it;
}

void InfoPublisher::publish()
{
  if (has_published_)
    return;

  AL::ALValue values = p_memory_.call<AL::ALValue>("getListData", alvalues_);
  std_msgs::String msg;
  for(size_t i = 0; i < keys_.size(); ++i)
  {
    msg.data += keys_[i] + ": " + std::string(values[i]);
    if (i != keys_.size()-1)
      msg.data += " ; ";
  }

  pub_.publish(msg);
  has_published_ = true;
}

void InfoPublisher::reset( ros::NodeHandle& nh )
{
  // We latch as we only publish once
  pub_ = nh.advertise<std_msgs::String>( "info", 1, true );

  has_published_ = false;
  is_initialized_ = true;
}

} // publisher
} //alros
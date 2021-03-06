/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2016, Neobotix GmbH
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Neobotix nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

#include <../include/neo_relayboard_v2_node.h>

//#######################
//#### main programm ####
int main(int argc, char** argv)
{
	// initialize ROS, spezify name of node
	ros::init(argc, argv, "neo_relayboard_v2_node");
	neo_relayboardV2_node node;
	if(node.init() != 0) return 1;
	double requestRate = node.getRequestRate();
	ros::Rate r(requestRate); //Cycle-Rate: Frequency of publishing States
    ros::Time cycleStartTime;
    ros::Time cycleEndTime;
    ros::Duration cycleTime;
	while(node.n.ok())
	{
        cycleStartTime = ros::Time::now();

        //Communication
		node.HandleCommunication();

		//RelayBoard
		node.PublishRelayBoardState();
        node.PublishBatteryState();
		node.PublishEmergencyStopStates();

		//Motors
		node.PublishJointStates();

		//IOBoard
        node.PublishIOBoard();

		//USBoard
		node.PublishUSBoardData();

		ros::spinOnce();

        cycleEndTime = ros::Time::now();

        cycleTime = cycleEndTime - cycleStartTime;

        //ROS_INFO("cycleTime: %f", cycleTime.toSec());

		r.sleep();
	}

	return 0;
}


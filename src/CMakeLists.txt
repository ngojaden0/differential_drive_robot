cmake_minimum_required(VERSION 2.8.3)

project(two_wheel_robot)

find_package(catkin REQUIRED COMPONENTS
	std_msgs
)
catkin_package(
	CATKIN_DEPENDS
	rospy
)

find_package(roslaunch)

foreach(dir config launch meshes urdf)
	install(DIRECTORY ${dir}/
		DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach(dir)

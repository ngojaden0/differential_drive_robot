#!/bin/sh
tmux rename-window 'gazebo/slam/nav'
tmux split-window -v
tmux split-window -h
tmux select-pane -t 0
tmux split-window -h
tmux select-pane -t 0
tmux send-keys -t 2 "cd differential_drive_robot/src/launch" Enter
tmux send-keys -t 2 "roslaunch gazebo.launch" Enter
sleep 3
tmux send-keys -t 3 "rosrun gmapping slam_gmapping scan:=scan" Enter
sleep 3
tmux send-keys -t 1 "cd differential_drive_robot/src/launch" Enter
tmux send-keys -t 1 "roslaunch move_base.launch" Enter
tmux new-window
tmux rename-window 'teleop'
tmux split-window -h
tmux select-pane -t 0
tmux split-window -v
sleep 3
tmux send-keys -t 0 "rosrun teleop_twist_keyboard teleop_twist_keyboard.py" Enter
sleep 3
tmux send-keys -t 1 "rviz" Enter

# roomba gesture control

roomba driver とkinect(v1)を組み合わせてジェスチャでroombaを操作する。

`ros-indigo-openni`が必要。

## example
[server]kinectがつながっているPC

```bash
export ROS_HOSTNAME=192.168.11.31
export ROS_MASTER_URI=http://192.168.11.31:11311
```

[client]roombaがつながっているPC

```bash
export ROS_HOSTNAME=192.168.11.30
export ROS_MASTER_URI=http://192.168.11.31:11311
```

起動手順
[server]

```bash
$ roscore
$ roslaunch openni_launch openni.launch 
$ rosrun rviz rviz 
$ rosrun roomba_gesture_ctrl gesture_drive
```

[client]

```bash
$ rosrun roomba_500driver_meiji roomba_500driver_meiji
```

[server]

```bash
$ rosrun openni_tracker openni_tracker
```

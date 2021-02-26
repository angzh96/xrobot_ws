#!/bin/bash

rm -rf ~/.ros/log/*
sleep $1
echo "'roslaunch $2 $3'"
roslaunch $2 $3


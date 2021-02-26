;; Auto-generated. Do not edit!


(when (boundp 'ros_arduino_msgs::CmdDiffVel)
  (if (not (find-package "ROS_ARDUINO_MSGS"))
    (make-package "ROS_ARDUINO_MSGS"))
  (shadow 'CmdDiffVel (find-package "ROS_ARDUINO_MSGS")))
(unless (find-package "ROS_ARDUINO_MSGS::CMDDIFFVEL")
  (make-package "ROS_ARDUINO_MSGS::CMDDIFFVEL"))

(in-package "ROS")
;;//! \htmlinclude CmdDiffVel.msg.html


(defclass ros_arduino_msgs::CmdDiffVel
  :super ros::object
  :slots (_left _right ))

(defmethod ros_arduino_msgs::CmdDiffVel
  (:init
   (&key
    ((:left __left) 0.0)
    ((:right __right) 0.0)
    )
   (send-super :init)
   (setq _left (float __left))
   (setq _right (float __right))
   self)
  (:left
   (&optional __left)
   (if __left (setq _left __left)) _left)
  (:right
   (&optional __right)
   (if __right (setq _right __right)) _right)
  (:serialization-length
   ()
   (+
    ;; float32 _left
    4
    ;; float32 _right
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _left
       (sys::poke _left (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _right
       (sys::poke _right (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _left
     (setq _left (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _right
     (setq _right (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get ros_arduino_msgs::CmdDiffVel :md5sum-) "3a927990ab5d5c3d628e2d52b8533e52")
(setf (get ros_arduino_msgs::CmdDiffVel :datatype-) "ros_arduino_msgs/CmdDiffVel")
(setf (get ros_arduino_msgs::CmdDiffVel :definition-)
      "float32 left
float32 right

")



(provide :ros_arduino_msgs/CmdDiffVel "3a927990ab5d5c3d628e2d52b8533e52")



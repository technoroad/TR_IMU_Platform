
# Experimental feature
  - In the "auto_update_test" branch, I have implemented the following two new features:
    - Monitor the accelerometer and gyro, and if the system is stationary, update the gyro bias.
    - To avoid drift in the attitude angle along the vertical axis relative to gravity,monitor the gyroscope and avoid calculating the attitude angle when in a completely stationary state.
  - We have not yet tested how it will behave when installed on various robots. We would appreciate any information you can provide on this matter.
  - The above functions are turned OFF by default, so please send the following command to turn them ON.
  - The pose estimation library has been removed from the functionality to calculate and remove gyro bias.
~~~
AUTO_UPDATE_ON
STATIONAL_OBSERVE_ON
SAVE_PARAM
~~~
  - To view the command description, please enter "help" below.
~~~
help
~~~

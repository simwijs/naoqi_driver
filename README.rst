Docker Instructions
===========
You can run this naoqi driver in a docker container.
Use a volume link using docker-compose,
e.g.
```
# 'workspace' is just your WORKDIR in your Dockerfile
- ./src/YOUR_PACKAGE:/workspace/YOUR_REPO/src/YOUR_PACKAGE
- ./src/naoqi_driver:/workspace/YOUR_REPO/src/naoqi_driver
- ./src/naoqi_bridge_msgs:/workspace/YOUR_REPO/src/naoqi_bridge_msgs
```
For naoqi 2.5 this Dockerfile would look like the following:
```
FROM ros:foxy
RUN sh \
    -c 'echo "deb http://packages.ros.org/ros/ubuntu `lsb_release -sc` main" \
        > /etc/apt/sources.list.d/ros-latest.list'
RUN wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
RUN apt-get update

# Get and build boost
RUN cd / && wget https://deac-ams.dl.sourceforge.net/project/boost/boost/1.62.0/boost_1_62_0.tar.gz
RUN cd / && tar zxvf boost_1_62_0.tar.gz && rm boost_1_62_0.tar.gz
RUN cd /boost_1_62_0 \
 && ./bootstrap.sh --with-libraries=all --with-toolset=gcc \
 && ./b2 install -j 8

RUN apt-get install ros-foxy-diagnostic-updater
RUN apt-get install ros-foxy-robot-state-publisher

# Install naoqi c++ sdk
RUN python3 -m pip install qibuild
RUN mkdir -p /opt
RUN cd /opt && git clone https://github.com/samiamlabs/libqi.git -b release-2.5
RUN cd /opt/libqi && mkdir build && cd build && cmake .. -DQI_WITH_TESTS=OFF
RUN cd /opt/libqi/build && make -j 8 && make install

RUN cd /opt && git clone https://github.com/aldebaran/libqicore.git -b release-2.5
RUN cd /opt/libqicore && mkdir build && cd build && cmake .. -DQI_WITH_TESTS=OFF
RUN cd /opt/libqicore/build && make -j 8 && make install

# Install dependencies
COPY src/YOUR_PACKAGE src/YOUR_PACKAGE
RUN rosdep install --from-paths src --ignore-src -r -y

# Add sourcing script
RUN echo "\nsource /workspace/liu-home-wreckers/src/lhw_qi/activate" >> /etc/zsh/zshrc

# Will be mounted later (that volume mount, remember?)
RUN rm -rf src

# Link
RUN ldconfig
```

Description
===========

This is a naoqi driver module that bridges with ROS. It publishes
several sensor data as well as the robot position.

On the other hand it enables ROS to call parts of the
NAOqi API.

What it does
============

The **naoqi_driver** module is in charge of providing some
bridge capabilities between ROS and NAOqiOS.

How it works
============

The **naoqi_driver** module is a NAOqi module that also acts
as a ROS node. As there is no **roscore** on the robot, it
needs to be given the IP of the **roscore** in order to be
registered as a node in the ROS processing graph. Usually,
you will start your **roscore** on your local desktop.

Once connected, normal ROS communication is happening between
your robot, running NAOqi OS, and your desktop, running ROS.


For further information, you can go `here <http://ros-naoqi.github.io/naoqi_driver/>`_ or build the doc:

.. code-block:: sh

  cd doc
  doxygen Doxyfile
  sphinx-build -b html ./source/ ./build/


Travis - Continuous Integration
===============================

.. |indigo| image:: https://travis-matrix-badges.herokuapp.com/repos/ros-naoqi/naoqi_driver/branches/master/1
    :alt: Indigo with Ubuntu Trusty
    :target: https://travis-ci.org/ros-naoqi/naoqi_driver/

.. |kinetic| image:: https://travis-matrix-badges.herokuapp.com/repos/ros-naoqi/naoqi_driver/branches/master/2
    :alt: Kinetic with Ubuntu Xenial
    :target: https://travis-ci.org/ros-naoqi/naoqi_driver/

.. |melodic| image:: https://travis-matrix-badges.herokuapp.com/repos/ros-naoqi/naoqi_driver/branches/master/3
    :alt: Melodic with Ubuntu Bionic
    :target: https://travis-ci.org/ros-naoqi/naoqi_driver/

.. |melodic-stretch| image:: https://travis-matrix-badges.herokuapp.com/repos/ros-naoqi/naoqi_driver/branches/master/4
    :alt: Melodic with Debian Stretch
    :target: https://travis-ci.org/ros-naoqi/naoqi_driver/

+-----------------+---------------------+
|   ROS Release   |       status        |
+=================+=====================+
| Melodic-stretch |  |melodic-stretch|  |
+-----------------+---------------------+
| Melodic         |     |melodic|       |
+-----------------+---------------------+
| Kinetic         |     |kinetic|       |
+-----------------+---------------------+
| Indigo          |     |indigo|        |
+-----------------+---------------------+

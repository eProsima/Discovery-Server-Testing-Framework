.. _linux_installation:

Linux installation
##################

The instructions for building the Discovery Server testing framework in a Linux environment are provided in this
page.
The framework builds against `eProsima Fast DDS <https://fast-dds.docs.eprosima.com/en/latest/>`__, so a compatible
version of the library must be available.
Each branch of the framework is attached to a *Fast DDS* release; the correspondence is listed in the
`RELEASE_SUPPORT.md <https://github.com/eProsima/Discovery-Server-Testing-Framework/blob/master/RELEASE_SUPPORT.md>`_ file of the
repository.

*eProsima Fast DDS* dependencies as tinyxml must be accessible, either because *Fast DDS* was build-installed
defining THIRDPARTY=ON or because those libraries have been specifically installed.
The cross-platform tool `colcon <https://colcon.readthedocs.io/en/released/>`__ was chosen to simplify the
build of the several mutually dependent `CMake <https://cmake.org/cmake/help/latest/>`__ projects.
In order to use colcon, `Python3 <https://www.python.org/>`__ and `CMake <https://cmake.org/cmake/help/latest/>`__
must be first installed.


.. contents::
    :local:
    :backlinks: none
    :depth: 2

.. _linux_requirements:

Requirements
************

Building the framework in a Linux environment from sources requires the following tools to be installed in the
system:

* :ref:`linux_cmake_gcc_pip3_wget_git_sl`
* :ref:`linux_python_modules`

.. _linux_cmake_gcc_pip3_wget_git_sl:

CMake, g++, pip3, wget and git
==============================

These packages provide the tools required to build the framework and its dependencies from command line.
Install CMake_, `g++ <https://gcc.gnu.org/>`_, pip3_, wget_ and git_ using the package manager of the appropriate
Linux distribution. For example, on Ubuntu use the command:

.. code-block:: bash

    sudo apt install cmake g++ python3-pip wget git

.. _linux_python_modules:

Python3 modules
===============

The test suite is written in Python3 and needs some modules to run and validate the test cases.
These can be installed using `pip`.

.. code-block:: bash

    pip3 install jsondiff==2.0.0 xmltodict==0.13.0 pandas==3.0.1 psutil xmlschema

.. _dependencies_sl:

Dependencies
************

The framework and *eProsima Fast DDS* have the following dependencies, when installed from binaries in a
Linux environment:

* :ref:`asiotinyxml2_sl`
* :ref:`openssl_sl`

.. _asiotinyxml2_sl:

Asio and TinyXML2 libraries
===========================

Asio is a cross-platform C++ library for network and low-level I/O programming, which provides a consistent
asynchronous model.
TinyXML2 is a simple, small and efficient C++ XML parser.
Install these libraries using the package manager of the appropriate Linux distribution.
For example, on Ubuntu use the command:

.. code-block:: bash

    sudo apt install libasio-dev libtinyxml2-dev

.. _openssl_sl:

OpenSSL
=======

OpenSSL is a robust toolkit for the TLS and SSL protocols and a general-purpose cryptography library.
Install OpenSSL_ using the package manager of the appropriate Linux distribution.
For example, on Ubuntu use the command:

.. code-block:: bash

   sudo apt install libssl-dev


.. _colcon_installation_linux:

Installation steps
******************

colcon_ is a command line tool based on CMake_ aimed at building sets of software packages.
This section explains how to use it to compile the framework and its dependencies.

#. Install the ROS 2 development tools (colcon_ and vcstool_) by executing the following command:

   .. code-block:: bash

       pip3 install -U colcon-common-extensions vcstool

   .. note::

       If this fails due to an Environment Error, add the :code:`--user` flag to the :code:`pip3` installation command.

#.  Create a workspace and download the repos file that will be used to build the framework and its dependencies:

    .. code-block:: bash

        $ mkdir -p discovery-server-ws/src && cd discovery-server-ws
        $ wget https://raw.githubusercontent.com/eProsima/Discovery-Server-Testing-Framework/master/discovery-server.repos
        $ vcs import src < discovery-server.repos

    The
    `discovery-server.repos <https://raw.githubusercontent.com/eProsima/Discovery-Server-Testing-Framework/master/discovery-server.repos>`_
    file is provided in order to profit from vcstool_ capabilities
    to download the needed repositories.

    .. note::

        In order to avoid using vcstool the following repositories should be downloaded from Github into
        the ``discovery-server-ws/src`` directory:

        +-------------------------+--------------------------------------------------------------------+--------+
        | PACKAGE                 | URL                                                                | BRANCH |
        +=========================+====================================================================+========+
        | fastcdr                 | https://github.com/eProsima/Fast-CDR.git                           | master |
        +-------------------------+--------------------------------------------------------------------+--------+
        | fastdds                 | https://github.com/eProsima/Fast-DDS.git                           | master |
        +-------------------------+--------------------------------------------------------------------+--------+
        | discovery_server        | https://github.com/eProsima/Discovery-Server-Testing-Framework.git | master |
        +-------------------------+--------------------------------------------------------------------+--------+
        | foonathan_memory_vendor | https://github.com/eProsima/foonathan_memory_vendor.git            | master |
        +-------------------------+--------------------------------------------------------------------+--------+
        | leethomason/tinyxml2    | https://github.com/leethomason/tinyxml2.git                        | master |
        +-------------------------+--------------------------------------------------------------------+--------+

#.  Finally, use colcon to compile all software.
    Choose the build configuration by declaring ``CMAKE_BUILD_TYPE`` as Debug or Release.
    For this example, the Debug option has been chosen, which would be the choice of advanced users for debugging
    purposes.
    The *Fast DDS* CLI tool is built and installed as well, since part of the test cases drive it.

    .. code-block:: bash

        $ colcon build --base-paths src \
                --packages-up-to discovery-server \
                --cmake-args -DTHIRDPARTY=ON -DCOMPILE_TOOLS=ON -DINSTALL_TOOLS=ON \
                        -DLOG_LEVEL_INFO=ON -DCMAKE_BUILD_TYPE=Debug

    The options available to configure the build of the framework are listed in :ref:`cmake_options`.

.. note::

    Being based on CMake_, it is possible to pass the CMake configuration options to the :code:`colcon build`
    command. For more information on the specific syntax, please refer to the
    `CMake specific arguments <https://colcon.readthedocs.io/en/released/reference/verb/build.html#cmake-specific-arguments>`_
    page of the colcon_ manual.


Run the test suite
******************

Once the framework has been built, the whole set of scenarios can be run with colcon:

.. code-block:: bash

    $ colcon test --base-paths src --packages-select discovery-server \
            --event-handlers=console_direct+ --ctest-args --label-exclude xfail

Refer to :ref:`test_suite` for running individual test cases and for the description of the validators applied to
their output, and to :ref:`usage` for launching the ``discovery-server`` tool by hand.

.. External links

.. _colcon: https://colcon.readthedocs.io/en/released/
.. _CMake: https://cmake.org
.. _pip3: https://docs.python.org/3/installing/index.html
.. _wget: https://www.gnu.org/software/wget/
.. _git: https://git-scm.com/
.. _OpenSSL: https://www.openssl.org/
.. _Gtest: https://github.com/google/googletest
.. _vcstool: https://pypi.org/project/vcstool/

.. _windows_installation:

Windows installation
####################

The instructions for building the Discovery Server testing framework in a Windows environment are provided in this
page.
The framework builds against `eProsima Fast DDS <https://fast-dds.docs.eprosima.com/en/latest/>`__, so a compatible
version of the library must be available.
Each branch of the framework is attached to a *Fast DDS* release; the correspondence is listed in the
`RELEASE_SUPPORT.md <https://github.com/eProsima/Discovery-Server-Testing-Framework/blob/master/RELEASE_SUPPORT.md>`_ file of the
repository.

*eProsima Fast DDS* dependencies as tinyxml must be installed and accessible in the system.
The cross-platform tool `colcon <https://colcon.readthedocs.io/en/released/>`__ was chosen to simplify the
build of the several mutually dependent `CMake <https://cmake.org/cmake/help/latest/>`__ projects.
In order to use colcon, `Python3 <https://www.python.org/>`__ and `CMake <https://cmake.org/cmake/help/latest/>`__
must be first installed.


.. contents::
    :local:
    :backlinks: none
    :depth: 2

.. _windows_requirements:

Requirements
************

Building the framework in a Windows environment from sources requires the following tools to be installed in the
system:

* :ref:`visual_studio_sw`
* :ref:`chocolatey_sw`
* :ref:`windows_cmake_pip3_wget_git_sw`
* :ref:`windows_python_modules`

.. _visual_studio_sw:

Visual Studio
=============

`Visual Studio <https://visualstudio.microsoft.com/>`_ is required to
have a C++ compiler in the system. For this purpose, make sure to check the
:code:`Desktop development with C++` option during the Visual Studio installation process.

If Visual Studio is already installed but the Visual C++ Redistributable packages are not,
open Visual Studio and go to :code:`Tools` -> :code:`Get Tools and Features` and in the :code:`Workloads` tab enable
:code:`Desktop development with C++`. Finally, click :code:`Modify` at the bottom right.

.. _chocolatey_sw:

Chocolatey
==========

Chocolatey is a Windows package manager. It is needed to install some of *eProsima Fast DDS*'s dependencies.
Download and install it directly from the `website <https://chocolatey.org/>`_.

.. _windows_cmake_pip3_wget_git_sw:

CMake, pip3, wget and git
==========================

These packages provide the tools required to build the framework, *eProsima Fast DDS* and its
dependencies from command line.
Download and install CMake_, pip3_, wget_ and git_ by following the instructions detailed in the respective websites.
Once installed, add the path to the executables to the :code:`PATH` from the
*Edit the system environment variables* control panel.

.. _windows_python_modules:

Python3 modules
===============

The test suite is written in Python3 and needs some modules to run and validate the test cases.
These can be installed using `pip`.

.. code-block:: bash

    > pip3 install jsondiff==2.0.0 xmltodict==0.13.0 pandas==3.0.1 psutil xmlschema

Dependencies
************

*eProsima Fast DDS* has the following dependencies, when installed from sources in a Windows environment:

* :ref:`asiotinyxml2_sw`
* :ref:`openssl_sw`

.. _asiotinyxml2_sw:

Asio and TinyXML2 libraries
===========================

Asio is a cross-platform C++ library for network and low-level I/O programming, which provides a consistent
asynchronous model.
TinyXML2 is a simple, small and efficient C++ XML parser.
They can be downloaded directly from the links below:

* `Asio <https://github.com/ros2/choco-packages/releases/download/2020-02-24/asio.1.12.1.nupkg>`_
* `TinyXML2 <https://github.com/ros2/choco-packages/releases/download/2020-02-24/tinyxml2.6.0.0.nupkg>`_

After downloading these packages, open an administrative shell with *PowerShell* and execute the following command:

.. code-block:: bash

    > choco install -y -s <PATH_TO_DOWNLOADS> asio tinyxml2

where :code:`<PATH_TO_DOWNLOADS>` is the folder into which the packages have been downloaded.

.. _openssl_sw:

OpenSSL
=======

OpenSSL is a robust toolkit for the TLS and SSL protocols and a general-purpose cryptography library.
Download and install the latest OpenSSL version for Windows at this
`link <https://slproweb.com/products/Win32OpenSSL.html>`_.
After installing, add the environment variable :code:`OPENSSL_ROOT_DIR` pointing to the installation root directory.

For example:

.. code-block:: bash

   > OPENSSL_ROOT_DIR=C:\Program Files\OpenSSL-Win64


.. _colcon_installation_windows:

Installation steps
******************

colcon_ is a command line tool based on CMake_ aimed at building sets of software packages.
This section explains how to use it to compile the framework and its dependencies.

.. important::

    Run colcon within a Visual Studio prompt. To do so, launch a *Developer Command Prompt* from the
    search engine.

#. Install the ROS 2 development tools (colcon_ and vcstool_) by executing the following command:

   .. code-block:: bash

       > pip3 install -U colcon-common-extensions vcstool

   and add the path to the :code:`vcs` executable to the :code:`PATH` from the
   *Edit the system environment variables* control panel.

   .. note::

       If this fails due to an Environment Error, add the :code:`--user` flag to the :code:`pip3` installation command.

#.  Create a workspace and download the repos file that will be used to build the framework and its dependencies:

    .. code-block:: bash

        > mkdir discovery-server-ws
        > cd discovery-server-ws
        > mkdir src
        > wget https://raw.githubusercontent.com/eProsima/Discovery-Server-Testing-Framework/master/discovery-server.repos
        > vcs import src < discovery-server.repos

    A
    `discovery-server.repos <https://raw.githubusercontent.com/eProsima/Discovery-Server-Testing-Framework/master/discovery-server.repos>`__
    file is available in order to profit from `vcstool <https://github.com/dirk-thomas/vcstool>`__
    capabilities to download the needed repositories.

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


#.  If the generator (compiler) of choice is Visual Studio, launch colcon from a visual studio console.
    Any console can be setup into a visual studio one by executing a batch file.
    For example, in VS2017 is usually
    :code:`C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat`.

#.  Finally, use colcon to compile all software.
    Choose the build configuration by declaring ``CMAKE_BUILD_TYPE`` as Debug or Release.
    For this example, the Debug option has been chosen, which would be the choice of advanced users for debugging
    purposes.
    If using a multi-configuration generator like Visual Studio we recommend to build both in debug and release modes.
    The *Fast DDS* CLI tool is built and installed as well, since part of the test cases drive it.

    .. code-block:: bash

        > colcon build --base-paths src ^
                --packages-up-to discovery-server ^
                --cmake-args -DTHIRDPARTY=ON -DCOMPILE_TOOLS=ON -DINSTALL_TOOLS=ON ^
                        -DLOG_LEVEL_INFO=ON -DCMAKE_BUILD_TYPE=Debug
        > colcon build --base-paths src ^
                --packages-up-to discovery-server ^
                --cmake-args -DTHIRDPARTY=ON -DCOMPILE_TOOLS=ON -DINSTALL_TOOLS=ON ^
                        -DCMAKE_BUILD_TYPE=Release

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

    > colcon test --base-paths src --packages-select discovery-server ^
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

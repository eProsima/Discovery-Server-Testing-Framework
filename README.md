# eProsima Discovery Server Testing Framework

This repository hosts the testing framework used to validate the **Discovery Server** discovery mechanism of
[eProsima Fast DDS](https://fast-dds.docs.eprosima.com/en/latest/).

Discovery Server is a discovery mechanism built into *Fast DDS*, not a separate product.
Its description, configuration and usage are documented in the
[Fast DDS documentation](https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/discovery.html).
Everything in this repository exists to exercise that mechanism and check that it behaves as expected.

## Commercial support

Looking for commercial support? Write us to info@eprosima.com

Find more about us at [eProsima’s webpage](https://eprosima.com/).

-   [Overview](#overview)
-   [Supported platforms](#supported-platforms)
-   [Installation Guide](#installation-guide)
-   [Running the tests](#running-the-tests)
-   [Test configuration files](#test-configuration-files)
-   [Documentation](#documentation)

## Overview

The framework has two parts:

-   **The `discovery-server` tool**: a C++ executable that reads a single XML configuration file and deploys the
    DDS entities described in it (servers, clients — super clients among them — publishers and subscribers),
    creating and destroying them at the times stated in the file.
    The tool listens to the discovery information received by every participant it creates, stores it in a database,
    and dumps *snapshots* of that database at the requested time points.
    A snapshot is the collective knowledge of every deployed participant at a given instant, serialized as XML.

-   **The test suite**: a set of Python scripts (`test/`) that run the tool over each test case, and validate the
    resulting snapshots against the expected discovery state.
    Test cases are declared in `test/configuration/tests_params.json`, their XML configurations live in
    `test/configuration/test_cases`, and the expected results in `test/configuration/test_solutions`.
    The suite is registered with CTest, so the whole set of scenarios runs with a single `ctest` invocation, and it
    is the same suite executed by this repository's CI.

Scenarios covered include single and multiple interconnected servers, server redundancy, remote servers, entity
disposals, lease duration, reconnections, super clients, the `fastdds discovery` CLI tool, the environment variable
setup, UDP and TCP transports, and XTypes.

## Supported platforms

* Linux
* Windows

## Installation Guide

Building the framework requires a compatible version of
[eProsima Fast DDS](https://fast-dds.docs.eprosima.com/en/latest/) (over release 3.0.0).
See [RELEASE_SUPPORT.md](RELEASE_SUPPORT.md) for the *Fast DDS* version matching each branch of this repository.
*Fast DDS* dependencies such as tinyxml must be accessible, either because *Fast DDS* was build-installed defining
`THIRDPARTY=ON|FORCE` or because those libraries have been specifically installed.

The cross-platform tool [colcon](https://colcon.readthedocs.io/en/released/) was chosen to simplify the
build of the several mutually dependent [CMake](https://cmake.org/cmake/help/latest/) projects. In order to use
colcon, [Python3](https://www.python.org/) and [CMake](https://cmake.org/cmake/help/latest/) must be first installed.
Detailed instructions can be found in the
[testing framework documentation](https://eprosima-discovery-server.readthedocs.io/en/latest/).

The test suite is written in Python3 and needs the following modules, which can be installed using `pip`:

```bash
pip3 install jsondiff==2.0.0 xmltodict==0.13.0 pandas==3.0.1 psutil xmlschema
```

A [discovery-server.repos](https://raw.githubusercontent.com/eProsima/Discovery-Server-Testing-Framework/master/discovery-server.repos)
file is available in order to profit from [vcstool](https://github.com/dirk-thomas/vcstool) capabilities to download
the needed repositories.

### Linux

1.  Create a workspace and download the repos file that will be used to build the framework and its dependencies:

    ```bash
    $ mkdir -p discovery-server-ws/src && cd discovery-server-ws
    $ wget https://raw.githubusercontent.com/eProsima/Discovery-Server-Testing-Framework/master/discovery-server.repos
    $ vcs import src < discovery-server.repos
    ```

1.  Use colcon to compile all software.
    Choose the build configuration by declaring ``CMAKE_BUILD_TYPE`` as Debug or Release.
    The `fastdds` CLI tool must be installed too, since part of the test cases drive it.

    ```bash
    $ colcon build --base-paths src \
            --packages-up-to discovery-server \
            --cmake-args -DTHIRDPARTY=ON -DCOMPILE_TOOLS=ON -DINSTALL_TOOLS=ON \
                    -DLOG_LEVEL_INFO=ON -DCMAKE_BUILD_TYPE=Debug
    ```

### Windows

1.  Create a workspace and download the repos file that will be used to build the framework and its dependencies:

    ```bat
    > mkdir discovery-server-ws
    > cd discovery-server-ws
    > mkdir src
    > wget https://raw.githubusercontent.com/eProsima/Discovery-Server-Testing-Framework/master/discovery-server.repos
    > vcs import src < discovery-server.repos
    ```

1.  If the generator (compiler) of choice is Visual Studio, launch colcon from a visual studio console.
    Any console can be setup into a visual studio one by executing a batch file.
    For example, in VS2017 is usually ``C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat``

1.  Use colcon to compile all software.
    If using a multi-configuration generator like Visual Studio we recommend to build both in debug and release modes.

    ```bat
    > colcon build --base-paths src ^
            --packages-up-to discovery-server ^
            --cmake-args -DTHIRDPARTY=ON -DCOMPILE_TOOLS=ON -DINSTALL_TOOLS=ON ^
                    -DLOG_LEVEL_INFO=ON -DCMAKE_BUILD_TYPE=Debug
    > colcon build --base-paths src ^
            --packages-up-to discovery-server ^
            --cmake-args -DTHIRDPARTY=ON -DCOMPILE_TOOLS=ON -DINSTALL_TOOLS=ON ^
                    -DCMAKE_BUILD_TYPE=Release
    ```

---
**NOTE**

In order to avoid using *vcstool*, the following repositories should be downloaded from github into
the `discovery-server-ws/src` directory:

|          PACKAGE          |                                URL                                 |  BRANCH  |
|:--------------------------|:-------------------------------------------------------------------|:--------:|
| fastcdr                   | https://github.com/eProsima/Fast-CDR.git                           |  master  |
| fastdds                   | https://github.com/eProsima/Fast-DDS.git                           |  master  |
| discovery_server          | https://github.com/eProsima/Discovery-Server-Testing-Framework.git |  master  |
| foonathan_memory_vendor   | https://github.com/eProsima/foonathan_memory_vendor.git            |  master  |
| leethomason/tinyxml2      | https://github.com/leethomason/tinyxml2.git                        |  master  |

---

## Running the tests

Every test case is registered with CTest twice, once with the shared memory transport enabled and once with it
disabled, under the name `discovery_server_test.<test_name>.SHM_[ON|OFF]`.

-   Run the whole suite with colcon:

    ```bash
    $ colcon test --base-paths src --packages-select discovery-server \
            --event-handlers=console_direct+ --ctest-args --label-exclude xfail
    ```

-   Or run it with CTest from the build directory.
    Tests are independent from each other (each server uses its own set of ports), so they can be parallelized:

    ```bash
    $ cd build/discovery-server
    $ ctest -j 10 --label-exclude xfail
    ```

    Tests labelled `xfail` are known to be leaky and may fail; CI excludes them.

-   A single scenario can also be launched directly through the test runner, which is handy while debugging.
    The tool binary and the `fastdds` CLI tool are passed as arguments:

    ```bash
    $ cd build/discovery-server/test
    $ python3 run_test.py -e <path/to>/discovery-server-X.X.X \
            -f <path/to>/fastdds \
            -t test_01_trivial \
            -s true -i false --debug --not-remove
    ```

    `-t` accepts several test names or a name pattern, `-s`/`-i` select the shared memory and intraprocess
    configurations, and `--not-remove` keeps the files generated during the execution for inspection.

Each process launched by a test case is checked by a set of validators declared in the test parameters file:

-   `ground_truth_validation`: compares the generated snapshot with the expected one stored in `test_solutions`.
-   `generate_validation`: compares the generated snapshot with one built on the fly from the test description.
-   `count_lines_validation`: compares the number of lines of the generated and expected snapshots.
-   `exit_code_validation` and `stderr_validation`: check the process exit code and its error output.

## Test configuration files

The tool is driven by an XML file whose outermost tag is `DS`, an extension of the *Fast DDS* XML schema that adds
the tags needed to describe a test scenario:

-   `profiles`: plain *Fast DDS* profiles, used to fine-tune the participants deployed by the tool.
    Refer to the
    [Fast DDS documentation](https://fast-dds.docs.eprosima.com/en/latest/fastdds/xml_configuration/making_xml_profiles.html)
    for further information on this element.
-   `servers` and `clients`: the participants to deploy, each one with its `creation_time`, `removal_time` and the
    `publisher` and `subscriber` endpoints it must create.
-   `snapshots`: the instants at which the discovery database must be dumped, and the file where the results are
    written.

The `DS` tag admits a `user_shutdown` attribute that defaults to *true*. Test XML files set
`user_shutdown="false"`, which makes the tool close as soon as the described scenario is fulfilled.

A trivial example, taken from the test suite:

```xml
<DS xmlns="http://www.eprosima.com/XMLSchemas/discovery-server" user_shutdown="false">
  <servers>
    <server name="server" profile_name="UDP server" />
  </servers>
  <clients>
    <client name="client1" profile_name="UDP client"/>
    <client name="client2" profile_name="UDP client"/>
  </clients>
  <snapshots file="./snapshots.xml">
    <snapshot time="3">Check the clients met the server and know each other</snapshot>
  </snapshots>
  <profiles>
    <!-- Fast DDS participant profiles referenced above -->
  </profiles>
</DS>
```

More examples are available under `resources/xml/examples` and `test/configuration/test_cases`, and the full
description of every tag is available in the
[testing framework documentation](https://eprosima-discovery-server.readthedocs.io/en/latest/).

## Documentation

The documentation of this testing framework is hosted on
[Read the Docs](https://eprosima-discovery-server.readthedocs.io/en/latest/), and its sources are kept in the
[docs](docs) directory of this repository.

The documentation is written with [Sphinx](https://www.sphinx-doc.org) and is built as part of the CMake project by
setting the `BUILD_DOCS` option to `ON`. Its Python dependencies are listed in
[docs/requirements.txt](docs/requirements.txt), and the spell checker needs the `enchant` library
(`sudo apt install libenchant-2-2` on Ubuntu):

```bash
$ python3 -m venv docs-venv
$ source docs-venv/bin/activate
$ pip3 install -r src/discovery-server/docs/requirements.txt
$ colcon build --base-paths src \
        --packages-up-to discovery-server \
        --cmake-args -DTHIRDPARTY=ON -DCOMPILE_TOOLS=ON -DINSTALL_TOOLS=ON \
                -DBUILD_DOCS=ON
```

The generated HTML is left in the `docs/html` directory of the build tree and is installed under
`share/doc/discovery-server/html`. The RST style and spelling checks are registered with CTest and can be run with
`ctest -R documentation`. To work on the documentation without building the framework, [docs](docs) is also a CMake
project of its own:

```bash
cmake -S docs -B build-docs && cmake --build build-docs && ctest --test-dir build-docs
```

The documentation of the Discovery Server discovery mechanism itself is part of the
[Fast DDS documentation](https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/discovery.html).

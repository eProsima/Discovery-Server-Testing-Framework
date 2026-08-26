# eProsima Discovery Server Testing Framework

This repository contains the framework used to validate the
[Discovery Server](https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/discovery.html) discovery mechanism
in [eProsima Fast DDS](https://fast-dds.docs.eprosima.com/en/latest/).

Discovery Server is part of Fast DDS, not a separate product. This project exercises that mechanism and verifies
that it behaves as expected.

## Components

- **`discovery-server`**: a C++ tool that deploys DDS entities from an XML scenario and records snapshots of their
  discovery state.
- **Test suite**: Python tests registered with CTest that compare the generated snapshots with the expected state.

The suite covers scenarios such as interconnected and redundant servers, reconnections, super clients, entity
disposal, lease duration, UDP and TCP transports, and XTypes.

## Documentation

Complete installation, configuration, usage, and test-suite guides are available in the
[Discovery Server Testing Framework documentation](https://discovery-server-testing-framework.readthedocs.io/).
The documentation sources are in the [`docs`](docs) directory.

For details about the Discovery Server mechanism itself, see the
[Fast DDS documentation](https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/discovery.html).

## Compatibility

The framework supports Linux and Windows. Each framework release targets a specific Fast DDS version; see
[`RELEASE_SUPPORT.md`](RELEASE_SUPPORT.md) for the compatibility matrix.

## Quick start

The project uses CMake through [colcon](https://colcon.readthedocs.io/en/released/) and
[vcstool](https://github.com/dirk-thomas/vcstool). Follow the full documentation for platform-specific dependencies
and setup.

Create a workspace and fetch the required repositories:

```bash
mkdir -p discovery-server-ws/src
cd discovery-server-ws
wget https://raw.githubusercontent.com/eProsima/Discovery-Server-Testing-Framework/master/discovery-server.repos
vcs import src < discovery-server.repos
```

Build the framework and the Fast DDS CLI tools:

```bash
colcon build --base-paths src \
    --packages-up-to discovery-server \
    --cmake-args -DTHIRDPARTY=ON -DCOMPILE_TOOLS=ON -DINSTALL_TOOLS=ON \
        -DLOG_LEVEL_INFO=ON -DCMAKE_BUILD_TYPE=Debug
```

## Running the tests

Run the complete suite, excluding tests marked as expected failures:

```bash
colcon test --base-paths src \
    --packages-select discovery-server \
    --event-handlers=console_direct+ \
    --ctest-args --label-exclude xfail
```

Tests can also be run directly from the build directory:

```bash
cd build/discovery-server
ctest -j 10 --label-exclude xfail
```

Test scenarios are defined in `test/configuration/test_cases`, their parameters in
`test/configuration/tests_params.json`, and their expected results in `test/configuration/test_solutions`.

## Commercial support

For commercial support, contact [info@eprosima.com](mailto:info@eprosima.com) or visit
[eProsima's website](https://eprosima.com/).

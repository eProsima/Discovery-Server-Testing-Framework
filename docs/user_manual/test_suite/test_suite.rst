.. include:: ../../03-exports/roles.include

.. _test_suite:

Test suite
##########

The test suite is the part of the framework that runs the ``discovery-server`` tool over a collection of scenarios
and checks that the resulting discovery state is the expected one.
It lives in the ``test`` directory of the
`framework repository <https://github.com/eProsima/Discovery-Server-Testing-Framework>`__ and is written in Python3.

.. contents::
    :local:
    :backlinks: none
    :depth: 2

Layout
******

.. list-table::
    :header-rows: 1

    *   - Path
        - Content
    *   - ``test/run_test.py``
        - Test runner. Launches the processes of a test case and applies its validators.
    *   - ``test/configuration/tests_params.json``
        - Declaration of every test case: processes to launch, configuration files, flags, environment variables
          and validators.
    *   - ``test/configuration/tests_params_definition.json``
        - Reference describing every field accepted by the parameters file.
    *   - ``test/configuration/test_cases``
        - XML configuration files deployed by the tool for each test case.
    *   - ``test/configuration/test_solutions``
        - Expected snapshots against which the generated ones are compared.
    *   - ``test/validation``
        - Validators applied to the output of each process.

A test case may be composed of several processes running simultaneously, each one with its own configuration file,
flags, environment variables, creation time and set of validators.
Besides the ``discovery-server`` tool, a process can also run the ``fastdds`` CLI tool, which is why that tool must
be available when the suite is executed.

Running the suite
*****************

Every test case is registered with CTest twice, once with the shared memory transport enabled and once with it
disabled, under the name ``discovery_server_test.<test_name>.SHM_[ON|OFF]``.

-   Run the whole suite with colcon:

    .. code-block:: bash

        $ colcon test --base-paths src --packages-select discovery-server \
                --event-handlers=console_direct+ --ctest-args --label-exclude xfail

-   Or run it with CTest from the build directory.
    Test cases are independent from each other, since each server uses its own set of ports, so they can be
    parallelized:

    .. code-block:: bash

        $ cd build/discovery-server
        $ ctest -j 10 --label-exclude xfail

Test cases labeled ``xfail`` are known to be leaky and may fail; they are excluded from the CI runs.

Running a single test case
**************************

While debugging a scenario it is often more convenient to call the test runner directly.
It takes the path to the tool binary and to the ``fastdds`` CLI tool, and the name of the test case to execute:

.. code-block:: bash

    $ cd build/discovery-server/test
    $ python3 run_test.py -e <path/to>/discovery-server-X.X.X \
            -f <path/to>/fastdds \
            -t test_01_trivial \
            -s true -i false --debug --not-remove

The runner accepts the following arguments:

.. list-table::
    :header-rows: 1

    *   - Argument
        - Description
    *   - :class:`-e` |br| :class:`--exe`
        - Mandatory path to the ``discovery-server`` executable.
    *   - :class:`-p` |br| :class:`--params`
        - Path to the JSON file containing the test parameters.
          Defaults to ``configuration/tests_params.json``.
    *   - :class:`-t` |br| :class:`--test`
        - Name of the test cases to execute, or a pattern matching their names.
    *   - :class:`-f` |br| :class:`--fds`
        - Path to the ``fastdds`` CLI tool.
    *   - :class:`-s` |br| :class:`--shm`
        - Run with the shared memory transport enabled or disabled.
          By default one execution is run with each.
    *   - :class:`-i` |br| :class:`--intraprocess`
        - Run with intraprocess delivery enabled or disabled.
          By default one execution is run with each.
    *   - :class:`-d` |br| :class:`--debug`
        - Print test debugging information.
    *   - :class:`-r` |br| :class:`--not-remove`
        - Keep the files generated during the execution.
    *   - :class:`--force-remove`
        - Remove the generated files whatever the result of the execution is.

Validators
**********

Each process of a test case declares in ``tests_params.json`` the validators that must be applied to its output.
The available validators are:

*   ``ground_truth_validation``: compares the generated :ref:`snapshot <getting_started_snapshots>` with the
    expected one stored in ``test_solutions``.
*   ``generate_validation``: compares the generated snapshot with one built on the fly, which mimics the format and
    content the output should have if the test passes.
*   ``count_lines_validation``: compares the number of lines of the generated and the expected snapshots.
*   ``exit_code_validation``: checks that the process finished with the expected exit code.
*   ``stderr_validation``: checks the number of lines the process wrote to its error output.

A test case passes when every validator of every one of its processes succeeds.

Adding a new test case
**********************

#.  Write the XML configuration file describing the scenario in ``test/configuration/test_cases``, setting
    :code:`user_shutdown="false"` so the tool closes on its own.
    See :ref:`config_files` for the tags available.

#.  Declare the test case in ``test/configuration/tests_params.json``, listing its processes and the validators to
    apply to each of them.

#.  If the case uses ``ground_truth_validation`` or ``count_lines_validation``, store the expected snapshot in
    ``test/configuration/test_solutions``.

#.  Add the test case name to the ``TEST_LIST`` variable in ``test/CMakeLists.txt`` so that it is registered with
    CTest.

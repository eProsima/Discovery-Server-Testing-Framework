.. include:: ../03-exports/roles.include

.. _cmake_options:

CMake options
=============

The Discovery Server testing framework provides some CMake options for changing the behavior and configuration of
the ``discovery-server`` tool. These options allow the user to enable/disable certain settings by defining these
options to ON/OFF at the CMake execution.

.. list-table::
    :header-rows: 1

    *   - Option
        - Description
        - Possible values
        - Default
    *   - :class:`LOG_LEVEL_INFO`
        - Set logging level to Info.
        - ``ON`` |br|
          ``OFF``
        - ``OFF``
    *   - :class:`LOG_LEVEL_WARN`
        - Set logging level to Warning.
        - ``ON`` |br|
          ``OFF``
        - ``OFF``
    *   - :class:`LOG_LEVEL_ERROR`
        - Set logging level to Error.
        - ``ON`` |br|
          ``OFF``
        - ``OFF``
    *   - :class:`SANITIZER`
        - Adds run-time instrumentation to the code. Supported options are:

            - ``Thread`` enables Thread Sanitizer. |br|
            - ``Address`` enables Address Sanitizer.
        - ``OFF`` |br| ``Address`` |br| ``Thread``
        - ``OFF``
    *   - :class:`BUILD_DOCS`
        - Build this documentation together with the framework. |br|
          See :ref:`build_documentation` for the required tools.
        - ``ON`` |br|
          ``OFF``
        - ``OFF``
    *   - :class:`DOCS_BUILDER`
        - Sphinx builder used to render the documentation when
          :class:`BUILD_DOCS` is ``ON``.
        - ``html`` |br| ``dirhtml`` |br| ``singlehtml`` |br| ``latex`` |br|
          ``man`` |br| ``text``
        - ``html``

The test cases are registered with CTest as part of the build, and some of them drive the *Fast DDS* CLI tool.
Build *Fast DDS* with :class:`COMPILE_TOOLS` and :class:`INSTALL_TOOLS` set to ``ON`` so that the ``fastdds``
executable is available when the suite is run.

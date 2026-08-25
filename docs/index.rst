.. eProsima Discovery Server testing framework documentation master file

.. include:: ./03-exports/roles.include


*********************************************************
eProsima Discovery Server Testing Framework Documentation
*********************************************************

.. image:: /01-figures/logo.png
    :height: 100px
    :width: 100px
    :align: left
    :alt: eProsima
    :target: http://www.eprosima.com/

This documentation covers the **Discovery Server testing framework**, the tool suite used to validate the
Discovery Server discovery mechanism of |fastdds|_.

.. important::

    Discovery Server is a discovery mechanism built into |fastdds|_, not a separate product.
    The description, configuration and usage of the discovery mechanism itself are documented in the
    `Fast DDS documentation <https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/discovery.html>`_.
    This documentation only explains how to build, configure and run the framework that tests it.

The framework is made of two parts:

*   The ``discovery-server`` tool, an executable that reads a single XML configuration file and deploys the DDS
    entities described in it (servers, clients, publishers and subscribers), creating and destroying them at the
    stated times.
    The tool listens to the discovery information received by every participant it creates, stores it in a database,
    and dumps *snapshots* of that database, that is, the collective knowledge of every deployed participant at a
    given instant.

*   A Python test suite that runs the tool over each test case and validates the resulting snapshots against the
    expected discovery state. The suite is registered with CTest and is the one executed by the framework's CI.

The source code of the framework, together with the sources of this documentation, is available in the
`Discovery Server GitHub repository <https://github.com/eProsima/Discovery-Server-Testing-Framework>`__.
See :ref:`build_documentation` for rendering these pages locally.

This documentation is organized into the following sections:

* :ref:`installation_manual`
* :ref:`user`
* :ref:`xml_examples`
* :ref:`notes`

.. _installation_manual:

.. toctree::
    :caption: Installation Manual
    :maxdepth: 2
    :numbered: 5
    :hidden:

    installation/installation_linux
    installation/installation_windows
    installation/cmake_options
    installation/build_documentation

.. _user:

.. toctree::
    :caption: User Manual
    :maxdepth: 2
    :numbered: 5
    :hidden:

    user_manual/getting_started/getting_started
    user_manual/usage/command_line
    config_files/config_files
    user_manual/test_suite/test_suite

.. _xml_examples:

.. toctree::
    :caption: XML examples
    :maxdepth: 2
    :numbered: 5
    :hidden:

    xml_profiles/basic_config
    xml_profiles/advanced_config
    xml_profiles/transports

.. _notes:

.. toctree::
    :caption: Release Notes
    :maxdepth: 2
    :hidden:

    notes/notes

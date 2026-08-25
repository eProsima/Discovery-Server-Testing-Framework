.. include:: ../../03-exports/roles.include

.. _getting_started:

Getting started
###############

This section explains how the Discovery Server testing framework works and which pieces it is made of.

.. note::

    The Discovery Server discovery mechanism is a feature of |fastdds|_.
    Its behavior, configuration and usage are described in the
    `Fast DDS documentation <https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/discovery.html>`_,
    which is the reference to consult when writing the participant profiles used by the framework.

.. contents::
    :local:
    :backlinks: none
    :depth: 2

Framework components
********************

The framework is made of two components:

*   The ``discovery-server`` **tool**.
    A C++ executable that takes a single XML configuration file and deploys the DDS entities it describes.
    Servers, clients and their publishers and subscribers are created and destroyed at the instants stated in the
    file, so a complete discovery scenario can be reproduced from a text file without writing any code.

*   The **test suite**.
    A set of Python scripts that run the tool over every test case and validate its output.
    It is registered with CTest, so the whole set of scenarios is launched with a single ``ctest`` invocation.
    See :ref:`test_suite` for details.

.. _getting_started_snapshots:

Snapshots
*********

Whenever the tool creates a participant, either a client or a server, it becomes its *listener*, meaning that all
the discovery information received by that participant is relayed to the tool and stored in a database.

A **snapshot** is a commit of that database at a given time point: the collective knowledge of every deployed
participant at that instant, that is, which participants each one has discovered and which publishers and
subscribers they know about.
Snapshots are serialized as XML, and they are the artifact the test suite validates.

The instants at which snapshots are taken, and the file where they are written, are declared in the ``snapshots``
tag of the configuration file, described in :ref:`config_files`.

Configuration files
*******************

The tool is driven from an XML configuration file whose outermost tag is ``DS``.
This schema extends the *Fast DDS* XML schema with the tags needed to describe a test scenario, and it has two
goals:

-   Simplify the setup of the participants that take part in the scenario.
    Writing a *Fast DDS* participant profile for each server is tiresome given the amount of boilerplate involved,
    so a reduced syntax is provided to declare servers and their connections.

-   Describe the scenario itself: which entities exist, when each one is created and removed, which topics they use,
    and when the discovery state must be captured.
    Testing discovery involves creating a large number of participants, publishers and subscribers over different
    transports, and being able to check the discovery status at several points in time.

The ``DS`` tag admits an optional boolean attribute ``user_shutdown``, which defaults to *true*.
Test configuration files set :code:`user_shutdown="false"`, which makes the tool close as soon as the described
scenario is fulfilled instead of running until the user stops it.

Every tag accepted by the configuration file is described in :ref:`config_files`, and complete examples are
available in :ref:`basic_config_file`, :ref:`advanced_config_file` and :ref:`xml_configuration_examples`.

Participant profiles
********************

The participants deployed by the tool are configured through ordinary *Fast DDS* XML profiles, gathered under the
``profiles`` tag of the configuration file.
The discovery-related settings used by these profiles, such as the discovery protocol of a participant, the list of
servers it connects to or its announcement period, belong to *Fast DDS* and are documented in the
`Discovery Server section <https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/discovery_server.html>`_
and in the
`XML profiles section <https://fast-dds.docs.eprosima.com/en/latest/fastdds/xml_configuration/making_xml_profiles.html>`_
of the *Fast DDS* documentation.

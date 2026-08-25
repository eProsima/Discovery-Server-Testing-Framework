.. include:: ../03-exports/roles.include

.. _config_files:

Configuration files
####################

The operation of the ``discovery-server`` tool is managed from an XML configuration file that follows the
schema shipped in the ``resources/xsd`` directory of the
`framework repository <https://github.com/eProsima/Discovery-Server-Testing-Framework>`__, which is an extension of the
*Fast DDS* XML schema.
The goals of this schema are:

-   Simplify the configuration of the servers taking part in the scenario. Using a *Fast DDS* participant profile
    for each server is tiresome, given the large number of boilerplate code to move around.
    XML syntax extensions are introduced to ease this task.

-   Describe a test scenario in a flexible way.
    Testing the discovery involves creating a large number of participants, publishers, subscribers that use
    specific topics and types (static or dynamic ones) over different transports.
    Besides, all these entities may be instantiated or removed at different times, giving the possibility to check the
    discovery status (collective participant knowledge) at any of these times.

The outermost XML tag is ``DS``.
It admits an optional boolean attribute called ``user_shutdown`` that defaults to
*true*. By default, the tool runs indefinitely until the user decides to shutdown.
Test XML files use :code:`user_shutdown="false"`, which grants that the tool is closed as soon as the test
is fulfilled. The ``DS`` tag can contain the following tags:

+   ``profiles``: is plainly the *Fast DDS* profiles.
    It can be used to fine-tune the server operation.
    Please refer to the
    `Fast DDS documentation <https://fast-dds.docs.eprosima.com/en/latest/fastdds/xml_configuration/making_xml_profiles.html>`_
    for further information on the ``profiles`` element.

+   ``servers``: is a list of servers that the tool must create and setup.
    It must contain at least a ``server`` tag.
    Each server admits the following attributes:

    -   ``name``: non-mandatory but advisable for debugging purposes.
    -   ``prefix``: server unique identifier.
        It is optional because it may be specified in the profile. By using this
        attribute the generation of server profiles that only differ in prefix can be avoided.
    -   ``profile_name``: identifies the profile associated with this server. It is a mandatory.
    -   ``persist``: specifies if the participant is a *SERVER* or a *BACKUP*, as described in the
        `Discovery Server section <https://fast-dds.docs.eprosima.com/en/latest/fastdds/discovery/discovery_server.html>`_
        of the *Fast DDS* documentation.
    -   ``creation_time``: specifies in seconds when a server must be created. It is introduced for testing purposes.
    -   ``removal_time``: specifies in seconds when a server must be destroyed. It is introduced for testing purposes.

    Each server element admits the following tags:

    -   ``ListeningPorts``: contains lists of locators where this server will listen for incoming client metatraffic.
    -   ``ServersList``: contains at least one ``RServer`` tag that references the servers this one wants to link to.
        ``RServer``: only has a prefix attribute. Based on this prefix the tool parser would search for the
        corresponding server locators within the config file.
    -   ``publisher``: introduced for testing purposes. Creates a dummy publisher characterized by ``profile_name``,
        ``topic``, ``creation_time``, and ``removal_time``.
    -   ``subscriber``: introduced for testing purposes. Creates a dummy publisher characterized by ``profile_name``,
        ``topic``, ``creation_time``, and ``removal_time``.

+   ``clients`` introduced for testing purposes.
    It is a list of dummy clients that the tool must create and set up.
    It must contain at least a ``client`` tag.
    Each client admits the following attributes:

    -   ``name``: non-mandatory but advisable for debugging purposes.
    -   ``profile_name``: identifies the profile associated with this server is a mandatory one.
    -   ``server`` specifies the prefix of the server we want to link to.
        This optional attribute saves us the nuisance
        of creating a ``ServerList`` (only if this client references a single server).
        Based on this prefix the tool parser would search for the corresponding server locators within
        the config file.
    -   ``listening_port``: specifies a physical port where to listen for incoming traffic.
        This attribute is mandatory in
        TCP transport (client wouldn't receive other clients traffic without it).
        When using the ``TCPv4`` the format is:
        ``[XXX.XXX.XXX.XXX:]XXXX`` where the IP address is the client's WAN address that must be specified if the
        client has to be reachable from outside a local NAT.
    -   ``creation_time``: specifies in seconds when a server must be created. It is introduced for testing purposes.
    -   ``removal_time``: specifies in seconds when a server must be destroyed. It is introduced for testing purposes.

    Each client element admits the following tags:

    -   ``ServersList`` contains at least one ``RServer`` tag that references the servers this one wants to link to.
        ``RServer`` only has a prefix attribute. Based on this prefix the tool parser would search for the
        corresponding server locators within the config file.
    -   ``publisher`` introduced for testing purposes. Creates a publisher characterized by ``profile_name``,
        ``topic``, ``creation_time``, and ``removal_time``.
    -   ``subscriber`` introduced for testing purposes. Creates a publisher characterized by ``profile_name``,
        ``topic``, ``creation_time``, and ``removal_time``.

+   ``topic``: is plainly the
    `Fast DDS topics <https://fast-dds.docs.eprosima.com/en/latest/fastdds/dds_layer/topic/topic.html>`_.
    It is introduced here for testing purposes to check how topic and type discovery info is handled by EDP.
    It is worth mentioning that only `HelloWorld` type is supported for now.

+   ``types``: configuration not supported for now. Please use ``HelloWorld`` topic type.

+   ``snapshots``: contains ``snapshot`` tags.
    Whenever the tool creates a participant (client or a server) it
    becomes its *listener* in the sense that all discovery info received by the participant is relayed to it.
    The reported discovery info is stored in a database.
    A ``snapshot`` is a *commit* of this database in a given time point.
    The ``snapshots`` element has a ``file`` attribute that must be filled with the filename of the XML results file.
    The ``snapshot`` tag has a single mandatory attribute ``time`` which specifies when the snapshot must be taken.

Complete configuration files are available in :ref:`basic_config_file`, :ref:`advanced_config_file` and
:ref:`xml_configuration_examples`, as well as in the ``test/configuration/test_cases`` directory of the
`framework repository <https://github.com/eProsima/Discovery-Server-Testing-Framework>`__.
The snapshots produced are the artifact checked by the :ref:`test_suite`.

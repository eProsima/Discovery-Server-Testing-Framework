.. _xml_configuration_examples:

Transport protocol configuration
################################

The participants deployed by the ``discovery-server`` tool are configured through ordinary *Fast DDS* profiles, so
any transport supported by the library can be exercised from a configuration file.
The examples below, available in the ``resources/xml/examples`` directory of the
`framework repository <https://github.com/eProsima/Discovery-Server-Testing-Framework>`__, set up a server over each transport.

UDP settings
************

.. literalinclude:: ../02-resources/examples/xml/HelloWorld_UDP_config.xml
    :language: xml
    :linenos:

+   Server prefix is specified.
+   Discovery kind set to SERVER.
+   Metatraffic locators set to the UDP listening port.

.. note::

    ``leaseDuration`` is set to ``INFINITY`` here, but it can take any value without affecting the discovery
    operation.

TCP settings
************

.. literalinclude:: ../02-resources/examples/xml/HelloWorld_TCP_config.xml
    :language: xml
    :linenos:

+   A TCP transport descriptor is created specifying the physical listening port as 9843.
+   The above transport descriptor is added to the participant user transports.
+   Builtin transport is disabled to avoid UDP operation. This wouldn't disturb TCP communication in any way and is
    specified merely to prove that the actual discovery traffic is not going through UDP.
+   Server prefix is specified.
+   Discovery kind set to SERVER.
+   Metatraffic locators set to the logical listening port. The real TCP locator is provided in the transport, this
    one is merely a port number that is linked with this particular server.

UDP and TCP simultaneously
**************************

This configuration generates a server able to listen simultaneously on TCP and UDP ports.
It mixes concepts from the previous UDP and TCP files:

.. literalinclude:: ../02-resources/examples/xml/HelloWorld_UDP_TCP_config.xml
    :language: xml
    :linenos:

+   A TCP transport descriptor is created specifying the physical listening port as 9843.
+   The above transport descriptor is added to the participant user transports.
+   Builtin transport is not disabled in order to allow UDP traffic.
+   Server prefix is specified.
+   Discovery kind set to SERVER.
+   Metatraffic locators set to the logical TCP listening port and UDP actual IP address and listening port.

A server set up this way lets participants discover each other regardless of the transport each one selected, and
not only those sharing the same transport.

The transport descriptors and locators used in these files belong to *Fast DDS*; refer to the
`transport section <https://fast-dds.docs.eprosima.com/en/latest/fastdds/transport/transport.html>`_ of the
*Fast DDS* documentation for the complete description of their settings.

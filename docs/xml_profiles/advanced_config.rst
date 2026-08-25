.. _advanced_config_file:

Advanced XML configuration
**************************

This test case configures an advanced topology in which multiple servers, with publishers and subscribers,
have multiple clients with publishers and subscribers in turn.
It also shows how to create and remove participants and endpoints while the scenario is running, using the
``creation_time`` and ``removal_time`` attributes.
Under the ``snapshots`` tag are specified the times at which the discovery state is captured.

.. literalinclude:: ../02-resources/tests/test_14_disposals_remote_servers.xml
    :language: xml
    :linenos:

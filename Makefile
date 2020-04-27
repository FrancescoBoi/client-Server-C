
all:
	+$(MAKE) -C dserver
	+$(MAKE) -C dclient
	+$(MAKE) -C dconnectionlessServer
	+$(MAKE) -C dconnectionlessClient
	+$(MAKE) -C dimgClient
	+$(MAKE) -C dimgServer
	+$(MAKE) -C imgTransferC/childDB
	+$(MAKE) -C imgTransferC/childP
	+$(MAKE) -C dvidServer
	+$(MAKE) -C dvidClient

clean:
	$(RM) client server
	+$(MAKE) -C dserver clean
	+$(MAKE) -C dclient clean
	+$(MAKE) -C dconnectionlessServer clean
	+$(MAKE) -C dconnectionlessClient clean
	+$(MAKE) -C dimgClient clean
	+$(MAKE) -C dimgServer clean
	+$(MAKE) -C imgTransferC/childDB clean
	+$(MAKE) -C imgTransferC/childP clean
	+$(MAKE) -C dvidServer
	+$(MAKE) -C dvidClient

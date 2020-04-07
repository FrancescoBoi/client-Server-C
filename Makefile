all:
	+$(MAKE) -C common
	+$(MAKE) -C dserver
	+$(MAKE) -C dclient
	+$(MAKE) -C dconnectionlessServer
	+$(MAKE) -C dconnectionlessClient
	+$(MAKE) -C dimgClient
	+$(MAKE) -C dimgServer
	+$(MAKE) -C imgTransferC/childDB
	+$(MAKE) -C imgTransferC/childP

clean:
	$(RM) client server common/obj/* dserver/obj/* dclient/obj/* \
		dconnectionlessClient/obj/* dconnectionlessServer/obj/* \
		dimgClient/obj/* dimgServer/obj/* imgTransferC/childDB/obj/* \
		imgTransferC/childP/obj/*

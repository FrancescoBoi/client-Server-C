all:
	+$(MAKE) -C common
	+$(MAKE) -C dserver
	+$(MAKE) -C dclient
	+$(MAKE) -C dconnectionlessServer
	+$(MAKE) -C dconnectionlessClient
	+$(MAKE) -C dimgClient
	+$(MAKE) -C dimgServer

clean:
	$(RM) client server common/obj/* dserver/obj/* dclient/obj/*

all:
	+$(MAKE) -C common
	+$(MAKE) -C dserver
	+$(MAKE) -C dclient
	+$(MAKE) -C dconnectionlessServer
	+$(MAKE) -C dconnectionlessClient


clean:
	$(RM) client server common/obj/* dserver/obj/* dclient/obj/*

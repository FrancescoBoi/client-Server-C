all:
#	+$(MAKE) -C common
	+$(MAKE) -C dserver
#	+$(MAKE) -C dclient
#	+$(MAKE) -C dconnectionlessServer


clean:
	$(RM) client server common/obj/* dserver/obj/* dclient/obj/*

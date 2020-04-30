all:
	+$(MAKE) -C dconnectionlessServer
	+$(MAKE) -C dconnectionlessClient
clean:
	$(RM) client server
	+$(MAKE) -C dconnectionlessServer clean
	+$(MAKE) -C dconnectionlessClient clean

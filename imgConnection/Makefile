all:
	+$(MAKE) -C dimgServer
	+$(MAKE) -C dimgClient
clean:
	$(RM) client server
	+$(MAKE) -C dimgServer clean
	+$(MAKE) -C dimgClient clean

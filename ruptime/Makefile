all:
	+$(MAKE) -C dserver
	+$(MAKE) -C dclient

clean:
	$(RM) client server
	+$(MAKE) -C dserver clean
	+$(MAKE) -C dclient clean

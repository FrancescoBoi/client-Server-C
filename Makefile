all:
	+$(MAKE) -C ruptime
	+$(MAKE) -C imgConnectionless
	+$(MAKE) -C imgConnection
	+$(MAKE) -C imgTransferC
	+$(MAKE) -C vid

clean:
	+$(MAKE) -C ruptime clean
	+$(MAKE) -C imgConnectionless clean
	+$(MAKE) -C imgConnection clean
	+$(MAKE) -C imgTransferC clean
	+$(MAKE) -C vid clean

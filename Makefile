
all:
	+$(MAKE) -C ruptime
	+$(MAKE) -C imgConnectionless
	+$(MAKE) -C imgConnection
	+$(MAKE) -C imgTransferC/childDB
	+$(MAKE) -C imgTransferC/childP
	+$(MAKE) -C vid

clean:
	+$(MAKE) -C ruptime clean
	+$(MAKE) -C imgConnectionless clean
	+$(MAKE) -C imgConnection clean
	+$(MAKE) -C imgTransferC/childDB clean
	+$(MAKE) -C imgTransferC/childP clean
	+$(MAKE) -C vid clean

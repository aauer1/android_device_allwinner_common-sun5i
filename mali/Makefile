PWD=$(shell pwd)

MALI_DRV_ROOT=DX910-SW-99002-r3p1-01rel0/driver/src/devicedrv/mali
MALI_UMP_ROOT=DX910-SW-99002-r3p1-01rel0/driver/src/devicedrv/ump

install: build
	cp $(MALI_DRV_ROOT)/mali.ko $(PWD)/
	cp $(MALI_UMP_ROOT)/ump.ko $(PWD)/
	

build:
	@echo $(LICHEE_KDIR)
	$(MAKE) -C $(MALI_UMP_ROOT) CONFIG=ca8-virtex820-m400-1 BUILD=release KDIR=${LICHEE_KDIR}
	$(MAKE) -C $(MALI_DRV_ROOT) USING_MMU=1 USING_UMP=1 USING_PMM=1 BUILD=release \
		CONFIG=ca8-virtex820-m400-1 KDIR=${LICHEE_KDIR}



clean:
	$(MAKE) -C $(MALI_UMP_ROOT) CONFIG=ca8-virtex820-m400-1 BUILD=release KDIR=${LICHEE_KDIR} clean
	$(MAKE) -C $(MALI_DRV_ROOT) USING_MMU=1 USING_UMP=1 USING_PMM=1 BUILD=release \
		CONFIG=ca8-virtex820-m400-1 KDIR=${LICHEE_KDIR} clean


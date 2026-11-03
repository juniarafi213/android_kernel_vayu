zram-y				:=	zcomp.o zram_drv.o
zram-$(CONFIG_ZRAM_DEDUP)	+=	zram_dedup.o
zram-$(CONFIG_HYBRIDSWAP) += hybridswap/hybridmain.o
zram-$(CONFIG_HYBRIDSWAP_SWAPD) += hybridswap/hybridswapd.o
zram-$(CONFIG_HYBRIDSWAP_CORE) += hybridswap/hybridswap.o

obj-$(CONFIG_ZRAM)	+=	zram.o

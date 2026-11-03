// SPDX-License-Identifier: GPL-2.0-only
#include <linux/types.h>
#include <linux/export.h>
#include <trace/hooks/vendor_hooks.h>

struct android_vh_hybridswap_table android_vh_hs = { 0 };
EXPORT_SYMBOL_GPL(android_vh_hs);

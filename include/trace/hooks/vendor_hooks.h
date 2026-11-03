/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _TRACE_HOOKS_VENDOR_HOOKS_H
#define _TRACE_HOOKS_VENDOR_HOOKS_H

#include <linux/types.h>

struct mem_cgroup;
struct cgroup_subsys_state;
struct zone;

struct android_vh_hybridswap_table {
	void (*mem_cgroup_alloc)(void *data, struct mem_cgroup *memcg);
	void (*mem_cgroup_free)(void *data, struct mem_cgroup *memcg);
	void (*mem_cgroup_css_online)(void *data, struct cgroup_subsys_state *css, struct mem_cgroup *memcg);
	void (*mem_cgroup_css_offline)(void *data, struct cgroup_subsys_state *css, struct mem_cgroup *memcg);
	void (*mem_cgroup_id_remove)(void *data, struct mem_cgroup *memcg);
	void (*rmqueue)(void *data, struct zone *preferred_zone, struct zone *zone,
			unsigned int order, gfp_t gfp_flags, unsigned int alloc_flags, int migratetype);
	void (*tune_scan_type)(void *data, char *scan_balance);
};

extern struct android_vh_hybridswap_table android_vh_hs;

#define DEFINE_VH_REGISTER(name, func_type) \
static inline int register_trace_android_vh_##name(func_type probe, void *data) \
{ \
	android_vh_hs.name = probe; \
	return 0; \
} \
static inline void unregister_trace_android_vh_##name(func_type probe, void *data) \
{ \
	android_vh_hs.name = NULL; \
}

DEFINE_VH_REGISTER(mem_cgroup_alloc, void (*)(void *, struct mem_cgroup *))
DEFINE_VH_REGISTER(mem_cgroup_free, void (*)(void *, struct mem_cgroup *))
DEFINE_VH_REGISTER(mem_cgroup_css_online, void (*)(void *, struct cgroup_subsys_state *, struct mem_cgroup *))
DEFINE_VH_REGISTER(mem_cgroup_css_offline, void (*)(void *, struct cgroup_subsys_state *, struct mem_cgroup *))
DEFINE_VH_REGISTER(mem_cgroup_id_remove, void (*)(void *, struct mem_cgroup *))
DEFINE_VH_REGISTER(rmqueue, void (*)(void *, struct zone *, struct zone *, unsigned int, gfp_t, unsigned int, int))
DEFINE_VH_REGISTER(tune_scan_type, void (*)(void *, char *))

static inline void trace_android_vh_mem_cgroup_alloc(struct mem_cgroup *memcg)
{
	if (android_vh_hs.mem_cgroup_alloc)
		android_vh_hs.mem_cgroup_alloc(NULL, memcg);
}

static inline void trace_android_vh_mem_cgroup_free(struct mem_cgroup *memcg)
{
	if (android_vh_hs.mem_cgroup_free)
		android_vh_hs.mem_cgroup_free(NULL, memcg);
}

static inline void trace_android_vh_mem_cgroup_css_online(struct cgroup_subsys_state *css, struct mem_cgroup *memcg)
{
	if (android_vh_hs.mem_cgroup_css_online)
		android_vh_hs.mem_cgroup_css_online(NULL, css, memcg);
}

static inline void trace_android_vh_mem_cgroup_css_offline(struct cgroup_subsys_state *css, struct mem_cgroup *memcg)
{
	if (android_vh_hs.mem_cgroup_css_offline)
		android_vh_hs.mem_cgroup_css_offline(NULL, css, memcg);
}

static inline void trace_android_vh_mem_cgroup_id_remove(struct mem_cgroup *memcg)
{
	if (android_vh_hs.mem_cgroup_id_remove)
		android_vh_hs.mem_cgroup_id_remove(NULL, memcg);
}

static inline void trace_android_vh_rmqueue(struct zone *preferred_zone, struct zone *zone,
					   unsigned int order, gfp_t gfp_flags,
					   unsigned int alloc_flags, int migratetype)
{
	if (android_vh_hs.rmqueue)
		android_vh_hs.rmqueue(NULL, preferred_zone, zone, order, gfp_flags, alloc_flags, migratetype);
}

static inline void trace_android_vh_tune_scan_type(char *scan_balance)
{
	if (android_vh_hs.tune_scan_type)
		android_vh_hs.tune_scan_type(NULL, scan_balance);
}

#endif /* _TRACE_HOOKS_VENDOR_HOOKS_H */

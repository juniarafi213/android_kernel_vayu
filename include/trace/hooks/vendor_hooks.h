/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _TRACE_HOOKS_VENDOR_HOOKS_H
#define _TRACE_HOOKS_VENDOR_HOOKS_H

#include <linux/types.h>

struct mem_cgroup;
struct cgroup_subsys_state;
struct zone;

typedef void (*android_vh_mem_cgroup_alloc_t)(void *data, struct mem_cgroup *memcg);
typedef void (*android_vh_mem_cgroup_free_t)(void *data, struct mem_cgroup *memcg);
typedef void (*android_vh_mem_cgroup_css_online_t)(void *data, struct cgroup_subsys_state *css, struct mem_cgroup *memcg);
typedef void (*android_vh_mem_cgroup_css_offline_t)(void *data, struct cgroup_subsys_state *css, struct mem_cgroup *memcg);
typedef void (*android_vh_mem_cgroup_id_remove_t)(void *data, struct mem_cgroup *memcg);
typedef void (*android_vh_rmqueue_t)(void *data, struct zone *preferred_zone, struct zone *zone,
				     unsigned int order, gfp_t gfp_flags, unsigned int alloc_flags, int migratetype);
typedef void (*android_vh_tune_scan_type_t)(void *data, char *scan_balance);

struct android_vh_hybridswap_table {
	android_vh_mem_cgroup_alloc_t mem_cgroup_alloc;
	android_vh_mem_cgroup_free_t mem_cgroup_free;
	android_vh_mem_cgroup_css_online_t mem_cgroup_css_online;
	android_vh_mem_cgroup_css_offline_t mem_cgroup_css_offline;
	android_vh_mem_cgroup_id_remove_t mem_cgroup_id_remove;
	android_vh_rmqueue_t rmqueue;
	android_vh_tune_scan_type_t tune_scan_type;
};

extern struct android_vh_hybridswap_table android_vh_hs;

#define DEFINE_VH_REGISTER(name) \
static inline int register_trace_android_vh_##name(android_vh_##name##_t probe, void *data) \
{ \
	android_vh_hs.name = probe; \
	return 0; \
} \
static inline void unregister_trace_android_vh_##name(android_vh_##name##_t probe, void *data) \
{ \
	android_vh_hs.name = NULL; \
}

DEFINE_VH_REGISTER(mem_cgroup_alloc)
DEFINE_VH_REGISTER(mem_cgroup_free)
DEFINE_VH_REGISTER(mem_cgroup_css_online)
DEFINE_VH_REGISTER(mem_cgroup_css_offline)
DEFINE_VH_REGISTER(mem_cgroup_id_remove)
DEFINE_VH_REGISTER(rmqueue)
DEFINE_VH_REGISTER(tune_scan_type)

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

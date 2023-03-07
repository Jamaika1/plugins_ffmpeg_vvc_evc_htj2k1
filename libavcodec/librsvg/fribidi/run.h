/* FriBidi
 * run.h - text run data type
 *
 * Authors:
 *   Behdad Esfahbod, 2001, 2002, 2004
 *   Dov Grobgeld, 1999, 2000
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc
 * Copyright (C) 2001,2002 Behdad Esfahbod
 * Copyright (C) 1999,2000 Dov Grobgeld
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library, in a file named COPYING; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA
 *
 * For licensing issues, contact <fribidi.license@gmail.com>.
 */
#ifndef _RUN_H
#define _RUN_H

#include "fribidi-common.h"

#include "fribidi-common.h"

#include "fribidi-types.h"
#include "fribidi-bidi-types.h"

#include "fribidi-begindecls.h"

typedef struct _FriBidiRunStruct FriBidiRun;

struct _FriBidiRunStruct
{
  FriBidiRun *prev;
  FriBidiRun *next;

  FriBidiStrIndex pos, len;
  FriBidiCharType type;
  FriBidiLevel level;
  FriBidiLevel isolate_level;
  FriBidiBracketType bracket_type;

  /* Additional links for connecting the isolate tree */
  FriBidiRun *prev_isolate, *next_isolate;
};


FriBidiRun *
new_run (
  void
)
     FRIBIDI_GNUC_HIDDEN FRIBIDI_GNUC_MALLOC FRIBIDI_GNUC_WARN_UNUSED;

     FriBidiRun *new_run_list (
  void
)
     FRIBIDI_GNUC_HIDDEN FRIBIDI_GNUC_MALLOC FRIBIDI_GNUC_WARN_UNUSED;

     void free_run_list (
  FriBidiRun *run_list
) FRIBIDI_GNUC_HIDDEN;

     FriBidiRun *run_list_encode_bidi_types (
  const FriBidiCharType *bidi_types,
  const FriBidiBracketType *bracket_types,
  const FriBidiStrIndex len
)
     FRIBIDI_GNUC_HIDDEN FRIBIDI_GNUC_WARN_UNUSED;

     fribidi_boolean shadow_run_list (
  FriBidiRun *base,
  FriBidiRun *over,
  fribidi_boolean preserve_length
)
     FRIBIDI_GNUC_HIDDEN FRIBIDI_GNUC_WARN_UNUSED;


#define swap(a,b) \
	FRIBIDI_BEGIN_STMT \
	void *t; \
	(t) = (a); \
	(a) = (b); \
	(b) = (t); \
	FRIBIDI_END_STMT

#define merge_lists(a,b) \
	FRIBIDI_BEGIN_STMT \
	swap((a)->prev->next, (b)->prev->next); \
	swap((a)->prev, (b)->prev); \
	FRIBIDI_END_STMT

#define delete_node(x) \
	FRIBIDI_BEGIN_STMT \
	(x)->prev->next = (x)->next; \
	(x)->next->prev = (x)->prev; \
	FRIBIDI_END_STMT

#define insert_node_before(x, list) \
	FRIBIDI_BEGIN_STMT \
	(x)->prev = (list)->prev; \
	(list)->prev->next = (x); \
	(x)->next = (list); \
	(list)->prev = (x); \
	FRIBIDI_END_STMT

#define move_node_before(x, list) \
	FRIBIDI_BEGIN_STMT \
	if ((x)->prev) { \
	  delete_node(x); \
	} \
	insert_node_before((x), (list)); \
	FRIBIDI_END_STMT

#define for_run_list(x, list) \
	for ((x) = (list)->next; (x)->type != FRIBIDI_TYPE_SENTINEL; (x) = (x)->next)


#ifdef DEBUG

     void fribidi_validate_run_list (
  FriBidiRun *run_list		/* input run list */
) FRIBIDI_GNUC_HIDDEN;

#else /* !DEBUG */

#define fribidi_validate_run_list(run_list) fribidi_assert(run_list)

#endif /* !DEBUG */

#include "fribidi-enddecls.h"

#endif /* !_RUN_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */

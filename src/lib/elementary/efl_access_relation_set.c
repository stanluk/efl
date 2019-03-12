#ifdef HAVE_CONFIG_H
  #include "elementary_config.h"
#endif

#include <Elementary.h>

typedef struct
{
   Eina_Array relations;
} Efl_Access_Relation_Set_Data;

#if 0
typedef struct
{
   Efl_Access_Relation_Type type;
   Eina_Array *targets;
} Efl_Access_Relation_Group;

static void
_efl_access_relation2_target_add(Efl_Access_Relation *rel, Efl_Access_Object2 *target)
{
   if (_efl_access_relation2_target_contains(rel, target))
     return;

   eina_array_push(rel->targets, target);
}

static void
_efl_access_relation2_target_del(Efl_Access_Relation *rel, Efl_Access_Object2 *target)
{
   eina_array_remove(rel->targets, target);
}

static bool
_efl_access_relation2_target_contains(Efl_Access_Relation *rel, Efl_Access_Object2 *reference_target)
{
   Efl_Access_Object2 *target;
   Eina_Array_Iterator *iter;
   int idx;

   EINA_ARRAY_ITER_NEXT(&pd->relations, idx, target, iter)
     {
        if (target == reference_target)
          return true;
     }
   return false;
}

static _efl_access_relation2_free(Efl_Access_Relation *rel)
{
   eina_array_free(rel->targets);
   free(rel);
}

#if 0
static void
_on_rel_obj_del(void *data, const Efl_Event *event)
{
#if 0
   Efl_Access_Relation_Set_Data *sd = data;
   Efl_Access_Relation *rel;
   Eina_List *l, *l2, *p, *p2;
   Eo *rel_obj;

   EINA_LIST_FOREACH_SAFE(sd->relations, l, l2, rel)
     {
        EINA_LIST_FOREACH_SAFE(rel->objects, p, p2, rel_obj)
          {
             if (rel_obj == event->object)
               rel->objects = eina_list_remove_list(rel->objects, p);
          }
        if (!rel->objects)
          {
             sd->relations = eina_list_remove_list(sd->relations, l);
             free(rel);
          }
     }
#endif
}

#endif
EOLIAN static void
_efl_access_relation_set_relationship_append(Eo *obj EINA_UNUSED, Efl_Access_Relation_Set_Data *sd, Efl_Access_Relation_Type type, Efl_Access_Object *target)
{
   Efl_Access_Relation *rel;
   Eina_Array_Iterator *iter;
   int idx;

   EINA_ARRAY_ITER_NEXT(&pd->relations, idx, rel, iter)
     {
        if (type == rel->type)
          {
             _efl_access_relation2_target_add(rel, target);
             return;
          }
     }

   rel = _efl_access_relation2_new(type);
   if (!rel) return;

   _efl_access_relation2_target_add(rel, target);
   eina_array_push(&pd->relations, rel);
}


EOLIAN static void
_efl_access_relation_set_relationship_remove(Eo *obj EINA_UNUSED, Efl_Access_Relation_Set_Data *sd, Efl_Access_Relation_Type type, const Efl_Access_Object *target_object)
{
   Efl_Access_Relation2 *rel;
   Eina_Array_Iterator *iter;
   int idx;

   EINA_ARRAY_ITER_NEXT(&pd->relations, idx, rel, iter)
     {
        if (type == rel->type)
          {
             _efl_access_relation2_target_del(rel, target_object);
          }
     }
}

EOLIAN static void
_efl_access_relation_set_remove_all(Eo *obj EINA_UNUSED, Efl_Access_Relation_Set_Data *pd)
{
   Efl_Access_Relation2 *rel;
   Eina_Array_Iterator *iter;
   int idx;

   EINA_ARRAY_ITER_NEXT(&pd->relations, idx, rel, iter)
      _efl_access_relation2_free(rel);

   eina_array_flush(&pd->relations);
}

EOLIAN static Eina_Iterator*
_efl_access_relations_set_relations_get(Eo *obj EINA_UNUSED, Efl_Access_Relation_Set_Data *pd)
{
   return eina_array_iterator_new(&pd->relations);
}

#include "efl_access_relation_set.eo.c"

#endif

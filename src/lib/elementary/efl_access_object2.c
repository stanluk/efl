#ifdef HAVE_CONFIG_H
  #include "elementary_config.h"
#endif

#define EFL_ACCESS_CONTEXT_BETA

#include <Elementary.h>
#include <stdbool.h>

#include "elm_widget.h"
#include "elm_priv.h"

typedef struct _Efl_Access_Object_Data
{
   Eina_Inarray relations;
   Eina_Inarray attributes;
   const char *name;
   const char *description;
   const char *translation_domain;
   const char *locale;
   Efl_Access_Object2 *parent;
   Efl_Access_Role role;
} Efl_Access_Object_Data;

EOLIAN static int
_efl_access_object2_index_in_parent_get(const Eo *obj, Efl_Access_Object_Data *pd EINA_UNUSED)
{
   Efl_Access_Object2 *parent = NULL;
   Eina_Array *children;

   parent = efl_access_object2_access_parent_get(obj);
   if (!parent) return -1;

   children = efl_access_object2_access_children_get(parent);
   if (!children) return -1;

   for (unsigned int i = 0; i < children->count; ++i) {
        Efl_Access_Object2 *child = eina_array_data_get(children, i);
        if (obj == child)
          {
             eina_array_free(children);
             return i;
        }
   }
   eina_array_free(children);
   return -1;
}

EOLIAN Eina_Iterator*
_efl_access_object2_attributes_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
   return eina_inarray_iterator_new(&pd->attributes);
}

static void
_attribute_init(Efl_Access_Attribute *attr, const char *key, const char *value)
{
   attr->key = eina_stringshare_add(key);
   attr->value = eina_stringshare_add(value);
}

static void
_attribute_deinit(Efl_Access_Attribute *attr)
{
   eina_stringshare_del(attr->key);
   eina_stringshare_del(attr->value);
}

static bool
_relation_target_exists(Efl_Access_Relation2 *rel, Efl_Access_Object2 *target)
{
   Eina_Array_Iterator iter;
   unsigned int idx;
   Efl_Access_Object2 *obj;

   EINA_ARRAY_ITER_NEXT(rel->targets, idx, obj, iter)
      if (target == obj) return true;

   return false;
}

static void
_relation_target_add(Efl_Access_Relation2 *rel, Efl_Access_Object2 *target)
{
   if (_relation_target_exists(rel, target))
     return;

   eina_array_push(rel->targets, target);
}

static Eina_Bool
_relation_keep_matching_pointer(void *data, void *gdata)
{
   return data == gdata ? EINA_FALSE : EINA_TRUE;
}

static void
_relation_target_del(Efl_Access_Relation2 *rel, Efl_Access_Object2 *target)
{
   if (!_relation_target_exists(rel, target))
     return;

   eina_array_remove(rel->targets, _relation_keep_matching_pointer, target);
}

static Efl_Access_Relation2*
_relation_new(Efl_Access_Relation_Type type)
{
   Efl_Access_Relation2 *rel = calloc(1, sizeof(Efl_Access_Relation2));
   if (!rel) return NULL;

   rel->type = type;
   rel->targets = eina_array_new(2);

   return rel;
}

static void
_relation_free(Efl_Access_Relation2 *rel)
{
   eina_array_free(rel->targets);
   free(rel);
}

EOLIAN void
_efl_access_object2_attribute_add(Eo *obj, Efl_Access_Object_Data *pd, const char *key, const char *value)
{
   Efl_Access_Attribute attr;

   if (efl_access_object2_attribute_exists(obj, key))
     efl_access_object2_attribute_del(obj, key);

   _attribute_init(&attr, key, value);
   eina_inarray_push(&pd->attributes, &attr);
}

static Eina_Bool
_remove_attributes_with_key(const void *container EINA_UNUSED, void *data, void *fdata)
{
   const char *key = fdata;
   Efl_Access_Attribute *attr = data;

   if (!strcmp(key, attr->key))
     {
        _attribute_deinit(attr);
        return EINA_TRUE;
     }
   else
     return EINA_FALSE;
}

EOLIAN void
_efl_access_object2_attribute_del(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd, const char *key)
{
   if (!key) return;

   eina_inarray_foreach_remove(&pd->attributes, _remove_attributes_with_key, key);
}

EOLIAN void
_efl_access_object2_attribute_del_all(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
   Efl_Access_Attribute *attr;

   EINA_INARRAY_FOREACH(&pd->attributes, attr)
      _attribute_deinit(attr);

   eina_inarray_flush(&pd->attributes);
}

EOLIAN Eina_Bool
_efl_access_object2_attribute_exists(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd, const char *key)
{
   Efl_Access_Attribute *attr;
   if (!key) return EINA_FALSE;

   EINA_INARRAY_FOREACH(&pd->attributes, attr)
     {
        if (!strcmp(key, attr->key))
          return EINA_TRUE;
     }

   return EINA_FALSE;
}

EOLIAN static Efl_Access_Role
_efl_access_object2_role_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
   return pd->role;
}

EOLIAN static void
_efl_access_object2_role_set(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd, Efl_Access_Role role)
{
   if (pd->role != role)
     pd->role = role;
}

EOLIAN const char *
_efl_access_object2_access_name_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
#ifdef ENABLE_NLS
   if (pd->translation_domain)
     return dgettext(pd->translation_domain, pd->name);
#endif
   return pd->name;
}

EOLIAN static void
_efl_access_object2_access_name_set(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd, const char *val)
{
   eina_stringshare_replace(&pd->name, val);
}

EOLIAN const char *
_efl_access_object2_access_description_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
#ifdef ENABLE_NLS
   if (pd->translation_domain)
      return dgettext(pd->translation_domain, pd->description);
#endif
   return pd->description;
}

EOLIAN static void
_efl_access_object2_access_description_set(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd, const char *val)
{
   eina_stringshare_replace(&pd->description, val);
}

EOLIAN static Efl_Access_State_Set
_efl_access_object2_state_set_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd EINA_UNUSED)
{
   return 0;
}

EOLIAN Eina_Iterator*
_efl_access_object2_relations_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
   return eina_inarray_iterator_new(&pd->relations);
}

EOLIAN void
_efl_access_object2_relation_add(Eo *obj, Efl_Access_Object_Data *pd, Efl_Access_Relation_Type type, Efl_Access_Object2 *target)
{
   Efl_Access_Relation2 *rel = (Efl_Access_Relation2*)efl_access_object2_relation_get(obj, type);
   if (!rel) {
        rel = _relation_new(type);
        if (!rel) return;
        eina_inarray_push(&pd->relations, rel);
   }

   _relation_target_add(rel, target);
}

EOLIAN void
_efl_access_object2_relation_del(Eo *obj, Efl_Access_Object_Data *pd EINA_UNUSED, Efl_Access_Relation_Type type, Efl_Access_Object2 *target)
{
   Efl_Access_Relation2 *rel = (Efl_Access_Relation2*)efl_access_object2_relation_get(obj, type);
   if (!rel) return;

   _relation_target_del(rel, target);
}

EOLIAN void
_efl_access_object2_relation_del_by_type(Eo *obj, Efl_Access_Object_Data *pd, Efl_Access_Relation_Type type)
{
   Efl_Access_Relation2 *rel = (Efl_Access_Relation2*)efl_access_object2_relation_get(obj, type);
   if (!rel) return;

   eina_inarray_remove(&pd->relations, rel);
   _relation_free(rel);
}

static int
_relation_search_by_key(const void *gdata, const void *data)
{
   const Efl_Access_Relation_Type *type = gdata;
   const Efl_Access_Relation2 *rel = data;

   return *type == rel->type ? 0 : 1;
}

EOLIAN const Efl_Access_Relation2*
_efl_access_object2_relation_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd, Efl_Access_Relation_Type key)
{
   int idx = eina_inarray_search(&pd->relations, &key, _relation_search_by_key);
   return idx == -1 ? NULL : eina_inarray_nth(&pd->relations, idx);
}

EOLIAN void
_efl_access_object2_relation_del_all(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
   Efl_Access_Relation2 *rel;

   EINA_INARRAY_FOREACH(&pd->relations, rel)
      _relation_free(rel);

   eina_inarray_flush(&pd->relations);
}

EOLIAN void
_efl_access_object2_translation_domain_set(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd, const char *domain)
{
   eina_stringshare_replace(&pd->translation_domain, domain);
}

EOLIAN const char*
_efl_access_object2_translation_domain_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
   return pd->translation_domain;
}

EOLIAN void
_efl_access_object2_locale_set(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd, const char *locale)
{
   eina_stringshare_replace(&pd->locale, locale);
}

EOLIAN const char*
_efl_access_object2_locale_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
   if (!pd->locale)
     return setlocale(LC_MESSAGES, NULL);
   return pd->locale;
}

EOLIAN Eo*
_efl_access_object2_efl_object_constructor(Eo *obj, Efl_Access_Object_Data *pd)
{
   pd->role = EFL_ACCESS_ROLE_INVALID;

   eina_inarray_step_set(&pd->relations, sizeof(Eina_Inarray), sizeof(Efl_Access_Relation2), 0);
   eina_inarray_step_set(&pd->attributes, sizeof(Eina_Inarray), sizeof(Efl_Access_Attribute), 0);

   return efl_constructor(efl_super(obj, EFL_ACCESS_OBJECT2_CLASS));
}

EOLIAN Efl_Access_Object2*
_efl_access_object2_access_parent_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd)
{
   return pd->parent;
}

EOLIAN void
_efl_access_object2_access_parent_set(Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd, Efl_Access_Object2 *parent)
{
   pd->parent = parent;
}

EOLIAN Eina_Array*
_efl_access_object2_access_children_get(const Eo *obj EINA_UNUSED, Efl_Access_Object_Data *pd EINA_UNUSED)
{
   return NULL;
}

EOLIAN void
_efl_access_object2_efl_object_destructor(Eo *obj, Efl_Access_Object_Data *pd)
{
   efl_access_object2_relation_del_all(obj);
   efl_access_object2_attribute_del_all(obj);

   eina_stringshare_del(pd->name);
   eina_stringshare_del(pd->description);
   eina_stringshare_del(pd->translation_domain);
   eina_stringshare_del(pd->locale);

   efl_destructor(efl_super(obj, EFL_ACCESS_OBJECT2_CLASS));
}

#include "efl_access_object2.eo.c"

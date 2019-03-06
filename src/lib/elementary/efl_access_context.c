#ifdef HAVE_CONFIG_H
  #include "elementary_config.h"
#endif

#define EFL_ACCESS_CONTEXT_BETA

#include <Elementary.h>
#include "elm_widget.h"
#include "elm_priv.h"

typedef struct _Efl_Access_Context_Data
{
   Eina_Array relations;
   Eina_Array attributes;
   const char *name;
   const char *description;
   const char *translation_domain;
   const char *locale;
   Efl_Access_Object2 *parent;
   Efl_Access_Role role;
   Efl_Access_Reading_Info_Type reading_info;
   Efl_Access_Object2 *access_object;
} Efl_Access_Context_Data;

static int _find_matching_pointer(const void *data, const void *gdata);
static Eina_Bool _remove_matching_pointer_only(void *data, void *gdata);

EAPI Efl_Access_Attribute*
efl_access_attribute_new(const char *key, const char *value)
{
   Efl_Access_Attribute *attr = calloc(1, sizeof(Efl_Access_Attribute));
   if (!attr) return NULL;
   attr->key = eina_stringshare_add(key);
   attr->value = eina_stringshare_add(value);
   return attr;
}

EAPI Efl_Access_Attribute*
efl_access_attribute_clone(const Efl_Access_Attribute *attribute)
{
   return efl_access_attribute_new(attribute->key, attribute->value);
}

//TODO uncomment when removeing efl_access_object.eo
#if 0
EAPI void
efl_access_attribute_free(Efl_Access_Attribute *attr)
{
   eina_stringshare_del(attr->key);
   eina_stringshare_del(attr->value);
   free(attr);
}
#endif

EAPI Efl_Access_Relation2*
efl_access_relation_new(Efl_Access_Relation_Type type)
{
   Efl_Access_Relation2 *rel = calloc(1, sizeof(Efl_Access_Relation2));
   if (!rel) return NULL;

   rel->type = type;
   rel->objects = eina_array_new(2);

   return rel;
}

EAPI void
efl_access_relation_target_add(Efl_Access_Relation2 *rel, const Efl_Access_Object2 *target)
{
   if (eina_array_search(rel->objects, target, _find_matching_pointer) != -1)
     return;
   eina_array_push(rel->objects, target);
}

EAPI void
efl_access_relation_target_del(Efl_Access_Relation2 *rel, const Efl_Access_Object2 *target)
{
   if (eina_array_search(rel->objects, target, _find_matching_pointer) == -1)
     return;

   eina_array_remove(rel->objects, _remove_matching_pointer_only, (void*)target);
}

EAPI Efl_Access_Relation2*
efl_access_relation_clone(Efl_Access_Relation2 *rel)
{
   Eina_Array_Iterator iter;
   Efl_Access_Object2 *target;
   unsigned int idx;

   Efl_Access_Relation2 *clone = efl_access_relation_new(rel->type);

   EINA_ARRAY_ITER_NEXT(rel->objects, idx, target, iter)
      efl_access_relation_target_add(clone, target);

   return clone;
}

EAPI void
efl_access_relation_free(Efl_Access_Relation2 *rel)
{
   eina_array_free(rel->objects);
   free(rel);
}

EOLIAN static int
_efl_access_context_index_in_parent_get(const Eo *obj, Efl_Access_Context_Data *pd EINA_UNUSED)
{
   Eina_Array *children = NULL;
   Efl_Access_Object2 *parent = NULL;
   int ret = 0;
   Efl_Access_Context *parent_ctx;

   parent = efl_access_context_access_parent_get(obj);
   if (!parent) return -1;

   parent_ctx = efl_access_object2_access_context_get(parent);
   if (!parent_ctx) return -1;

   children = efl_access_context_access_children_get(parent_ctx);
   if (!children) return -1;

   ret = eina_array_search(children, efl_access_context_access_object_get(obj), _find_matching_pointer);
   eina_array_free(children);
   return ret;
}

EOLIAN static Efl_Access_Object2*
_efl_access_context_access_parent_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   return pd->parent;
}

EOLIAN static void
_efl_access_context_access_parent_set(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd, Efl_Access_Object2 *parent)
{
   pd->parent = parent;
}

EOLIAN Eina_Array*
_efl_access_context_attributes_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   Efl_Access_Attribute *attribute;
   Eina_Array_Iterator iter;
   unsigned int idx;
   Eina_Array *attributes = eina_array_new(2);

   if (!attributes)
     return NULL;

   EINA_ARRAY_ITER_NEXT(&pd->attributes, idx, attribute, iter)
     {
        Efl_Access_Attribute *attr = efl_access_attribute_clone(attribute);
        eina_array_push(attributes, attr);
     }

   return attributes;
}

EOLIAN static void
_efl_access_context_attribute_append(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd, const char *key, const char *value)
{
   Efl_Access_Attribute *attr = NULL;

   if (!key || !value) return;

   attr = efl_access_attribute_new(key, value);

   eina_array_push(&pd->attributes, attr);
}

EOLIAN static void
_efl_access_context_attribute_remove(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd, const char *key)
{
}

EOLIAN static void
_efl_access_context_attributes_clear(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   Efl_Access_Attribute *attr;
   Eina_Array_Iterator iter;
   unsigned int idx;

   EINA_ARRAY_ITER_NEXT(&pd->attributes, idx, attr, iter)
      efl_access_attribute_free(attr);

   eina_array_free(&pd->attributes);
}

EOLIAN static void
_efl_access_context_reading_info_type_set(Eo *obj, Efl_Access_Context_Data *pd, Efl_Access_Reading_Info_Type reading_info)
{
   Eina_Strbuf *buf = NULL;
   pd->reading_info = reading_info;
   buf = eina_strbuf_new();
   eina_strbuf_reset(buf);
   if (reading_info & (EFL_ACCESS_READING_INFO_TYPE_NAME))
     {
        eina_strbuf_append(buf, "name");
        eina_strbuf_append_char(buf, '|');
     }
   if (reading_info & (EFL_ACCESS_READING_INFO_TYPE_ROLE))
     {
        eina_strbuf_append(buf, "role");
        eina_strbuf_append_char(buf, '|');
     }
   if (reading_info & (EFL_ACCESS_READING_INFO_TYPE_DESCRIPTION))
     {
        eina_strbuf_append(buf, "description");
        eina_strbuf_append_char(buf, '|');
     }
   if (reading_info & (EFL_ACCESS_READING_INFO_TYPE_STATE))
     {
        eina_strbuf_append(buf, "state");
     }
   efl_access_object_attribute_append(obj, "reading_info_type", eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);
}

EOLIAN Efl_Access_Reading_Info_Type
_efl_access_context_reading_info_type_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   return pd->reading_info;
}

EOLIAN static Efl_Access_Role
_efl_access_context_role_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   return pd->role;
}

EOLIAN static void
_efl_access_context_role_set(Eo *obj, Efl_Access_Context_Data *pd, Efl_Access_Role role)
{
   pd->role = role;
}

EOLIAN const char *
_efl_access_context_access_name_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
#ifdef ENABLE_NLS
   if (pd->translation_domain)
     return dgettext(pd->translation_domain, pd->name);
#endif
   return pd->name;
}

EOLIAN static void
_efl_access_context_access_name_set(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd, const char *val)
{
   eina_stringshare_replace(&pd->name, val);
}

EOLIAN const char *
_efl_access_context_access_description_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
#ifdef ENABLE_NLS
   if (pd->translation_domain)
      return dgettext(pd->translation_domain, pd->description);
#endif
   return pd->description;
}

EOLIAN static void
_efl_access_context_access_description_set(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd, const char *val)
{
   eina_stringshare_replace(&pd->description, val);
}

EOLIAN static Eina_Array*
_efl_access_context_access_children_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd EINA_UNUSED)
{
   return NULL;
}

EOLIAN static Efl_Access_State_Set
_efl_access_context_state_set_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd EINA_UNUSED)
{
   return 0;
}

EOLIAN Eina_Array*
_efl_access_context_relations_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   Eina_Array *ret = eina_array_new(2);

   return ret;
}

EOLIAN void
_efl_access_context_translation_domain_set(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd, const char *domain)
{
   eina_stringshare_replace(&pd->translation_domain, domain);
}

EOLIAN const char*
_efl_access_context_translation_domain_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   return pd->translation_domain;
}

EOLIAN void
_efl_access_context_locale_set(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd, const char *locale)
{
   eina_stringshare_replace(&pd->locale, locale);
}

EOLIAN const char*
_efl_access_context_locale_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   if (!pd->locale)
     return setlocale(LC_MESSAGES, NULL);
   return pd->locale;
}

#if 0
static void
_on_rel_obj_del(void *data, const Efl_Event *event)
{
   Efl_Access_Context_Data *sd = data;
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
}
#endif

static Eina_Bool
_remove_matching_pointer_only(void *data, void *gdata)
{
   return data != gdata;
}

static int
_find_matching_pointer(const void *data, const void *gdata)
{
   return data != gdata;
}

static int
_relation_find_by_type(const void *data, const void *gdata)
{
   const Efl_Access_Relation_Type *type = gdata;
   const Efl_Access_Relation2 *rel = data;
   return rel->type != *type;
}

EOLIAN static void
_efl_access_context_relationship_append(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *sd, Efl_Access_Relation_Type type, const Efl_Access_Object2 *target)
{
   Efl_Access_Relation2 *rel;
   int idx;

   idx = eina_array_search(&sd->relations, &type, _relation_find_by_type);
   if (idx == -1)
     rel = efl_access_relation_new(type);
   else
     rel = eina_array_data_get(&sd->relations, idx);

   efl_access_relation_target_add(rel, target);
   eina_array_push(&sd->relations, rel);
}

EOLIAN static void
_efl_access_context_relationship_remove(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd, Efl_Access_Relation_Type type, const Efl_Access_Object2 *relation_object)
{
   Efl_Access_Relation2 *rel;
   int idx;
   Eina_Bool remove_all = relation_object ? EINA_FALSE : EINA_TRUE;

   idx = eina_array_search(&pd->relations, &type, _relation_find_by_type);
   if (idx == -1) return;

   rel = eina_array_data_get(&pd->relations, idx);
   if (!rel) return;

   if (remove_all)
     {
        eina_array_remove(&pd->relations, _remove_matching_pointer_only, rel);
        efl_access_relation_free(rel);
     }
   else
     {
        efl_access_relation_target_del(rel, relation_object);
     }
}

EOLIAN void
_efl_access_context_relationships_clear(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   Efl_Access_Relation2 *rel;
   Eina_Array_Iterator iter;
   unsigned int idx;

   EINA_ARRAY_ITER_NEXT(&pd->relations, idx, rel, iter)
      efl_access_relation_free(rel);

   eina_array_free(&pd->relations);
}

EOLIAN Eo*
_efl_access_context_efl_object_constructor(Eo *obj, Efl_Access_Context_Data *pd)
{
   eina_array_step_set(&pd->relations, sizeof(Eina_Array), 2);
   eina_array_step_set(&pd->attributes, sizeof(Eina_Array), 2);

   return efl_constructor(efl_super(obj, EFL_ACCESS_CONTEXT_CLASS));
}

EOLIAN void
_efl_access_context_efl_object_destructor(Eo *obj, Efl_Access_Context_Data *pd)
{
   efl_access_object_attributes_clear(obj);
   efl_access_object_relationships_clear(obj);

   eina_stringshare_del(pd->name);
   eina_stringshare_del(pd->description);
   eina_stringshare_del(pd->translation_domain);
   eina_stringshare_del(pd->locale);

   efl_destructor(efl_super(obj, EFL_ACCESS_CONTEXT_CLASS));
}

EOLIAN Efl_Access_Object2*
_efl_access_context_access_object_get(const Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd)
{
   return pd->access_object;
}

EOLIAN void
_efl_access_context_access_object_set(Eo *obj EINA_UNUSED, Efl_Access_Context_Data *pd, Efl_Access_Object2 *ao)
{
   pd->access_object = ao;
}

#include "efl_access_context.eo.c"

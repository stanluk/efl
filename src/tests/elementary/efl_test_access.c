#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <stdbool.h>
#include <Elementary.h>

#include "efl_ui_suite.h"

static bool _attribute_exists(Efl_Access_Object2 *ao, const char *key, const char *value)
{
   bool ret = false;
   Efl_Access_Attribute *attr;
   Eina_Iterator *iter = efl_access_object2_attributes_get(ao);

   EINA_ITERATOR_FOREACH(iter, attr)
     {
        if (!strcmp(attr->key, key))
          {
             if (value)
               {
                  if (!strcmp(attr->value, value))
                    ret = true;
               }
             else
               ret = true;
             break;
          }
     }

   eina_iterator_free(iter);

   return ret;
}

EFL_START_TEST(efl_access_test_attributtes)
{
   Efl_Access_Object2 *ao = efl_add_ref(EFL_ACCESS_OBJECT2_CLASS, NULL);
   ck_assert(!_attribute_exists(ao, "type", "test"));

   efl_access_object2_attribute_add(ao, "type", "test");
   ck_assert(_attribute_exists(ao, "type", "test"));

   efl_access_object2_attribute_del(ao, "type");
   ck_assert(!_attribute_exists(ao, "type", "test"));

   efl_access_object2_attribute_add(ao, "type", "test");
   efl_access_object2_attribute_add(ao, "type", "test2");
   ck_assert(!_attribute_exists(ao, "type", "test"));
   ck_assert(_attribute_exists(ao, "type", "test2"));

   efl_unref(ao);
}
EFL_END_TEST

void
efl_access_test(TCase *tc)
{
   tcase_add_test(tc, efl_access_test_attributtes);
}

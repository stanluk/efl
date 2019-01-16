using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.ComponentModel;
using System.Threading;

namespace Efl {

public class Mono2Model<T> : /*Efl.Object, Efl.Model,*/ IDisposable
{
   private Efl.Model model;
    
   public Mono2Model (Efl.Model model)
   {
     //System.Diagnostics.Debug.Assert (inherited == false);
     this.model = model;
   }
    
   ~Mono2Model()
   {
      Dispose();
   }

   public void Dispose()
   {
       model = null;
   }

   public async System.Threading.Tasks.Task<T> GetItemAsync (uint index)
   {
       Eina.Value v = await model.GetChildrenSliceAsync (index, 1);

       return (T)Activator.CreateInstance(typeof(T), new System.Object[] {});
   }

   // public Eina.Iterator< System.String> Properties {
   //    get { return GetProperties(); }
   // }
   // public  uint ChildrenCount {
   //    get { return GetChildrenCount(); }
   // }
    
   // public Eina.Iterator<System.String> GetProperties()
   // {
   //     return model.GetProperties();
   // }
   // public Eina.Value GetProperty(  System.String property)
   // {
   //     return model.GetProperty(property);
   // }
   // public Eina.Future SetProperty(  System.String property,   Eina.Value value)
   // {
   //     return model.SetProperty(property, value);
   // }
   // public uint GetChildrenCount()
   // {
   //     return model.GetChildrenCount();
   // }
   // public Eina.Future GetPropertyReady(  System.String property)
   // {
   //     return model.GetPropertyReady(property);
   // }
   // public Eina.Future GetChildrenSlice(  uint start,   uint count)
   // {
   //     return model.GetChildrenSlice(start, count);
   // }
   // public Efl.Object AddChild()
   // {
   //     return model.AddChild();
   // }
   // public void DelChild( Efl.Object child)
   // {
   //     model.DelChild(child);
   // }
   // public System.Threading.Tasks.Task<Eina.Value> SetPropertyAsync(  System.String property,  Eina.Value value, System.Threading.CancellationToken token=default(System.Threading.CancellationToken))
   // {
   //     return model.SetPropertyAsync(property, value, token);
   // }
   // public System.Threading.Tasks.Task<Eina.Value> GetPropertyReadyAsync(  System.String property, System.Threading.CancellationToken token=default(System.Threading.CancellationToken))
   // {
   //     return model.GetPropertyReadyAsync(property, token);
   // }
   // public System.Threading.Tasks.Task<Eina.Value> GetChildrenSliceAsync(  uint start,  uint count, System.Threading.CancellationToken token=default(System.Threading.CancellationToken))
   // {
   //     return model.GetChildrenSliceAsync(start, count, token);
   // }
   // public event EventHandler<Efl.ModelPropertiesChangedEvt_Args> PropertiesChangedEvt
   // {
   //    add {
   //        model.PropertiesChangedEvt += value;
   //    }
   //    remove {
   //        model.PropertiesChangedEvt -= value;
   //    }
   // }
   // public event EventHandler<Efl.ModelChildAddedEvt_Args> ChildAddedEvt
   // {
   //    add {
   //        model.ChildAddedEvt += value;
   //    }
   //    remove {
   //        model.ChildAddedEvt -= value;
   //    }
   // }
   // public event EventHandler<Efl.ModelChildRemovedEvt_Args> ChildRemovedEvt
   // {
   //    add {
   //        model.ChildRemovedEvt += value;
   //    }
   //    remove {
   //        model.ChildRemovedEvt -= value;
   //    }
   // }
   // public event EventHandler ChildrenCountChangedEvt
   // {
   //    add {
   //        model.ChildrenCountChangedEvt += value;
   //    }
   //    remove {
   //        model.ChildrenCountChangedEvt -= value;
   //    }
   // }
}

}

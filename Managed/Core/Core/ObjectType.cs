//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated from a template.
//
//     Manual changes to this file may cause unexpected behavior in your application.
//     Manual changes to this file will be overwritten if the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace Core
{
    using System;
    using System.Collections.Generic;
    
    public partial class ObjectType
    {
        public ObjectType()
        {
            this.FileCatalogues = new HashSet<FileCatalogue>();
        }
    
        public int ObjectTypeId { get; set; }
        public string Description { get; set; }
    
        public virtual ICollection<FileCatalogue> FileCatalogues { get; set; }
    }
}
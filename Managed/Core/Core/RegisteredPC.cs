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
    
    public partial class RegisteredPC
    {
        public RegisteredPC()
        {
            this.FileCatalogues = new HashSet<FileCatalogue>();
        }
    
        public int PCId { get; set; }
        public string MacAddress { get; set; }
        public bool IsEnabled { get; set; }
        public string PCName { get; set; }
    
        public virtual ICollection<FileCatalogue> FileCatalogues { get; set; }
    }
}

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
    
    public partial class FileCatalogue
    {
        public FileCatalogue()
        {
            this.Metadatas = new HashSet<Metadata>();
        }
    
        public int RowId { get; set; }
        public int Fk_PCId { get; set; }
        public int Fk_ObjectType { get; set; }
        public string FileName { get; set; }
        public string FullPath { get; set; }
        public string Category { get; set; }
        public string SubCategory { get; set; }
    
        public virtual ObjectType ObjectType { get; set; }
        public virtual RegisteredPC RegisteredPC { get; set; }
        public virtual ICollection<Metadata> Metadatas { get; set; }
    }
}

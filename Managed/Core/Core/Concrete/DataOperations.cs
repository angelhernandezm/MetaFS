// Copyright (C) 2015 Angel Hernandez Matos / Bonafide Ideas.
// You can redistribute this software and/or modify it under the terms of the 
// Microsoft Reciprocal License (Ms-RL).  This program is distributed in the hope 
// that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
// See License.txt for more details. 

/* C# compiler    : Microsoft (R) Visual C# Compiler version 12.0.31101.0 for C# 5
Creation date     : 09/06/2015
Developer         : Angel Hernandez Matos
e-m@il            : angel@bonafideideas.com
Website           : http://www.bonafideideas.com

Description: Data Access Layer used by MetaFSProps control
*/

using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.Entity;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Transactions;
using System.Windows.Forms;
using System.Xml.Linq;
using Core.Abstractions;
using DAL;

namespace Core.Concrete {
	public class DataOperations : IDataOperations {
		/// <summary>
		/// Gets the selected mac.
		/// </summary>
		/// <value>
		/// The selected mac.
		/// </value>
		public string SelectedMac {
			get;
			private set;
		}

		/// <summary>
		/// Gets the pc identifier.
		/// </summary>
		/// <value>
		/// The pc identifier.
		/// </value>
		public int PcIdentifier {
			get;
			private set;
		}

		/// <summary>
		/// Gets the name of the machine.
		/// </summary>
		/// <value>
		/// The name of the machine.
		/// </value>
		public string MachineName {
			get;
			private set;
		}

		/// <summary>
		/// Gets the logger.
		/// </summary>
		/// <value>
		/// The logger.
		/// </value>
		public IGenericLogger Logger {
			get;
			private set;
		}

		/// <summary>
		/// Gets the network ops.
		/// </summary>
		/// <value>
		/// The network ops.
		/// </value>
		public INetworkOperations NetworkOps {
			get;
			private set;
		}

		/// <summary>
		/// Gets the renderer.
		/// </summary>
		/// <value>
		/// The renderer.
		/// </value>
		public IRenderer Renderer {
			get;
			private set;
		}


		/// <summary>
		/// Initializes a new instance of the <see cref="DataOperations" /> class.
		/// </summary>
		/// <param name="logger">The logger.</param>
		/// <param name="networkOps">The network ops.</param>
		/// <param name="view">The view.</param>
		protected DataOperations(IGenericLogger logger, INetworkOperations networkOps, UserControl view = null) {
			Logger = logger;
			NetworkOps = networkOps;
			Renderer = new Renderer() {
				View = view
			};
			InitializeDb();
		}

		/// <summary>
		/// Gets the instance.
		/// </summary>
		/// <returns></returns>
		public static IDataOperations GetInstance(UserControl view = null) {
			return new DataOperations(GenericLogger.Instance, NetworkOperations.Instance, view);
		}

		/// <summary>
		/// Preloads the ef.
		/// </summary>
		/// <param name="args">The arguments.</param>
		/// <returns>System.Int32.</returns>
		private static int PreloadEF(string args) {
			using (var context = new MetaFSDB()) 
				context.Categories.Any();

			return 0;
		}

		/// <summary>
		/// Creates the record.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="t">The t.</param>
		/// <returns></returns>
		public bool CreateRecord<T>(T t) {
			var retval = true;

			try {
				using (var context = new MetaFSDB()) {
					if (t is RegisteredPC)
						context.RegisteredPCs.Add(t as RegisteredPC);
					else if (t is FileCatalogue)
						context.FileCatalogues.Add(t as FileCatalogue);
					else if (t is Metadata)
						context.Metadatas.Add(t as Metadata);

					context.SaveChanges();
				}
			} catch (Exception ex) {
				retval = false;
				Logger.LogError(ex);
			}

			return retval;
		}

		/// <summary>
		/// Updates the file metadata.
		/// </summary>
		/// <param name="currentMetadata">The current metadata.</param>
		public void UpdateFileMetadata(IFileMetadata currentMetadata) {
			var metadata = GetMetadataAsXml(Renderer.RehydrateFromUi(), currentMetadata);

			try {
				using (var ts = new TransactionScope(TransactionScopeOption.RequiresNew,
					new TransactionOptions() {
						IsolationLevel = IsolationLevel.ReadUncommitted
					})) {

					using (var context = new MetaFSDB()) {
						if (currentMetadata.MetadataId != 0) {
							UpdateExistingRecord(currentMetadata, context, metadata);
						} else {
							var newFileEntry = context.FileCatalogues.Add(new FileCatalogue() {
								Fk_PCId = PcIdentifier,
								FileName = currentMetadata.FileName,
								FullPath = currentMetadata.FullPath,
								Fk_ObjectType = 1
							});

							context.SaveChanges();

							context.Metadatas.Add(new Metadata() {
								Fk_RowId = newFileEntry.RowId,
								Fk_TemplateId = currentMetadata.TemplateId,
								Metadata1 = metadata.ToString()
							});
							context.SaveChanges();
						}
					}
					ts.Complete();
				}
			} catch (Exception ex) {
				Logger.LogError(ex);
			}
		}

		/// <summary>
		/// Updates the existing record.
		/// </summary>
		/// <param name="currentMetadata">The current metadata.</param>
		/// <param name="context">The context.</param>
		/// <param name="metadata">The metadata.</param>
		private void UpdateExistingRecord(IFileMetadata currentMetadata, MetaFSDB context, XDocument metadata) {
			var existing = context.Metadatas.FirstOrDefault(x => x.MetaId == currentMetadata.MetadataId);
			existing.Metadata1 = metadata.ToString();
			context.Metadatas.Attach(existing);
			var entry = context.Entry(existing);
			entry.Property(e => e.Metadata1).IsModified = true;
			context.SaveChanges();
		}

		/// <summary>
		/// Gets the metadata as XML.
		/// </summary>
		/// <param name="elements">The elements.</param>
		/// <param name="currentMetadata">The current metadata.</param>
		/// <returns></returns>
		private XDocument GetMetadataAsXml(IEnumerable<IUiElement> elements, IFileMetadata currentMetadata) {
			XDocument retval = null;
			List<IUiElement> uiElements;

			if (elements != null && (uiElements = elements.ToList()).Count > 0) {
				var xml = new StringBuilder();
				xml.Append("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
				xml.Append(string.Format("<MetaFS-Data version=\"1.0\" templateToUse=\"{0}\">", currentMetadata.TemplateId));
				xml.Append("<Data>");
				uiElements.ForEach(x => xml.Append(string.Format("<UIElement name=\"{0}\" value=\"{1}\" />", new object[] { x.Name, x.Value })));
				xml.Append("</Data>");
				xml.Append("</MetaFS-Data>");

				using (var sr = new StringReader(xml.ToString()))
					retval = XDocument.Load(sr);
			}

			return retval;
		}


		/// <summary>
		/// Gets the metadata.
		/// </summary>
		/// <param name="context">The context.</param>
		/// <param name="macAddress">The mac address.</param>
		/// <param name="computerName">Name of the computer.</param>
		/// <param name="fileName">Name of the file.</param>
		/// <returns>System.Object.</returns>
		private object GetMetadata(MetaFSDB context, string macAddress, string computerName, string fileName) {
			return (from pcs in context.RegisteredPCs
					join fileCat in context.FileCatalogues
						on pcs.PCId equals fileCat.Fk_PCId
					join meta in context.Metadatas
						on fileCat.RowId equals meta.Fk_RowId
					join template in context.TemplateCatalogues
						on meta.Fk_TemplateId equals template.TemplateId
					where pcs.MacAddress == macAddress && fileCat.FullPath == fileName
					select new {
						PcId = pcs.PCId,
						RowId = meta.MetaId,
						Metadata = meta.Metadata1,
						FileName = fileCat.FileName,
						FilePath = fileCat.FullPath,
						Category = fileCat.Category,
						TemplateId = template.TemplateId,
						Template = template.TemplateBody,
						SubCategory = fileCat.SubCategory
					}).FirstOrDefault();
		}

		/// <summary>
		/// Gets the file metadata.
		/// </summary>
		/// <param name="macAddress">The mac address.</param>
		/// <param name="computerName">Name of the computer.</param>
		/// <param name="fileName">Name of the file.</param>
		/// <returns></returns>
		public IFileMetadata GetFileMetadata(string macAddress, string computerName, string fileName) {
			dynamic metadata;
			IFileMetadata retval = null;

			try {
				using (var context = new MetaFSDB()) {
					if ((metadata = GetMetadata(context, macAddress, computerName, fileName)) != null) {
						// Metadata found
						retval = new FileMetadata() {
							MetadataId = metadata.RowId,
							Category = metadata.Category,
							FileName = metadata.FileName,
							FullPath = metadata.FilePath,
							TemplateId = metadata.TemplateId,
							PcId = metadata.PcId,
							Metadata = ((string)metadata.Metadata).LoadAsXDocument(),
							SubCategory = metadata.SubCategory,
							TemplateBody = ((string)metadata.Template).LoadAsXDocument()
						};
					} else { // Metadata needs to be created
						retval = CreateMetadata(fileName);
					}
				}
			} catch (Exception ex) {
				Logger.LogError(ex);
			}

			return retval;
		}

		/// <summary>
		/// Creates the metadata.
		/// </summary>
		/// <param name="fileName">Name of the file.</param>
		/// <returns></returns>
		private IFileMetadata CreateMetadata(string fileName) {
			IFileMetadata retval = null;

			try {
				using (var context = new MetaFSDB()) {
					var metadata = context.TemplateCatalogues.FirstOrDefault(x => x.IsActive.Value);

					if (metadata != null) {
						retval = new FileMetadata() {
							PcId = PcIdentifier,
							FullPath = fileName,
							TemplateId = metadata.TemplateId,
							FileName = Path.GetFileName(fileName),
							TemplateBody = metadata.TemplateBody.LoadAsXDocument()
						};
					}
				}
			} catch (Exception ex) {
				Logger.LogError(ex);
			}

			return retval;
		}

		/// <summary>
		/// Gets the basic categories.
		/// </summary>
		/// <returns></returns>
		private IEnumerable<Category> GetBasicCategories() {
			return new List<Category>()
						{
							new Category() {CategoryName = "Family"},
							new Category() {CategoryName = "Work Related"},
							new Category() {CategoryName = "School"},
							new Category() {CategoryName = "Financial"},
							new Category() {CategoryName = "Family"}
						};
		}

		/// <summary>
		/// Gets the basic object types.
		/// </summary>
		/// <returns></returns>
		private IEnumerable<ObjectType> GetBasicObjectTypes() {
			return new List<ObjectType>()
						{
							new ObjectType() {Description = "File"},
							new ObjectType() {Description = "Directory"},
							new ObjectType() {Description = "Symbolic Link"}
						};
		}

		/// <summary>
		/// Initializes the database.
		/// </summary>
		private void InitializeDb() {
			try {
				using (var ts = new TransactionScope(TransactionScopeOption.RequiresNew,
					new TransactionOptions() {
						IsolationLevel = IsolationLevel.ReadUncommitted
					})) {

					using (var context = new MetaFSDB()) {
						// Create basic categories
						if (!context.Categories.Any())
							context.Categories.AddRange(GetBasicCategories());

						// Create basic object types
						if (!context.ObjectTypes.Any())
							context.ObjectTypes.AddRange(GetBasicObjectTypes());

						// Register PC
						SelectedMac = NetworkOps.GetMacAddress();

						var registeredPc = context.RegisteredPCs.FirstOrDefault(x => x.PCName.Equals(Environment.MachineName,
																									 StringComparison.OrdinalIgnoreCase));
						if (registeredPc == null) {
							registeredPc = new RegisteredPC() {
								MacAddress = SelectedMac,
								IsEnabled = true,
								PCName = Environment.MachineName
							};
							context.RegisteredPCs.Add(registeredPc);
						} else {
							registeredPc.MacAddress = SelectedMac;
							context.RegisteredPCs.Attach(registeredPc);
							var entry = context.Entry(registeredPc);
							entry.Property(e => e.MacAddress).IsModified = true;
						}

						context.SaveChanges();

						PcIdentifier = registeredPc.PCId;
						MachineName = registeredPc.PCName;
					}
					ts.Complete();
				}
			} catch (Exception ex) {
				Logger.LogError(ex);
			}
		}

	}
}

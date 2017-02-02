CREATE TABLE [dbo].[TemplateCatalogue](
	[TemplateId] [int] IDENTITY(1,1) NOT NULL,
	[VersionNumber] [int] NOT NULL,
	[IsActive] [bit]  DEFAULT (1) NULL,
	[Comments] [varchar](256) DEFAULT ('') NULL,
	[TemplateBody] [xml] NOT NULL,
 CONSTRAINT [PK_TemplateCatalogue] PRIMARY KEY CLUSTERED (	[TemplateId] ASC));
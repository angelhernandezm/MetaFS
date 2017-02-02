CREATE TABLE [dbo].[Metadata] (
    [MetaId]   INT IDENTITY (1, 1) NOT NULL,
    [Fk_RowId] INT NOT NULL,
	[Fk_TemplateId] [int] NOT NULL,
    [Metadata] XML CONSTRAINT [DF_Metadata_Metadata] DEFAULT ('') NULL,
    CONSTRAINT [PK_Metadata] PRIMARY KEY CLUSTERED ([MetaId] ASC),
    CONSTRAINT [FK_Metadata_FileCatalogue] FOREIGN KEY ([Fk_RowId]) REFERENCES [dbo].[FileCatalogue] ([RowId]),
	CONSTRAINT [FK_Metadata_TemplateCatalogue] FOREIGN KEY ([Fk_TemplateId]) REFERENCES [dbo].[TemplateCatalogue] ([TemplateId])
);
CREATE TABLE [dbo].[FileCatalogue] (
    [RowId]         INT           IDENTITY (1, 1) NOT NULL,
    [Fk_PCId]       INT           NOT NULL,
    [Fk_ObjectType] INT           NOT NULL,
    [FileName]      VARCHAR (100) NOT NULL,
    [FullPath]      VARCHAR (256) NOT NULL,
    [Category]      VARCHAR (50)  CONSTRAINT [DF_FileCatalogue_Category] DEFAULT ('') NULL,
    [SubCategory]   VARCHAR (50)  CONSTRAINT [DF_FileCatalogue_SubCategory] DEFAULT ('') NULL,
    CONSTRAINT [PK_FileCatalogue] PRIMARY KEY CLUSTERED ([RowId] ASC),
    CONSTRAINT [FK_FileCatalogue_ObjectType] FOREIGN KEY ([Fk_ObjectType]) REFERENCES [dbo].[ObjectType] ([ObjectTypeId]),
    CONSTRAINT [FK_FileCatalogue_RegisteredPC] FOREIGN KEY ([Fk_PCId]) REFERENCES [dbo].[RegisteredPC] ([PCId])
);


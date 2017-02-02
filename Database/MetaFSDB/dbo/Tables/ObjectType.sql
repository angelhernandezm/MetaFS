CREATE TABLE [dbo].[ObjectType] (
    [ObjectTypeId] INT          IDENTITY (1, 1) NOT NULL,
    [Description]  VARCHAR (50) CONSTRAINT [DF_ObjectType_Description] DEFAULT ('') NOT NULL,
    CONSTRAINT [PK_ObjectType] PRIMARY KEY CLUSTERED ([ObjectTypeId] ASC)
);


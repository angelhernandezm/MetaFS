CREATE TABLE [dbo].[Category] (
    [CategoryId]   INT          IDENTITY (1, 1) NOT NULL,
    [CategoryName] VARCHAR (50) CONSTRAINT [DF_Category_CategoryName] DEFAULT ('') NULL,
    CONSTRAINT [PK_Category] PRIMARY KEY CLUSTERED ([CategoryId] ASC)
);


CREATE TABLE [dbo].[RegisteredPC] (
    [PCId]       INT          IDENTITY (1, 1) NOT NULL,
    [MacAddress] VARCHAR (36) CONSTRAINT [DF_PCCatalog_MacAddress] DEFAULT ('') NOT NULL,
	[PCName] VARCHAR (24) CONSTRAINT [DF_PCCatalog_PCName] DEFAULT ('') NOT NULL,
    [IsEnabled]  BIT          CONSTRAINT [DF_PCCatalog_IsEnabled] DEFAULT ('') NOT NULL,
    CONSTRAINT [PK_PCCatalog] PRIMARY KEY CLUSTERED ([PCId] ASC)
);


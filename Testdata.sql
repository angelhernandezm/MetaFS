Use MetaFSDB
Go

-- Let's insert a sample file
 insert into FileCatalogue (Fk_PCId, Fk_ObjectType, FileName, FullPath, Category, SubCategory)
  Values (1, 1, 'test.txt', 'c:\temp\test.txt', '1', '4')


-- Let's insert a sample metadata template
Insert into TemplateCatalogue (VersionNumber, IsActive, Comments, TemplateBody)
  Values (1, 1, 'Sample Template',
    CONVERT(XML, N'<MetaFS version="1.0" templateid="1">
	<UIElement class="Label" rect="30,100,-1,10" name="lblComments" text="Comments" taborder="0" />
	<UIElement class="Textbox" rect="85,100,230,20" readonly="false" name="txtComments" maxlength="256" taborder="1" />
	<UIElement class="Label" rect="30,150,-1,10" name="lblAdditionalInfo" text="Additional Information" taborder="3" />
	<UIElement class="Textbox" rect="140,150,180,10" width="200" readonly="false" name="txtAdditionalInfo" maxlength="256" taborder="4"/></MetaFS>')) 


Insert into Metadata (Fk_RowId, Fk_TemplateId, Metadata)
  Values (1, 1, CONVERT(XML, N'<?xml version="1.0" encoding="utf-16"?>
                                   <MetaFS-Data version="1.0" templateToUse="1">
	                                  <Data>
		                                <UIElement name="txtComments" value="This is a comment coming from the database" />
		                                <UIElement name="txtAdditionalInfo" value="Additional Information" />
	                                 </Data>
                            </MetaFS-Data>'))

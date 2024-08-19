/*
File:   main_markdown.cpp
Author: Taylor Robbins
Date:   08\17\2024
Description: 
	** Holds the functions that help us interact with MdFile_t, MdPiece_t, etc.
	**
	** This layer is a very crude version of parsing Markdown and only supports a
	** subset of the syntax (headers and bullet points) and also adds a few pieces of
	** syntax on top that aren't part of traditional Markdown (key:value, and //comments)
	** All other syntax is treated as "Content" pieces, to be displayed as regular text.
	**
	** All parsing done on top of this layer will have no correlation to Markdown
	** they will simply use to markdown pieces as input and enforce our own rules to
	** convert the data into a proper structure that can be displayed in the tree.
	**
	** When the user edits a bit of text in the UI, we will use the FileStr_t structure
	** to "inject" those changes into the file without modifying the rest of the file.
	** This allows us to maintain any whitespace, comments, etc. that the user may
	** have added when editing the file by hand in a text editor.
*/

bool IsInitialized(const MdFile_t* file)
{
	NotNull(file);
	return (file->allocArena != nullptr);
}

FileStr_t NewFileStr(MdFile_t* file, MyStr_t str)
{
	NotNull2(file, file->allocArena);
	NotNullStr(&file->contents);
	NotNullStr(&str);
	Assert(IsSizedPntrWithin(file->contents.chars, file->contents.length, str.chars, str.length));
	u64 startIndex = (u64)(str.chars - file->contents.chars);
	Assert(startIndex + str.length <= INT32_MAX);
	FileStr_t result;
	result.range = NewRangei((i32)startIndex, (i32)(startIndex + str.length));
	result.str = str;
	return result;
}

FileStr_t* GetFileStrFromMdPiece(MdPiece_t* piece, u64 index)
{
	switch (index)
	{
		case 0: return &piece->prefix;
		case 1: return &piece->editable;
		case 2: return &piece->suffix;
		case 3: return &piece->keyStr;
		default: return nullptr;
	}
}

void FreeMdFile(MdFile_t* file)
{
	NotNull(file);
	if (file->allocArena != nullptr)
	{
		FreeString(file->allocArena, &file->path);
		FreeString(file->allocArena, &file->contents);
		FreeLinkedList(&file->pieces);
	}
	ClearPointer(file);
}

void InitMdFile(MdFile_t* file, MemArena_t* memArena)
{
	NotNull2(file, memArena);
	ClearPointer(file);
	file->allocArena = memArena;
	CreateLinkedList(&file->pieces, memArena, MdPiece_t);
}

void MdFileSetPath(MdFile_t* file, MyStr_t path)
{
	NotNull2(file, file->allocArena);
	NotNullStr(&path);
	FreeString(file->allocArena, &file->path);
	file->path = AllocString(file->allocArena, &path);
}
void MdFileSetContents(MdFile_t* file, MyStr_t contents)
{
	NotNull2(file, file->allocArena);
	NotNullStr(&contents);
	FreeString(file->allocArena, &file->contents);
	file->contents = AllocString(file->allocArena, &contents);
}

bool TryParseMdFile(MyStr_t fileContents, ProcessLog_t* log, MdFile_t* fileOut)
{
	NotNull2(log, fileOut);
	NotNullStr(&fileContents);
	NotNull(fileOut->allocArena); //Please call InitMdFile before calling this function (also probably call MdFileSetPath)
	
	SetProcessLogName(log, NewStr("TryParseMdFile"));
	
	MdFileSetContents(fileOut, fileContents);
	
	u64 lineStart = 0;
	for (u64 cIndex = 0; cIndex < fileOut->contents.length; cIndex++)
	{
		char c = fileOut->contents.chars[cIndex];
		char nextChar = (cIndex+1 < fileOut->contents.length) ? fileOut->contents.chars[cIndex+1] : '\0';
		if (c == '\r' || c == '\n')
		{
			Assert(cIndex >= lineStart);
			MyStr_t line = NewStr(cIndex - lineStart, &fileOut->contents.chars[lineStart]);
			MyStr_t commentStr = MyStr_Empty;
			u64 commentStartIndex = 0;
			if (FindSubstring(line, "//", &commentStartIndex))
			{
				//Comments must have a leading space or tab char (or be at the beginning of the line)
				//This is to avoid false matches in links like https://www.google.com/
				if (commentStartIndex == 0 || line.chars[commentStartIndex-1] == ' ' || line.chars[commentStartIndex-1] == '\t')
				{
					commentStr = StrSubstring(&line, commentStartIndex);
					line = StrSubstring(&line, 0, commentStartIndex);
				}
			}
			MyStr_t trimmedLine = line;
			u64 numLeadingWhitespace = TrimLeadingWhitespace(&trimmedLine);
			u64 numTrailingWhitespace = TrimTrailingWhitespace(&trimmedLine);
			
			u64 colonIndex = 0;
			bool hasColon = FindSubstring(trimmedLine, ":", &colonIndex);
			MyStr_t validKeyChars = NewStr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_");
			
			// +==============================+
			// |  Generate MdPieceType_Empty  |
			// +==============================+
			if (IsEmptyStr(trimmedLine))
			{
				MdPiece_t* emptyPiece = LinkedListAdd(&fileOut->pieces, MdPiece_t);
				NotNull(emptyPiece);
				ClearPointer(emptyPiece);
				emptyPiece->type = MdPieceType_EmptyLine;
				emptyPiece->prefix = NewFileStr(fileOut, StrSubstring(&line, 0, numLeadingWhitespace));
				emptyPiece->editable = NewFileStr(fileOut, trimmedLine);
				emptyPiece->suffix = NewFileStr(fileOut, NewStr(numTrailingWhitespace, trimmedLine.chars + trimmedLine.length));
			}
			// +==============================+
			// | Generate MdPieceType_Header  |
			// +==============================+
			else if (StrStartsWith(trimmedLine, "### ") || StrStartsWith(trimmedLine, "## ") || StrStartsWith(trimmedLine, "# "))
			{
				u64 prefixLength = 0;
				if (StrStartsWith(trimmedLine, "### ")) { prefixLength = 4; }
				else if (StrStartsWith(trimmedLine, "## ")) { prefixLength = 3; }
				else if (StrStartsWith(trimmedLine, "# ")) { prefixLength = 2; }
				else { Assert(false); }
				
				MdPiece_t* headerPiece = LinkedListAdd(&fileOut->pieces, MdPiece_t);
				NotNull(headerPiece);
				ClearPointer(headerPiece);
				headerPiece->type = MdPieceType_Header;
				headerPiece->headerLevel = prefixLength-1;
				headerPiece->prefix = NewFileStr(fileOut, StrSubstring(&line, 0, numLeadingWhitespace + prefixLength));
				headerPiece->editable = NewFileStr(fileOut, StrSubstring(&trimmedLine, prefixLength));
				headerPiece->suffix = NewFileStr(fileOut, NewStr(numTrailingWhitespace, trimmedLine.chars + trimmedLine.length));
			}
			// +===============================+
			// | Generate MdPieceType_KeyValue |
			// +===============================+
			else if (hasColon && colonIndex > 0 &&
				IsStringMadeOfChars(StrSubstring(&trimmedLine, 0, colonIndex), validKeyChars) &&
				!IsCharNumeric(trimmedLine.chars[0]))
			{
				MyStr_t valueStr = StrSubstring(&trimmedLine, colonIndex+1);
				u64 numMiddleWhitespace = TrimLeadingWhitespace(&valueStr);
				MdPiece_t* keyValuePiece = LinkedListAdd(&fileOut->pieces, MdPiece_t);
				NotNull(keyValuePiece);
				ClearPointer(keyValuePiece);
				keyValuePiece->type = MdPieceType_KeyValue;
				keyValuePiece->keyStr = NewFileStr(fileOut, StrSubstring(&trimmedLine, 0, colonIndex));
				keyValuePiece->prefix = NewFileStr(fileOut, StrSubstring(&line, 0, numLeadingWhitespace + colonIndex+1 + numMiddleWhitespace));
				keyValuePiece->editable = NewFileStr(fileOut, valueStr);
				keyValuePiece->suffix = NewFileStr(fileOut, NewStr(numTrailingWhitespace, trimmedLine.chars + trimmedLine.length));
			}
			// +==================================+
			// | Generate MdPieceType_BulletPoint |
			// +==================================+
			else if (StrStartsWith(trimmedLine, "* ") || StrStartsWith(trimmedLine, "- "))
			{
				MdPiece_t* bulletPointPiece = LinkedListAdd(&fileOut->pieces, MdPiece_t);
				NotNull(bulletPointPiece);
				ClearPointer(bulletPointPiece);
				bulletPointPiece->type = MdPieceType_BulletPoint;
				bulletPointPiece->prefix = NewFileStr(fileOut, StrSubstring(&line, 0, numLeadingWhitespace + 2));
				bulletPointPiece->editable = NewFileStr(fileOut, StrSubstring(&trimmedLine, 2));
				bulletPointPiece->suffix = NewFileStr(fileOut, NewStr(numTrailingWhitespace, trimmedLine.chars + trimmedLine.length));
			}
			// +==============================+
			// | Generate MdPieceType_Content |
			// +==============================+
			else
			{
				MdPiece_t* contentPiece = LinkedListAdd(&fileOut->pieces, MdPiece_t);
				NotNull(contentPiece);
				ClearPointer(contentPiece);
				contentPiece->type = MdPieceType_Content;
				contentPiece->prefix = NewFileStr(fileOut, StrSubstring(&line, 0, numLeadingWhitespace));
				contentPiece->editable = NewFileStr(fileOut, trimmedLine);
				contentPiece->suffix = NewFileStr(fileOut, NewStr(numTrailingWhitespace, trimmedLine.chars + trimmedLine.length));
			}
			
			// +==============================+
			// | Generate MdPieceType_Comment |
			// +==============================+
			if (!IsEmptyStr(commentStr))
			{
				MdPiece_t* commentPiece = LinkedListAdd(&fileOut->pieces, MdPiece_t);
				NotNull(commentPiece);
				ClearPointer(commentPiece);
				commentPiece->type = MdPieceType_Comment;
				commentPiece->prefix = NewFileStr(fileOut, StrSubstring(&commentStr, 0, 2));
				commentPiece->editable = NewFileStr(fileOut, StrSubstring(&commentStr, 2));
				commentPiece->prefix = NewFileStr(fileOut, StrSubstring(&commentStr, commentStr.length));
			}
			
			if (c == '\r' && nextChar == '\n') { cIndex++; } //handle \r\n style new-lines
			lineStart = cIndex+1;
		}
	}
	
	LogExitSuccess(log);
	return true;
}

//TODO: Create a MdParseContent function that finds inline syntax for links, bold, images, etc.

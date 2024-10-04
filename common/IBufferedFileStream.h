#pragma once

#include "common/IDataStream.h"
#include "common/IFileStream.h"

/**
 *	An input/output file stream with buffering, write file on close
 */
class IBufferedFileStream : public IFileStream
{
public:
	IBufferedFileStream();
	virtual ~IBufferedFileStream();

	virtual bool	Open(const char* name);
	virtual bool	Create(const char* name);

	virtual void	Close(void);

	virtual void	ReadBuf(void* buf, UInt32 inLength);
	virtual void	WriteBuf(const void* buf, UInt32 inLength);
	virtual void	SetOffset(SInt64 inOffset);

	virtual void	SetLength(UInt64 length);

private:
	void	ReallocateStreamBuffer(UInt64 length);
	UInt64	CalculateStreamBufferLength(UInt64 length);

	UInt8* streamBuffer = nullptr;
	UInt64	streamBufferLength = 0;
	bool	streamBufferHasWrite = false;
};

#pragma once

#include "common/IDataStream.h"
#include "common/IFileStream.h"
#include <vector>

/**
 *	An input/output file stream with buffering, write file on close
 */
class IBufferedFileStream : public IDataStream
{
public:
	IBufferedFileStream();
	virtual ~IBufferedFileStream();

	bool	Open(const char* name);
	bool	Create(const char* name);

	void	Close(void);

	virtual void	ReadBuf(void* buf, UInt32 inLength);
	virtual void	WriteBuf(const void* buf, UInt32 inLength);

	void	SetLength(UInt64 length);

private:
	std::vector<UInt8>	streamBuffer;
	IFileStream			streamFile;

	bool	streamBufferHasWrite = false;
};

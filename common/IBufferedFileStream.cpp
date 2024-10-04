#include "IBufferedFileStream.h"

IBufferedFileStream::IBufferedFileStream() { }

IBufferedFileStream::~IBufferedFileStream()
{
	Close();

	free(streamBuffer);
}

bool IBufferedFileStream::Open(const char* name)
{
	bool success = IFileStream::Open(name);

	if (success)
	{
		ReallocateStreamBuffer(streamLength);

		IFileStream::ReadBuf(streamBuffer, streamLength);

		IFileStream::SetOffset(0);
	}

	return success;
}

bool IBufferedFileStream::Create(const char* name)
{
	bool success = IFileStream::Create(name);

	if (success)
	{
		ReallocateStreamBuffer(streamLength);
	}

	return success;
}

void IBufferedFileStream::Close(void)
{
	if (streamBufferHasWrite)
	{
		IFileStream::SetOffset(0);

		IFileStream::WriteBuf(streamBuffer, streamLength);
	}

	IFileStream::Close();
}

void IBufferedFileStream::ReadBuf(void* buf, UInt32 inLength)
{
	ASSERT_STR(inLength <= GetRemain(), "IBufferedFileStream::ReadBuf: hit eof");
	ASSERT_STR(streamBufferLength >= streamOffset + inLength, "IBufferedFileStream::ReadBuf: hit buffer eof");

	memcpy(buf, &streamBuffer[streamOffset], inLength);
	streamOffset += inLength;
}

void IBufferedFileStream::WriteBuf(const void* buf, UInt32 inLength)
{
	streamBufferHasWrite = true;

	if (streamBufferLength < streamOffset + inLength)
		ReallocateStreamBuffer(streamOffset + inLength);

	memcpy(&streamBuffer[streamOffset], buf, inLength);
	streamOffset += inLength;

	if (streamLength < streamOffset)
		streamLength = streamOffset;
}

void IBufferedFileStream::SetOffset(SInt64 inOffset)
{
	IDataStream::SetOffset(inOffset);
}

void IBufferedFileStream::SetLength(UInt64 length)
{
	ReallocateStreamBuffer(length);

	IFileStream::SetLength(length);
}

void IBufferedFileStream::ReallocateStreamBuffer(UInt64 length)
{
	UInt64 newStreamBufferLength = CalculateStreamBufferLength(length);

	if (newStreamBufferLength != streamBufferLength)
	{
		streamBufferLength = newStreamBufferLength;

		if (streamBuffer == nullptr)
		{
			streamBuffer = (UInt8*)malloc(streamBufferLength);
		}
		else
		{
			UInt8* newStreamBuffer = (UInt8*)realloc(streamBuffer, streamBufferLength);

			ASSERT_STR(newStreamBuffer, "IBufferedFileStream::ReallocateBuffer: realloc failed");

			streamBuffer = newStreamBuffer;
		}
	}
}

UInt64 IBufferedFileStream::CalculateStreamBufferLength(UInt64 length)
{
	const UInt64 step = 1024ull * 1024ull;

	if (!length) return step;

	UInt64 whole = length / step;
	UInt64 remain = length % step;

	return (whole + (remain > 0 ? 1 : 0)) * step;
}

#include "IBufferedFileStream.h"

IBufferedFileStream::IBufferedFileStream() { }

IBufferedFileStream::~IBufferedFileStream()
{
	Close();
}

bool IBufferedFileStream::Open(const char* name)
{
	bool success = streamFile.Open(name);

	if (success)
	{
		SetLength(streamFile.GetLength());
		SetOffset(0);

		streamFile.ReadBuf(streamBuffer.data(), streamLength);

		streamFile.SetOffset(0);
	}

	return success;
}

bool IBufferedFileStream::Create(const char* name)
{
	bool success = streamFile.Create(name);

	if (success)
	{
		SetLength(0);
		SetOffset(0);
	}

	return success;
}

void IBufferedFileStream::Close(void)
{
	if (streamBufferHasWrite)
	{
		streamFile.SetLength(streamLength);

		streamFile.SetOffset(0);

		streamFile.WriteBuf(streamBuffer.data(), streamLength);

		streamBufferHasWrite = false;
	}

	SetLength(0);
	SetOffset(0);

	streamFile.Close();
}

void IBufferedFileStream::ReadBuf(void* buf, UInt32 inLength)
{
	ASSERT_STR(inLength <= GetRemain(), "IBufferedFileStream::ReadBuf: hit eof");
	ASSERT_STR(streamBuffer.size() >= streamOffset + inLength, "IBufferedFileStream::ReadBuf: hit buffer eof");

	memcpy(buf, &streamBuffer[streamOffset], inLength);
	streamOffset += inLength;
}

void IBufferedFileStream::WriteBuf(const void* buf, UInt32 inLength)
{
	streamBufferHasWrite = true;

	if (streamBuffer.size() < streamOffset + inLength)
		SetLength(streamOffset + inLength);

	memcpy(&streamBuffer[streamOffset], buf, inLength);
	streamOffset += inLength;
}

void IBufferedFileStream::SetLength(UInt64 length)
{
	streamBuffer.resize(length);

	streamLength = length;
}

#pragma once

#include "common/ITypes.h"
#include "common/ISingleton.h"
#ifdef __MINGW32__
#include <windows.h>
#else
#include <Windows.h>
#endif
/**
 *	Wrapper class for a standard Windows console
 *
 *	@todo make nonblocking
 */
class IConsole : public ISingleton<IConsole>
{
public:
	IConsole();
	~IConsole();

	void Write(char *buf);
	void Write(char *buf, UInt32 bufLen, const char *fmt, ...);

	char ReadChar(void);
	UInt32 ReadBuf(char *buf, UInt32 len);

private:
	HANDLE inputHandle, outputHandle;
};

#include "assert.h"

#include "my_windows.h"


void assert_function(wchar_t const* const& str)
{
	OutputDebugStringW(str);
}

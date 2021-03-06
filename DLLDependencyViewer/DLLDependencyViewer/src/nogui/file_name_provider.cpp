#include "file_name_provider.h"

#include <cassert>


static constexpr wchar_t const s_file_name_err[] = L"Error.";


static file_name_provider* g_file_name_provider = nullptr;


void file_name_provider::init()
{
	assert(!g_file_name_provider);
	g_file_name_provider = new file_name_provider();
}

void file_name_provider::deinit()
{
	assert(g_file_name_provider);
	delete g_file_name_provider;
	g_file_name_provider = nullptr;
}

wstring_handle file_name_provider::get_correct_file_name(wchar_t const* const& file_name, int const& file_name_len, wunique_strings& us, allocator& alc)
{
	assert(g_file_name_provider);
	return g_file_name_provider->get_correct_file_name_(file_name, file_name_len, us, alc);
}

file_name_provider::file_name_provider()
{
	HRESULT const get_clsid = CLSIDFromProgID(L"Scripting.FileSystemObject", &m_clsid);
	if(!(get_clsid == S_OK))
	{
		throw s_file_name_err;
	}
	HRESULT const instance_created = CoCreateInstance(m_clsid, nullptr, CLSCTX_INPROC_SERVER, IID_IDispatch, reinterpret_cast<void**>(&m_object));
	if(!(instance_created == S_OK))
	{
		throw s_file_name_err;
	}
	static constexpr wchar_t const s_method_name[] = L"GetAbsolutePathName";
	wchar_t const* const method_name = s_method_name;
	HRESULT const got_method = m_object->lpVtbl->GetIDsOfNames(m_object, IID_NULL, const_cast<wchar_t**>(&method_name), 1, LOCALE_USER_DEFAULT, &m_method);
	if(!(got_method == S_OK))
	{
		throw s_file_name_err;
	}
}

file_name_provider::~file_name_provider()
{
	m_object->lpVtbl->Release(m_object);
}

wstring_handle file_name_provider::get_correct_file_name_(wchar_t const* const& file_name, int const& file_name_len, wunique_strings& us, allocator& alc)
{
	VARIANT input_1;
	input_1.vt = VT_BSTR;
	input_1.bstrVal = SysAllocStringLen(file_name, file_name_len);
	DISPPARAMS input;
	input.rgvarg = &input_1;
	input.rgdispidNamedArgs = nullptr;
	input.cArgs = 1;
	input.cNamedArgs = 0;
	VARIANT output;
	output.vt = VT_BSTR;
	HRESULT const invoked = m_object->lpVtbl->Invoke(m_object, m_method, m_clsid, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &input, &output, nullptr, nullptr);
	if(!(invoked == S_OK))
	{
		throw s_file_name_err;
	}
	if(!(output.bstrVal))
	{
		throw s_file_name_err;
	}
	int const output_len = static_cast<int>(*reinterpret_cast<std::uint32_t const*>(reinterpret_cast<char const*>(output.bstrVal) - 4));
	if(output_len % 2 != 0)
	{
		throw s_file_name_err;
	}
	wstring_handle const ret = us.add_string(output.bstrVal, output_len / 2, alc);
	SysFreeString(output.bstrVal);
	SysFreeString(input_1.bstrVal);
	return ret;
}

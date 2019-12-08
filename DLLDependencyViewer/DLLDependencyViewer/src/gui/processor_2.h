#pragma once

#include "../nogui/my_string_handle.h"
#include "../nogui/pe.h"

#include <cstdint>


struct file_info_2
{
	void* m_tree_item;
	file_info_2* m_fis;
	file_info_2* m_orig_instance;
	wstring_handle m_file_path;
	bool m_is_32_bit;
	pe_import_table_info m_import_table;
	pe_export_table_info m_export_table;
	std::uint16_t* m_matched_imports;
};
void init(file_info_2* const fi);
void init(file_info_2* const fi, int const count);
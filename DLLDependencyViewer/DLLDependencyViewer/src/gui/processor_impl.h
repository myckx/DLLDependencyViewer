#pragma once


#include "processor.h"

#include "../nogui/allocator.h"
#include "../nogui/dependency_locator.h"
#include "../nogui/memory_manager.h"
#include "../nogui/my_string_handle.h"

#include <cstdint>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>


struct enptr_type
{
	std::uint16_t const* m_table;
	std::uint16_t m_count;
};

struct fat_type
{
	file_info* m_orig_instance;
	enptr_type m_enpt;
};

struct tmp_type
{
	memory_manager* m_mm;
	allocator* m_tmp_alc;
	std::deque<std::pair<wstring_handle, file_info*>> m_queue;
	std::unordered_map<wstring_handle, fat_type*> m_map;
	dependency_locator m_dl;
};


bool process_impl(std::vector<std::wstring> const& file_paths, file_info& fi, memory_manager& mm);

bool step_1(tmp_type& to);
bool step_2(wstring_handle const& file_path, file_info& fi, tmp_type& to);
bool step_3(file_info const& fi, std::uint16_t const i, tmp_type& to);

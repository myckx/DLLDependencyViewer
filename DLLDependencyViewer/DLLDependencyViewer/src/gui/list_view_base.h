#pragma once


namespace list_view_base
{
	int on_columnclick(void const* const param, int const n_headers, int const curr_sort);
	void refresh_headers(void const* const handle, int const n_headers, int const curr_sort);
	void select_item(void const* const hwnd_ptr, void const* const sort_ptr, int const item_idx);
};

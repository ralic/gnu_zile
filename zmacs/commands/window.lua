-- Window handling commands.
--
-- Copyright (c) 2010-2013 Free Software Foundation, Inc.
--
-- This file is part of GNU Zile.
--
-- This program is free software; you can redistribute it and/or modify it
-- under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 3, or (at your option)
-- any later version.
--
-- This program is distributed in the hope that it will be useful, but
-- WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-- General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.


Defun ("delete-window",
       {},
[[
Remove the current window from the screen.
]],
  true,
  function ()
    if #windows == 1 then
      minibuf_error ("Attempt to delete sole ordinary window")
      return false
    end

    delete_window (cur_wp)
  end
)


Defun ("enlarge-window",
       {},
[[
Make current window one line bigger.
]],
  true,
  function ()
    if #windows == 1 then
      return false
    end

    local wp = cur_wp.next
    if not wp or wp.fheight < 3 then
      for _, wp in ipairs (windows) do
        if wp.next == cur_wp then
          if wp.fheight < 3 then
            return false
          end
          break
        end
      end

      if cur_wp == windows[#windows] and cur_wp.next.fheight < 3 then
        return false
      end

      wp.fheight = wp.fheight - 1
      wp.eheight = wp.eheight - 1
      if wp.topdelta >= wp.eheight then
        recenter (wp)
      end
      cur_wp.fheight = cur_wp.fheight + 1
      cur_wp.eheight = cur_wp.eheight + 1
    end
  end
)


Defun ("shrink-window",
       {},
[[
Make current window one line smaller.
]],
  true,
  function ()
    if #windows == 1 or cur_wp.fheight < 3 then
      return false
    end

    local next_wp = window_next (cur_wp)
    next_wp.fheight = next_wp.fheight + 1
    next_wp.eheight = next_wp.eheight + 1
    cur_wp.fheight = cur_wp.fheight - 1
    cur_wp.eheight = cur_wp.eheight - 1
    if cur_wp.topdelta >= cur_wp.eheight then
      recenter (next_wp)
    end
  end
)


Defun ("delete-other-windows",
       {},
[[
Make the selected window fill the screen.
]],
  true,
  function ()
    for _, wp in ipairs (table.clone (windows)) do
      if wp ~= cur_wp then
        delete_window (wp)
      end
    end
  end
)


Defun ("other-window",
       {},
[[
Select the first different window on the screen.
All windows are arranged in a cyclic order.
This command selects the window one step away in that order.
]],
  true,
  function ()
    set_current_window (window_next (cur_wp))
  end
)


Defun ("split-window",
       {},
[[
Split current window into two windows, one above the other.
Both windows display the same buffer now current.
]],
  true,
  function ()
    -- Windows smaller than 4 lines cannot be split.
    if cur_wp.fheight < 4 then
      minibuf_error (string.format ("Window height %d too small (after splitting)", cur_wp.fheight))
      return false
    end

    local newwp = table.clone (cur_wp)
    newwp.fheight = math.floor (cur_wp.fheight / 2) + cur_wp.fheight % 2
    newwp.eheight = newwp.fheight - 1
    newwp.saved_pt = point_marker ()
    table.insert (windows, newwp)

    cur_wp.next = newwp
    cur_wp.fheight = math.floor (cur_wp.fheight / 2)
    cur_wp.eheight = cur_wp.fheight - 1
    if cur_wp.topdelta >= cur_wp.eheight then
      recenter (cur_wp)
    end

    return true
  end
)


Defun ("recenter",
       {},
[[
Center point in selected window and redisplay frame.
]],
  true,
  function ()
    recenter (cur_wp)
    term_clear ()
    term_redisplay ()
    term_refresh ()
    return true
  end
)

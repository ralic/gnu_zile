call_command ("move-end-line")
call_command ("move-previous-word", "2")
insert_string ("a")
call_command ("file-save")
call_command ("file-quit")
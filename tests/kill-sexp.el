(forward-word 2)
(insert ")")
(beginning-of-line)
(insert "(")
(beginning-of-line)
(kill-sexp)
(save-buffer)
(save-buffers-kill-emacs)
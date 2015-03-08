set datafile separator ':'
set datafile commentschar 'm'
plot [-20:150][:27] "/etc/passwd" u 3:($0+2):( stringcolumn(1) . "\n" . stringcolumn(5) ) w labels

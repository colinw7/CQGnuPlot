set_label(x, y, text) = sprintf("set label '%s' at %f, %f point pt 5", text, x, y)

eval set_label(1., 1., 'one/one')
eval set_label(2., 1., 'two/one')
eval set_label(1., 2., 'one/two')

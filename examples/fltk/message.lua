#!/usr/bin/env lua
-- MoonFLTK example: message.lua
--
-- Derived from the FLTK test/message.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function printf(...) io.write(string.format(...)) end

fl.scheme(NULL)
--fl.args(argc, argv)
fl.get_system_colors()

fl.message(string.format("Spelling check sucessful, %d errors found with %g%% confidence",
        1002, 100*(15/77.0)))

fl.alert(string.format(
[[Quantum fluctuations in the space-time continuum detected,
you have %g seconds to comply.

"In physics, spacetime is any mathematical model that combines
space and time into a single construct called the space-time
continuum. Spacetime is usually interpreted with space being
three-dimensional and time playing the role of the
fourth dimension." - Wikipedia]], 10.0))

printf("fl.choice returned %d\n",
   fl.choice("Do you really want to continue?", "No", "Yes"))

printf("fl.choice returned %d\n",
   fl.choice("Choose one of the following:","choice0","choice1","choice2"))

r = fl.input("Please enter a string for 'testing':", "this is the default value")
printf("fl.input returned '%s'\n", r)

r = fl.password("Enter somebody's password:")
printf("fl.password returned '%s'\n", r)

return 0;

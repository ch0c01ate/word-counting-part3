By default program finds config by ../config.dat path from your working repository.

Config file example: config.dat, all lines should be written in format key=value without any spacings, keys must remain
the same as in the example.

Repository input contains input data, result - 10 files with results ordered by number and by name for different threads.

Analysis time:

1 thread: 602639 microseconds
2 threads: 406347 microseconds
3 threads: 386974 microseconds
4 threads: 361510 microseconds

All files give the same result.

For launching Python script, you should enter repetition number and config name.
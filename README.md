# Arduino Simple 24h Clock

- Simple 24h clock/watchdog
- No calendar/date
- Up to 10 events
 
This is simple 24h clock base on watchdog timer, the output is in *24 hour format* - see the examples below. It can't be used as a calendar because it is unable to count over 24 hours. For some simple projects, it's enough. If you want a fully-aimed calendar, then move to use RTC module which is more reliable, less complex and complicated. It supports up to 10 events, you control it through UART for both input and output. The function name of `serialEvent()` might be adjusted depended on Arduino board, some of them only support `serialEvent0()` while other support `serialEvent1()`. You can add more job at `// do sth here` in the function `loop`, feel free to modified it or you can get the time for other purposes.

### Syntax
     [set_time] [get_time] [set_trigger_] [show_triggers] [help] time
### Options
      help          Display this readme
      set_time      Set the current time, if not, it will count from 00:00
          Example: set_time 15:25
      get_time      Get the current time
          Example: get_time
      set_trigger_  Set events to do, have to be specific the event's location
          Example: set_trigger_0 15:26
                   set_trigger_1 15:27
      show_triggers View all events
          Example: show_triggers
      save          Save all records to EEPROM
          Example: save         

# uav-teststand
Motor profiling tool for UAV project

## Serial transmission format

The target Arduino sends bytes back to the python host in this format:

Start transmission:
```
0x00
0x00
```

Data block 1
```
0xXX - RPM
0xXX - thrust
```
...

Data block N

...

End transmission
```
0x00
0x00
```

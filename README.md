# GodoxRemote
Arduino library to "spoof" a Godox RC-A5 433mhz wireless remote, used by most consumer Godox camera lights.

## Info
Should work with most 433mhz transmitters. [Something similar to this was used for testing](https://www.aliexpress.com/item/32840204269.html), although there's loads of others available.

Connect any arduino digital pin to the DIN pin of the antenna. See attached example for usage (it's pretty straightforward).

### Limitations
Currently this only works on channels 1/2, as the complete signal setup hasn't been decoded.
Also temperature has not been implemented yet, as I don't have a light that uses it to test (yet).

## Signal breakdown
The remote sends a 32bit data packet with a final low end-bit, defined by long/short low pulses separated by 750us high pulses.

Here's a signal corresponding to a brightness command of 25% sent to channel 1, group A. (Signal visualised by [https://test.sui.li/oszi/](https://test.sui.li/oszi/))
![signal](https://github.com/BrittonPlewes/GodoxRemote/blob/master/docs/image.png "RC-45 signal")

| GRP     | CHAN    |  value           | cmd | temp        |  checksum        |
|---      |---      |---               |---  |---          |---               |
| 0,0,0,1 | 0,0,0,0 | 0,0,0,1,1,0,0,1  | 0,0 |0,0,1,1,0,0  | 1,0,1,0,0,1,0,1  |
|   A     |    1    |       25         | val |   4400k     |     ?????        |

**Note**: I'm only *guessing* the last byte is a checksum, I can't figure out the encoding. Currently the library works by storing the checksum bytes for all commands in Channels 1/2, groups A-F in a lookup table. If you're reading this and have an idea how to encode this byte programmatically, *please* let me know!

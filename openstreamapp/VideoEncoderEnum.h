#ifndef VIDEOENCODERENUM_H
#define VIDEOENCODERENUM_H

enum VideoEncoder {
    h264CPU,
    h265CPU,
    h264NVENC,
    h265NVENC,
    h264AMDAMF,
    h265AMDAMF
};

#endif // VIDEOENCODERENUM_H

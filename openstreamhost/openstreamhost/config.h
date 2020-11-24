#ifndef SUNSHINE_CONFIG_H
#define SUNSHINE_CONFIG_H

#include <chrono>
#include <string>
#include <bitset>
#include <optional>

namespace config {

//https://forum.videohelp.com/threads/372625-ffmpeg-does-not-apply-parameters-with-libx265
static const std::string x265_default_params = "info=0:keyint=-1:pmode=1:bitrate=0";
static const std::string x264_default_params = "log-level=info";

struct video_t {
  // ffmpeg params
  int crf; // higher == more compression and less quality
  int qp; // higher == more compression and less quality, ignored if crf != 0

  int hevc_mode;
  int vbv_maxrate;
  int vbv_bufsize;
  // in x265 you control the thread limit with --pools config option and
  // with --frame-threads option. https://trac.ffmpeg.org/ticket/3730?cversion=1
  // This struct field 'pools' holds the value for --pools.
  // The value for --frame-threads is taken from 'min_threads' configuration.
  int pools;

  // x265 params are passed as a plain string of concatenated options.
  std::string x265_params;
  std::string x264_params;

  int min_threads; // Minimum number of threads/slices for CPU encoding
  struct {
    std::string preset;
    std::string tune;
  } sw;

  struct {
    std::optional<int> preset;
    std::optional<int> rc;
    int coder;
  } nv;

  std::string encoder;
  std::string adapter_name;
  std::string output_name;
};

struct audio_t {
  std::string sink;
};

struct stream_t {
  std::chrono::milliseconds ping_timeout;

  std::string file_apps;

  int fec_percentage;

  // max unique instances of video and audio streams
  int channels;
};

struct nvhttp_t {
  // Could be any of the following values:
  // pc|lan|wan
  std::string origin_pin_allowed;

  std::string pkey; // must be 2048 bits
  std::string cert; // must be signed with a key of 2048 bits

  std::string sunshine_name;

  std::string file_state;

  std::string external_ip;
};

struct input_t {
  std::chrono::milliseconds back_button_timeout;
  std::chrono::milliseconds key_repeat_delay;
  std::chrono::duration<double> key_repeat_period;
};

namespace flag {
enum flag_e : std::size_t {
  PIN_STDIN = 0, // Read PIN from stdin instead of http
  FRESH_STATE, // Do not load or save state
  FLAG_SIZE
};
}

struct sunshine_t {
  int min_log_level;

  std::bitset<flag::FLAG_SIZE> flags;
};

struct system_priority {
  //1 ABOVE_NORMAL_PRIORITY_CLASS
  //2 HIGH_PRIORITY_CLAS
  //3 REALTIME_PRIORITY_CLASS
  //Default ABOVE_NORMAL_PRIORITY_CLASS
  int priority_class;
};

extern video_t video;
extern audio_t audio;
extern stream_t stream;
extern nvhttp_t nvhttp;
extern input_t input;
extern sunshine_t sunshine;
extern system_priority sys_priority;

int parse(int argc, char *argv[]);

void update_x265_options();
void update_x264_options();
}

#endif
